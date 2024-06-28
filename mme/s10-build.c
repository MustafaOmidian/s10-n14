#include "s10-build.h"
/* Function to build Context Request */
ogs_pkbuf_t *s10_build_context_request(const char *ue_id, const char *serving_mme_ip, const uint32_t mme_teid) {
    // Allocate memory for the message buffer
    ogs_pkbuf_t *pkbuf = ogs_pkbuf_alloc(S10_CONTEXT_REQUEST_MAX_LEN);
    if (!pkbuf) {
        return NULL;
    }

    // Initialize message headers
    set_diameter_header(pkbuf, GTPC_CONTEXT_REQUEST, 0, 0, 0, 0);

    // Add IEs
    add_diameter_ie(pkbuf, IMSI, ue_id, strlen(ue_id));
    add_diameter_ie(pkbuf, SERVING_MME_IP, serving_mme_ip, strlen(serving_mme_ip));
    add_diameter_ie(pkbuf, MMETEID, &mme_teid, sizeof(mme_teid));

    // Finalize the message
    finalize_diameter_message(pkbuf);

    return pkbuf;
}

/* Function to build Context Response */
ogs_pkbuf_t *s10_build_context_response(const char *target_mme_ip, const uint32_t target_mme_teid, const bool forward_relocation, const bool relocation_accepted) {
    // Allocate memory for the message buffer
    ogs_pkbuf_t *pkbuf = ogs_pkbuf_alloc(S10_CONTEXT_RESPONSE_MAX_LEN);
    if (!pkbuf) {
        return NULL;
    }

    // Initialize message headers
    set_diameter_header(pkbuf, GTPC_CONTEXT_RESPONSE, 0, 0, 0, 0);

    // Add IEs
    if (forward_relocation) {
        if (relocation_accepted) {
            add_diameter_ie(pkbuf, FTEID, &target_mme_teid, sizeof(target_mme_teid));
        } else {
            add_diameter_ie(pkbuf, REDIRECT_HOST, target_mme_ip, strlen(target_mme_ip));
            add_diameter_ie(pkbuf, MMETEID, &target_mme_teid, sizeof(target_mme_teid));
        }
    } else {
        if (relocation_accepted) {
            add_diameter_ie(pkbuf, REDIRECT_HOST, target_mme_ip, strlen(target_mme_ip));
            add_diameter_ie(pkbuf, MMETEID, &target_mme_teid, sizeof(target_mme_teid));
        } else {
            add_diameter_ie(pkbuf, ERROR_CODE, &rejection_reason, sizeof(rejection_reason));
        }
    }

    // Finalize the message
    finalize_diameter_message(pkbuf);

    return pkbuf;
}
/* Function to build Context Acknowledge */
ogs_pkbuf_t *s10_build_context_acknowledge(const char *target_mme_ip, const uint32_t target_mme_teid) {
    // Allocate memory for the message buffer
    ogs_pkbuf_t *pkbuf = ogs_pkbuf_alloc(S10_CONTEXT_ACKNOWLEDGE_MAX_LEN);
    if (!pkbuf) {
        return NULL;
    }

    // Initialize message headers
    set_diameter_header(pkbuf, GTPC_CONTEXT_ACKNOWLEDGE, 0, 0, 0, 0);

    // Add IEs
    add_diameter_ie(pkbuf, REDIRECT_HOST, target_mme_ip, strlen(target_mme_ip));
    add_diameter_ie(pkbuf, MMETEID, &target_mme_teid, sizeof(target_mme_teid));

    // Finalize the message
    finalize_diameter_message(pkbuf);

    return pkbuf;
}
/* Function to build Forward Relocation Request */
ogs_pkbuf_t *s10_build_forward_relocation_request(const char *ue_id, const char *serving_mme_ip, const uint32_t serving_mme_teid, const char *target_mme_ip, const uint32_t target_mme_teid) {
    // Allocate memory for the message buffer
    ogs_pkbuf_t *pkbuf = ogs_pkbuf_alloc(S10_FORWARD_RELOCATION_REQUEST_MAX_LEN);
    if (!pkbuf) {
        return NULL;
    }

    // Initialize message headers
    set_diameter_header(pkbuf, GTPC_FORWARD_RELOCATION_REQUEST, 0, 0, 0, 0);

    // Add IEs
    add_diameter_ie(pkbuf, IMSI, ue_id, strlen(ue_id));
    add_diameter_ie(pkbuf, SERVING_MME_IP, serving_mme_ip, strlen(serving_mme_ip));
    add_diameter_ie(pkbuf, MMETEID, &serving_mme_teid, sizeof(serving_mme_teid));
    add_diameter_ie(pkbuf, TARGET_MME_IP, target_mme_ip, strlen(target_mme_ip));
    add_diameter_ie(pkbuf, MMETEID, &target_mme_teid, sizeof(target_mme_teid));

    // Finalize the message
    finalize_diameter_message(pkbuf);
    
return pkbuf;
}
/* Function to build Forward Relocation Response */
ogs_pkbuf_t *s10_build_forward_relocation_response(const char *target_mme_ip, const uint32_t target_mme_teid, const bool relocation_accepted) {
    // Allocate memory for the message buffer
    ogs_pkbuf_t *pkbuf = ogs_pkbuf_alloc(S10_FORWARD_RELOCATION_RESPONSE_MAX_LEN);
    if (!pkbuf) {
        return NULL;
    }

    // Initialize message headers
    set_diameter_header(pkbuf, GTPC_FORWARD_RELOCATION_RESPONSE, 0, 0, 0, 0);

    // Add IEs
    if (relocation_accepted) {
        add_diameter_ie(pkbuf, FTEID, &target_mme_teid, sizeof(target_mme_teid));
    } else {
        add_diameter_ie(pkbuf, ERROR_CODE, &rejection_reason, sizeof(rejection_reason));
    }

    // Finalize the message
    finalize_diameter_message(pkbuf);

    return pkbuf;
}

/* Function to build Forward Relocation Complete Notification */
ogs_pkbuf_t *s10_build_forward_relocation_complete_notification(const char *target_mme_ip, const uint32_t target_mme_teid) {
  // Allocate memory for the message buffer
  ogs_pkbuf_t *pkbuf = ogs_pkbuf_alloc(S10_FORWARD_RELOCATION_COMPLETE_NOTIFICATION_MAX_LEN);
  if (!pkbuf) {
    return NULL;
  }

  // Initialize message headers
  set_diameter_header(pkbuf, GTPC_FORWARD_RELOCATION_COMPLETE_NOTIFICATION, 0, 0, 0, 0);

  // Add IEs
  add_diameter_ie(pkbuf, REDIRECT_HOST, target_mme_ip, strlen(target_mme_ip));
  add_diameter_ie(pkbuf, MMETEID, &target_mme_teid, sizeof(target_mme_teid));

  // Finalize the message
  finalize_diameter_message(pkbuf);

  return pkbuf;
}

/* Function to build Forward Relocation Complete Acknowledge */
ogs_pkbuf_t *s10_build_forward_relocation_complete_acknowledge(const char *serving_mme_ip, const uint32_t serving_mme_teid) {
  // Allocate memory for the message buffer
  ogs_pkbuf_t *pkbuf = ogs_pkbuf_alloc(S10_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE_MAX_LEN);
  if (!pkbuf) {
    return NULL;
  }

  // Initialize message headers
  set_diameter_header(pkbuf, GTPC_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE, 0, 0, 0, 0);

  // Add IEs
  add_diameter_ie(pkbuf, SERVING_MME_IP, serving_mme_ip, strlen(serving_mme_ip));
  add_diameter_ie(pkbuf, MMETEID, &serving_mme_teid, sizeof(serving_mme_teid));

  // Finalize the message
  finalize_diameter_message(pkbuf);

  return pkbuf;
}