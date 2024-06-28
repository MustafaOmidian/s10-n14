#include "n14-path.h"
#include "n14-context.h"

int n14_path_init(void) {
    ogs_log_info("Initializing N14 path");

    // Initialize the N14 context
    n14_context_init();

    ogs_log_info("N14 path initialized successfully");
    return OGS_OK;
}

void n14_path_final(void) {
    ogs_log_info("Finalizing N14 path");

    // Finalize the N14 context
    n14_context_final();

    ogs_log_info("N14 path finalized successfully");
}

int n14_path_open(void) {
    ogs_log_info("Opening N14 path");

    // Initialize the IMSI to UE hash table
    n14_self()->imsi_ue_hash = ogs_hash_make();
    if (!n14_self()->imsi_ue_hash) {
        ogs_error("Failed to create IMSI to UE hash table");
        return OGS_ERROR;
    }

    // Initialize the N14 session hash table
    n14_self()->n14_sess_hash = ogs_hash_make();
    if (!n14_self()->n14_sess_hash) {
        ogs_error("Failed to create N14 session hash table");
        ogs_hash_destroy(n14_self()->imsi_ue_hash);
        return OGS_ERROR;
    }

    // Initialize the UE list
    ogs_list_init(&n14_self()->ue_list);

    ogs_log_info("N14 path opened successfully");
    return OGS_OK;
}

void n14_path_close(void) {
    ogs_log_info("Closing N14 path");

    // Destroy the IMSI to UE hash table
    if (n14_self()->imsi_ue_hash) {
        ogs_hash_destroy(n14_self()->imsi_ue_hash);
        n14_self()->imsi_ue_hash = NULL;
    }

    // Destroy the N14 session hash table
    if (n14_self()->n14_sess_hash) {
        ogs_hash_destroy(n14_self()->n14_sess_hash);
        n14_self()->n14_sess_hash = NULL;
    }

    // Remove all UEs from the list
    n14_ue_remove_all();

    ogs_log_info("N14 path closed successfully");
}