#ifndef S10_PATH_H

#define S10_PATH_H


#include "context.h" // Include the context or relevant structures

#include "ogs-gtp.h" // Include the necessary GTP library definitions


#ifdef __cplusplus

extern "C" {

#endif


// Function declarations for opening and closing the S10 path

int s10_init(void);

void s10_final(void);


// Function declarations for sending and receiving messages on the S10 interface

int s10_send(ogs_pkbuf_t *pkbuf);

int s10_receive(ogs_pkbuf_t **pkbuf);


// Function declaration for handling incoming messages on the S10 interface

void s10_handle_incoming(void);


// Function declarations for building S10 GTPv2 messages

ogs_pkbuf_t *s10_build_context_request(

    const char *ue_id,

    const char *serving_mme_ip,

    const uint32_t mme_teid

);


ogs_pkbuf_t *s10_build_context_response(

    const char *target_mme_ip,

    const uint32_t target_mme_teid,

    const bool forward_relocation,

    const bool relocation_accepted

);


ogs_pkbuf_t *s10_build_context_acknowledge(

    const char *target_mme_ip,

    const uint32_t target_mme_teid

);


ogs_pkbuf_t *s10_build_forward_relocation_request(

    const char *ue_id,

    const char *serving_mme_ip,

    const uint32_t serving_mme_teid,

    const char *target_mme_ip,

    const uint32_t target_mme_teid

);


ogs_pkbuf_t *s10_build_forward_relocation_response(

    const char *target_mme_ip,

    const uint32_t target_mme_teid,

    const bool relocation_accepted

);


ogs_pkbuf_t *s10_build_forward_relocation_complete_notification(

    const char *target_mme_ip,

    const uint32_t target_mme_teid

);


ogs_pkbuf_t *s10_build_forward_relocation_complete_acknowledge(

    const char *serving_mme_ip,

    const uint32_t serving_mme_teid

);


// ... additional message building function prototypes ...


#ifdef __cplusplus

}

#endif


#endif /* S10_PATH_H */