#include "s10-context.h"
#include "s10-build.h"

/* Handler for Context Request */
void handle_context_request(void) {
  // Receive the S10 Context Request message from the MME
  ogs_pkbuf_t *req = ogs_pkbuf_alloc(S10_CONTEXT_REQUEST_MAX_LEN);
  if (!req) {
    return;
  }

  // Receive the Diameter message from the MME
  if (receive_s10_message(req) != S10_CONTEXT_REQUEST) {
    ogs_pkbuf_free(req);
    return;
  }

  // Extract the UE's IMSI and serving MME IP address from the message
  const char *imsi = get_diameter_ie_value(req, IMSI);
  const char *serving_mme_ip = get_diameter_ie_value(req, SERVING_MME_IP);

  // Check if the UE is already registered in the serving MME
  if (is_ue_registered(imsi)) {
    // Build an S10 Context Acknowledge message with the serving MME IP address
    ogs_pkbuf_t *ack = s10_build_context_acknowledge(serving_mme_ip);

    // Send the S10 Context Acknowledge message to the MME
    send_s10_message(ack);
  } else {
    // Inform the MME that the UE is not registered
    ogs_pkbuf_t *error_response = s10_build_error_response(S10_CTX_REQ_FAILED, "UE Not Registered");

    // Send the S10 Error Response message to the MME
    send_s10_message(error_response);
  }

  // Free the request buffer
  ogs_pkbuf_free(req);
}

/* Handler for Context Response */
void handle_context_response(void) {
  // Receive the S10 Context Response message from the MME
  ogs_pkbuf_t *resp = ogs_pkbuf_alloc(S10_CONTEXT_RESPONSE_MAX_LEN);
  if (!resp) {
    return;
  }

  // Receive the Diameter message from the MME
  if (receive_s10_message(resp) != S10_CONTEXT_RESPONSE) {
    ogs_pkbuf_free(resp);
    return;
  }

  // Check if the UE is relocated
  const char *fteid = get_diameter_ie_value(resp, FTEID);
  if (fteid) {

    // UE is relocated, update the serving MME IP and TEID

    const char *serving_mme_ip = get_diameter_ie_value(resp, SERVING_MME_IP);

    const uint32_t serving_mme_teid = get_diameter_ie_value_uint32(resp, MMETEID);

    update_ue_serving_mme(serving_mme_ip, serving_mme_teid);


    // Notify the MME of the UE relocation

    ogs_pkbuf_t *notify = s10_build_forward_relocation_complete_notification(get_diameter_ie_value(resp, TARGET_MME_IP));

    send_s10_message(notify);

  } else {
    // UE is not relocated, acknowledge the MME
    ogs_pkbuf_t *ack = s10_build_forward_relocation_complete_acknowledge(get_diameter_ie_value(resp, SERVING_MME_IP));
    send_s10_message(ack);
  }

  // Free the response buffer
  ogs_pkbuf_free(resp);
}

/* Handler for Context Acknowledge */
void handle_context_acknowledge(void) {
  // Receive the S10 Context Acknowledge message from the MME
  ogs_pkbuf_t *ack = ogs_pkbuf_alloc(S10_CONTEXT_ACKNOWLEDGE_MAX_LEN);
  if (!ack) {
    return;
  }

  // Receive the Diameter message from the MME
  if (receive_s10_message(ack) != S10_CONTEXT_ACKNOWLEDGE) {
    ogs_pkbuf_free(ack);
    return;
  }

  // Extract the UE's IMSI, serving MME IP address, and TEID from the message
  const char *imsi = get_diameter_ie_value(ack, IMSI);
  const char *serving_mme_ip = get_diameter_ie_value(ack, SERVING_MME_IP);
  const uint32_t serving_mme_teid = get_diameter_ie_value_uint32(ack, MMETEID);

  // Update the serving MME IP address and TEID for the UE in the database
  update_ue_serving_mme(serving_mme_ip, serving_mme_teid);

  // Notify the MME of the successful context acknowledge
  ogs_pkbuf_t *notify = s10_build_forward_relocation_complete_notification(serving_mme_ip);
  send_s10_message(notify);

  // Free the acknowledge buffer
  ogs_pkbuf_free(ack);
}

/* Handler for Forward Relocation Request */
void handle_forward_relocation_request(void) {
    // Receive the S10 Forward Relocation Request message from the MME
    ogs_pkbuf_t *req = ogs_pkbuf_alloc(S10_FORWARD_RELOCATION_REQUEST_MAX_LEN);
    if (!req) {
        return;
    }

    // Receive the Diameter message from the MME
    if (receive_s10_message(req) != S10_FORWARD_RELOCATION_REQUEST) {
        ogs_pkbuf_free(req);
        return;
    }

    // Extract the UE's IMSI, serving MME IP address, target MME IP address, and TEID from the message
    const char *imsi = get_diameter_ie_value(req, IMSI);
    const char *serving_mme_ip = get_diameter_ie_value(req, SERVING_MME_IP);
    const char *target_mme_ip = get_diameter_ie_value(req, TARGET_MME_IP);
    const uint32_t target_mme_teid = get_diameter_ie_value_uint32(req, MMETEID);

    // Check if the UE is registered with the serving MME
    if (!is_ue_registered(imsi)) {
        // UE not registered, send an error response to the MME
        ogs_pkbuf_t *error_response = s10_build_error_response(S10_FWL_REQ_FAILED, "UE Not Registered");
        send_s10_message(error_response);
    } else {
        // UE is registered, proceed with forwarding the location

        // Update the MME database with the new target MME IP address and TEID
        update_ue_serving_mme(target_mme_ip, target_mme_teid);
        // needs to triger a createsession request and get a create session response here
        
        // Send a forward relocation response to the MME
        ogs_pkbuf_t *resp = s10_build_forward_relocation_response(target_mme_ip, target_mme_teid, true);
        send_s10_message(resp);

        // Notify the target MME of the new serving MME
        ogs_pkbuf_t *notify = s10_build_context_acknowledge(target_mme_ip);
        send_s10_message(notify);
    }

    // Free the request buffer
    ogs_pkbuf_free(req);
}

/* Handler for Forward Relocation Response */
void handle_forward_relocation_response(void) {
  // Receive the S10 Forward Relocation Response message from the MME
  ogs_pkbuf_t *resp = ogs_pkbuf_alloc(S10_FORWARD_RELOCATION_RESPONSE_MAX_LEN);
  if (!resp) {
    return;
  }

  // Receive the Diameter message from the MME
  if (receive_s10_message(resp) != S10_FORWARD_RELOCATION_RESPONSE) {
    ogs_pkbuf_free(resp);
    return;
  }

  // Check if the UE was successfully relocated
  bool relocation_successful = get_diameter_ie_value_bool(resp, FTEID);

  // Handle successful relocation
  if (relocation_successful) {
    // Remove the UE from the serving MME database
    remove_ue_from_serving_mme(mme_ue_t *mme_ue)

    // Send a forward relocation complete notification to the target MME
    ogs_pkbuf_t *notify = s10_build_forward_relocation_complete_notification(get_diameter_ie_value(resp, TARGET_MME_IP));
    send_s10_message(notify);
  }

  // Handle unsuccessful relocation
  else {
    // Inform the MME of the unsuccessful relocation
    ogs_pkbuf_t *error_response = s10_build_error_response(S10_FWL_RSP_FAILED, "Relocation Failed");
    send_s10_message(error_response);
  }

  // Free the response buffer
  ogs_pkbuf_free(resp);
}

/* Handler for Forward Relocation Complete Notification */
void handle_forward_relocation_complete_notification(

    mme_ue_s1ap_id_t mme_ue_s1ap_id,

    const char *serving_mme_ip,

    uint32_t mme_teid)

{
  // Receive the S10 Forward Relocation Complete Notification message from the MME
  ogs_pkbuf_t *notify = ogs_pkbuf_alloc(S10_FORWARD_RELOCATION_COMPLETE_NOTIFICATION_MAX_LEN);
  if (!notify) {
    return;
  }

  // Receive the Diameter message from the MME
  if (receive_s10_message(notify) != S10_FORWARD_RELOCATION_COMPLETE_NOTIFICATION) {
    ogs_pkbuf_free(notify);
    return;
  }

  // Extract the target MME IP address
  const char *target_mme_ip = get_diameter_ie_value(notify, TARGET_MME_IP);

  // Update the serving MME IP address in the UE database to the target MME's IP address
    update_ue_serving_mme(serving_mme_ip, mme_teid);
  // Send a context acknowledge to the MME to indicate the successful deletion of the UE from the serving MME's database
  ogs_pkbuf_t *ack = ogs_pkbuf_alloc(S10_CONTEXT_ACKNOWLEDGE_MAX_LEN);
  if (!ack) {
    ogs_pkbuf_free(notify);
    return;
  }

  s10_build_context_acknowledge(ack, target_mme_ip);
  send_s10_message(ack);

  // Remove the UE from the serving MME database
  remove_ue_from_serving_mme(mme_ue_t *mme_ue);

  // Free the notification buffer and acknowledgment buffer
  ogs_pkbuf_free(notify);
  ogs_pkbuf_free(ack);
}

/* Handler for Forward Relocation Complete Acknowledge */
void handle_forward_relocation_complete_acknowledge(

    ogs_s10_t *s10, ogs_gtpv2c_cause_t *cause, ogs_gtpv2c_teid_t *teid)

{
  // Receive the S10 Forward Relocation Complete Acknowledge from the MME
  ogs_pkbuf_t *ack = ogs_pkbuf_alloc(S10_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE_MAX_LEN);
  if (!ack) {
    return;
  }


// Receive the Diameter message from the MME
  if (receive_s10_message(ack) != S10_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE) {
    ogs_pkbuf_free(ack);
    return;
  }

  // Extract the serving MME IP address from the MME
  const char *serving_mme_ip = get_diameter_ie_value(ack, SERVING_MME_IP);

  // Update the UE serving MME IP in the UE database to the serving MME's IP address
  update_ue_serving_mme(serving_mme_ip, teid);
  // Free the acknowledgment buffer
  ogs_pkbuf_free(ack);
}