#include "n14-handler.h"
#include "n14-build.h"

bool n14_handle_message_transfer(smf_sess_t *sess, int state, ogs_sbi_message_t *recvmsg) {
    int status;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(sess);
    ogs_assert(recvmsg);

    ogs_sbi_message_t sendmsg;
    OpenAPI_n1_n2_message_transfer_rsp_data_t N1N2MessageTransferRspData;

    memset(&sendmsg, 0, sizeof(sendmsg));
    memset(&N1N2MessageTransferRspData, 0, sizeof(N1N2MessageTransferRspData));

    N1N2MessageTransferRspData.cause = OpenAPI_n1_n2_message_transfer_cause_N1_N2_TRANSFER_INITIATED;
    sendmsg.N1N2MessageTransferRspData = &N1N2MessageTransferRspData;

    status = OGS_SBI_HTTP_STATUS_OK;

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(sess->stream, response));

    return true;
}

bool n14_handle_message_transfer_failure_notify(ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg) {
    int status = OGS_SBI_HTTP_STATUS_NO_CONTENT;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    ogs_sbi_message_t sendmsg;
    memset(&sendmsg, 0, sizeof(sendmsg));

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return true;
}