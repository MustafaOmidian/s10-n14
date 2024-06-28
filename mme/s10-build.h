#ifndef S10_BUILD_H
#define S10_BUILD_H

#include "ogs-gtp.h" // Include the necessary GTP library definitions

#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes for building S10 GTPv2 messages

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

#endif /* S10_BUILD_H */