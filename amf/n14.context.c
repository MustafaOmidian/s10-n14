#include "n14-context.h"

int __n14_log_domain = OGS_LOG_DOMAIN;

static n14_context_t n14_self_context;
static n14_context_t *n14_self_ptr = &n14_self_context;

void n14_context_init(void) {
    ogs_log_info("Initializing N14 context");

    ogs_list_init(&n14_self_ptr->ue_list);
    n14_self_ptr->imsi_ue_hash = ogs_hash_make();
    ogs_assert(n14_self_ptr->imsi_ue_hash);

    n14_self_ptr->n14_sess_hash = ogs_hash_make();
    ogs_assert(n14_self_ptr->n14_sess_hash);
}

void n14_context_final(void) {
    ogs_log_info("Finalizing N14 context");

    n14_ue_remove_all();

    if (n14_self_ptr->imsi_ue_hash) {
        ogs_hash_destroy(n14_self_ptr->imsi_ue_hash);
        n14_self_ptr->imsi_ue_hash = NULL;
    }

    if (n14_self_ptr->n14_sess_hash) {
        ogs_hash_destroy(n14_self_ptr->n14_sess_hash);
        n14_self_ptr->n14_sess_hash = NULL;
    }
}

n14_context_t *n14_self(void) {
    return n14_self_ptr;
}

n14_ue_t *n14_ue_add(uint8_t *imsi, int imsi_len) {
    n14_ue_t *ue = ogs_calloc(1, sizeof(n14_ue_t));
    ogs_assert(ue);

    memcpy(ue->imsi, imsi, imsi_len);
    ue->imsi_len = imsi_len;
    ogs_sbi_bcd_from_imsi(ue->imsi_bcd, imsi, imsi_len);

    ogs_list_init(&ue->sess_list);
    ogs_list_add(&n14_self_ptr->ue_list, &ue->lnode);

    ogs_hash_set(n14_self_ptr->imsi_ue_hash, ue->imsi_bcd, ue);

    return ue;
}

int n14_ue_remove(n14_ue_t *ue) {
    ogs_assert(ue);

    n14_sess_remove_all(ue);

    ogs_list_remove(&n14_self_ptr->ue_list, &ue->lnode);
    ogs_hash_remove(n14_self_ptr->imsi_ue_hash, ue->imsi_bcd);

    ogs_free(ue);

    return OGS_OK;
}

void n14_ue_remove_all(void) {
    n14_ue_t *ue, *next;

    for (ue = ogs_list_first(&n14_self_ptr->ue_list); ue; ue = next) {
        next = ogs_list_next(ue);
        n14_ue_remove(ue);
    }
}

n14_sess_t *n14_sess_add(n14_ue_t *ue, uint64_t session_id) {
    n14_sess_t *sess = ogs_calloc(1, sizeof(n14_sess_t));
    ogs_assert(sess);

    sess->session_id = session_id;
    sess->ue = ue;

    ogs_list_add(&ue->sess_list, &sess->lnode);
    ogs_hash_set(n14_self_ptr->n14_sess_hash, &sess->session_id, sess);

    return sess;
}

int n14_sess_remove(n14_sess_t *sess) {
    ogs_assert(sess);

    ogs_list_remove(&sess->ue->sess_list, &sess->lnode);
    ogs_hash_remove(n14_self_ptr->n14_sess_hash, &sess->session_id);

    ogs_free(sess);

    return OGS_OK;
}

void n14_sess_remove_all(n14_ue_t *ue) {
    n14_sess_t *sess, *next;

    for (sess = ogs_list_first(&ue->sess_list); sess; sess = next) {
        next = ogs_list_next(sess);
        n14_sess_remove(sess);
    }
}

n14_ue_t *n14_ue_find_by_imsi(uint8_t *imsi, int imsi_len) {
    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN + 1];
    ogs_sbi_bcd_from_imsi(imsi_bcd, imsi, imsi_len);
    return ogs_hash_get(n14_self_ptr->imsi_ue_hash, imsi_bcd);
}

n14_sess_t *n14_sess_find_by_id(uint64_t session_id) {
    return ogs_hash_get(n14_self_ptr->n14_sess_hash, &session_id);
}