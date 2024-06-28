/*
 * Copyright (C) 2019-2023 by Sukchan Lee
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef N14_CONTEXT_H
#define N14_CONTEXT_H

#include "ogs-app.h"
#include "ogs-sbi.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __n14_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __n14_log_domain

typedef struct n14_context_s {
    ogs_hash_t *imsi_ue_hash;   /* hash table (IMSI : N14_UE) */
    ogs_hash_t *n14_sess_hash;  /* hash table (N14-Session-ID : N14_Session) */
    ogs_list_t ue_list;         /* N14_UE List */
} n14_context_t;

typedef struct n14_ue_s {
    ogs_lnode_t lnode;
    uint8_t imsi[OGS_MAX_IMSI_LEN];
    int imsi_len;
    char imsi_bcd[OGS_MAX_IMSI_BCD_LEN + 1];
    ogs_list_t sess_list;
} n14_ue_t;

typedef struct n14_sess_s {
    ogs_lnode_t lnode;
    uint64_t session_id;
    n14_ue_t *ue;
    ogs_sbi_message_t *message;
} n14_sess_t;

void n14_context_init(void);
void n14_context_final(void);
n14_context_t *n14_self(void);

n14_ue_t *n14_ue_add(uint8_t *imsi, int imsi_len);
int n14_ue_remove(n14_ue_t *ue);
void n14_ue_remove_all(void);

n14_sess_t *n14_sess_add(n14_ue_t *ue, uint64_t session_id);
int n14_sess_remove(n14_sess_t *sess);
void n14_sess_remove_all(n14_ue_t *ue);

n14_ue_t *n14_ue_find_by_imsi(uint8_t *imsi, int imsi_len);
n14_sess_t *n14_sess_find_by_id(uint64_t session_id);

#ifdef __cplusplus
}
#endif

#endif /* N14_CONTEXT_H */