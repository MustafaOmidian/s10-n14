#include "context.h"

#include "gtp-path.h"

#include "s10-context.h"

#include "s10-path.h"


// Initialization function for the S10 interface

int s10_init(void) {

    ogs_gtp_node_t *gtpc = NULL;


    // Allocate and initialize S10 context

    s10_context_t *s10_ctx = s10_context_init();

    if (s10_ctx == NULL) {

        ogs_error("Failed to initialize S10 context");

        return OGS_ERROR;

    }


    // Create a GTP-C node for S10 interface

    gtpc = ogs_gtp_node_new(OGS_GTP_C_NODE);

    if (gtpc == NULL) {

        ogs_error("Failed to create GTP-C node for S10");

        s10_context_final();

        return OGS_ERROR;

    }


    // Configure the GTP-C node with S10 interface information

    gtpc->local_addr = ogs_calloc(1, sizeof(ogs_sockaddr_t));

    if (gtpc->local_addr == NULL) {

        ogs_error("Failed to allocate local address for S10 GTP-C node");

        ogs_gtp_node_free(gtpc);

        s10_context_final();

        return OGS_ERROR;

    }

    // Set the local address and port for the GTP-C node

    ogs_sockaddr_init(AF_INET, gtpc->local_addr, s10_ctx->config.bind_addr, s10_ctx->config.port);


    // Open the socket for the GTP-C node

    if (ogs_gtp_open(gtpc) != OGS_OK) {

        ogs_error("Failed to open socket for S10 GTP-C node");

        ogs_free(gtpc->local_addr);

        ogs_gtp_node_free(gtpc);

        s10_context_final();

        return OGS_ERROR;

    }


    // Save the GTP-C node in the S10 context

    s10_ctx->gtpc_node = gtpc;


    ogs_info("S10 GTP-C interface initialized");

    return OGS_OK;

}


// Finalization function for the S10 interface

void s10_final(void) {
    s10_context_t *s10_ctx = s10_self(); // Retrieve the S10 context

    if (s10_ctx) {
        // Close the GTP-C node if it has been initialized
        if (s10_ctx->gtpc_node) {
            ogs_gtp_node_free(s10_ctx->gtpc_node);
            s10_ctx->gtpc_node = NULL;
        }

        // Finalize the S10 context itself
        s10_context_final();
    }

    ogs_info("S10 GTP-C interface finalized");
}


// Function to build a GTPv2-C Context Request message

ogs_pkbuf_t *s10_build_context_request(

    const char *ue_id, 

    const char *serving_mme_ip, 

    const uint32_t mme_teid

) {

    ogs_pkbuf_t *pkbuf = ogs_pkbuf_alloc(S10_CONTEXT_REQUEST_MAX_LEN);

    if (!pkbuf) {

        return NULL; // Failed to allocate pkbuf

    }


    // Initialize message headers for GTPv2-C Context Request

    ogs_gtp_header_t *hdr = ogs_pkbuf_push(pkbuf, sizeof(ogs_gtp_header_t));

    hdr->type = GTPC_CONTEXT_REQUEST;

    hdr->teid = 0; // TEID is not used in the initial request

    hdr->length = 0; // Will be set after adding all IEs

    hdr->sequence_number = ogs_gtp_new_sequence(); // Assuming a function to generate sequence numbers

    hdr->message_priority = 0; // Default priority

    hdr->spare = 0; // Spare bits, set to 0


    // Add IEs to the message

    ogs_gtp_add_ie(pkbuf, OGS_GTP_IE_IMSI, strlen(ue_id), ue_id);

    ogs_gtp_add_ie(pkbuf, OGS_GTP_IE_FQDN, strlen(serving_mme_ip), serving_mme_ip);

    ogs_gtp_add_ie(pkbuf, OGS_GTP_IE_TEID_DATA_I, sizeof(mme_teid), &mme_teid);


    // Update the length in the GTP header

    hdr->length = htons(pkbuf->len - sizeof(ogs_gtp_header_t));


    // Finalize the message by setting the header flags and length

    ogs_gtp_finalize_message(pkbuf, hdr->type);


    return pkbuf;

}


// Function to build a GTPv2-C Context Response message

ogs_pkbuf_t *s10_build_context_response(

    const char *target_mme_ip, 

    const uint32_t target_mme_teid, 

    const bool forward_relocation, 

    const bool relocation_accepted

) {

    // Allocate memory for the message buffer

    ogs_pkbuf_t *pkbuf = ogs_pkbuf_alloc(S10_CONTEXT_RESPONSE_MAX_LEN);

    if (!pkbuf) {

        return NULL; // Failed to allocate pkbuf

    }


    // Initialize message headers

    ogs_gtp_header_t *hdr = ogs_pkbuf_push(pkbuf, sizeof(ogs_gtp_header_t));

    hdr->type = GTPC_CONTEXT_RESPONSE;

    hdr->teid = target_mme_teid; // TEID of the target MME

    hdr->length = 0; // Will be set after adding all IEs

    hdr->sequence_number = ogs_gtp_new_sequence(); // Assuming a function to generate sequence numbers

    hdr->message_priority = 0; // Default priority

    hdr->spare = 0; // Spare bits, set to 0


    // Add IEs to the message

    if (forward_relocation) {

        if (relocation_accepted) {

            // Add F-TEID IE for the target MME

            ogs_gtp_add_ie(pkbuf, OGS_GTP_IE_F_TEID, sizeof(target_mme_teid), &target_mme_teid);

        } else {

            // Add Redirect Host IE for the target MME

            ogs_gtp_add_ie(pkbuf, OGS_GTP_IE_REDIRECT_HOST, strlen(target_mme_ip), target_mme_ip);

        }

    } else {

        // Add Cause IE to indicate rejection

        uint8_t cause_value = OGS_GTP_CAUSE_REQUEST_REJECTED; // Assuming a defined cause value for rejection

        ogs_gtp_add_ie(pkbuf, OGS_GTP_IE_CAUSE, sizeof(cause_value), &cause_value);

    }


    // Update the length in the GTP header

    hdr->length = htons(pkbuf->len - sizeof(ogs_gtp_header_t));


    // Finalize the message by setting the header flags and length

    ogs_gtp_finalize_message(pkbuf, hdr->type);


    return pkbuf;

}


// Function to send a GTPv2-C message over the S10 interface


int s10_send(ogs_pkbuf_t *pkbuf) {

    s10_context_t *s10_ctx = s10_self(); // Retrieve the S10 context

    ogs_gtp_node_t *gtpc_node = NULL;


    if (s10_ctx == NULL) {

        ogs_error("S10 context is not initialized");

        ogs_pkbuf_free(pkbuf);

        return OGS_ERROR;

    }


    gtpc_node = s10_ctx->gtpc_node;

    if (gtpc_node == NULL) {

        ogs_error("S10 GTP-C node is not initialized");

        ogs_pkbuf_free(pkbuf);

        return OGS_ERROR;

    }


    // Send the message using the GTP-C node

    int rv = ogs_gtp_send(gtpc_node, pkbuf);

    if (rv != OGS_OK) {

        ogs_error("Failed to send message on S10 interface");

        // ogs_pkbuf_free(pkbuf); // pkbuf is normally freed by ogs_gtp_send

        return OGS_ERROR;

    }


    return OGS_OK;

}

