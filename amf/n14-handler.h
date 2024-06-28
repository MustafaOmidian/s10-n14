#ifndef N14_HANDLER_H
#define N14_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

bool n14_handle_message_transfer(smf_sess_t *sess, int state, ogs_sbi_message_t *recvmsg);
bool n14_handle_message_transfer_failure_notify(ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg);

#ifdef __cplusplus
}
#endif

#endif /* N14_HANDLER_H */