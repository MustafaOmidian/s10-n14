/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef N14_BUILD_H
#define N14_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct n14_message_transfer_param_s {
#define N14_MESSAGE_TRANSFER_NO_STATE             0
#define N14_UE_REQUESTED_SESSION_ESTABLISHMENT    1
#define N14_NETWORK_REQUESTED_SESSION_MODIFICATION 2
#define N14_NETWORK_REQUESTED_QOS_FLOW_MODIFICATION 3
#define N14_NETWORK_REQUESTED_SESSION_RELEASE     4
#define N14_NETWORK_TRIGGERED_SERVICE_REQUEST     5
#define N14_ERROR_INDICATION_RECEIVED_FROM_5G_AN  6
    int state;

    ogs_pkbuf_t *n1smbuf;
    ogs_pkbuf_t *n2smbuf;

    bool n1n2_failure_txf_notif_uri;
    bool skip_ind;
} n14_message_transfer_param_t;

ogs_sbi_request_t *n14_comm_build_message_transfer(
        smf_sess_t *sess, n14_message_transfer_param_t *param);

ogs_sbi_request_t *n14_callback_build_context_status(
        smf_sess_t *sess, void *data);

#ifdef __cplusplus
}
#endif

#endif /* N14_BUILD_H */