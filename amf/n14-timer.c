#include "n14-timer.h"
#include "ogs-logger.h"

const char *n14_timer_get_name(n14_timer_e id) {
    switch (id) {
        case N14_TIMER_FIRST:
            return "N14_TIMER_FIRST";
        case N14_TIMER_SECOND:
            return "N14_TIMER_SECOND";
        // Add cases for other timers
        default:
            return "UNKNOWN_TIMER";
    }
}

static void timer_send_event(int timer_id, void *data) {
    int rv;
    n14_event_t *e = NULL;
    ogs_assert(data);

    e = n14_event_new(N14_EVT_TIMER);
    e->timer_id = timer_id;
    e->data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        n14_event_free(e);
    }
}

void n14_timer_first_handler(void *data) {
    timer_send_event(N14_TIMER_FIRST, data);
}

void n14_timer_second_handler(void *data) {
    timer_send_event(N14_TIMER_SECOND, data);
}

n14_timer_cfg_t *n14_timer_cfg(n14_timer_e id) {
    static n14_timer_cfg_t timer_cfg[MAX_NUM_OF_N14_TIMER];
    // Code to configure timers based on the ID
    return &timer_cfg[id];
}