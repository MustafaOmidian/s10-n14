/*
 * This would be the header for the S10 interface context in the Open5GS project.
 * The actual content and structure would depend on the specific requirements
 * and design of the S10 interface within the Open5GS framework.
 */

#ifndef S10_CONTEXT_H
#define S10_CONTEXT_H

#include "context.h"
#include "ogs-app.h"
#include "ogs-gtp.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __s10_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __s10_log_domain

#define GTPC_CONTEXT_REQUEST 130
#define GTPC_CONTEXT_RESPONSE 131
#define GTPC_CONTEXT_ACKNOWLEDGE 132
#define GTPC_FORWARD_RELOCATION_REQUEST 133
#define GTPC_FORWARD_RELOCATION_RESPONSE 134
#define GTPC_FORWARD_RELOCATION_COMPLETE_NOTIFICATION 135
#define GTPC_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE 136

typedef struct s10_context_s {
    ogs_gtp_node_t *gtpc_node; // GTP-C node for S10 interface

    // Hash tables, lists, and other data structures to manage the state and configuration of the S10 interface
    ogs_hash_t *imsi_ue_hash;   /* hash table (IMSI : UE context) */
    ogs_hash_t *mme_teid_hash;  /* hash table (MME-TEID : UE context) */
    ogs_list_t ue_list;         /* List of UE contexts */
} s10_context_t;

void s10_context_init(void);
void s10_context_final(void);
s10_context_t *s10_self(void);

// Additional functions for managing the S10 context
s10_context_t *s10_context_create(void);
void s10_context_destroy(s10_context_t *context);
int s10_context_add_ue(s10_context_t *context, const char *imsi, uint32_t mme_teid);
void s10_context_remove_ue(s10_context_t *context, const char *imsi);
void s10_context_remove_all_ues(s10_context_t *context);
s10_context_t *s10_context_find_by_imsi(s10_context_t *context, const char *imsi);
s10_context_t *s10_context_find_by_teid(s10_context_t *context, uint32_t teid);

#ifdef __cplusplus
}
#endif

#endif /* S10_CONTEXT_H */