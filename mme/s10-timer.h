/*
 * This would be the header for the S10 interface timers.
 * It would follow the same licensing and structure as the other timer headers.
 */

#ifndef S10_TIMER_H
#define S10_TIMER_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Timer enumeration for S10 interface */
typedef enum {
    S10_TIMER_BASE = 0,

    S10_TIMER_CONTEXT_REQUEST,
    S10_TIMER_CONTEXT_RESPONSE,
    S10_TIMER_FORWARD_RELOCATION_REQUEST,
    S10_TIMER_FORWARD_RELOCATION_RESPONSE,

    MAX_NUM_OF_S10_TIMER,

} s10_timer_e;

/* Configuration structure for S10 timers */
typedef struct s10_timer_cfg_s {
    bool have;
    int max_count;
    ogs_time_t duration;
} s10_timer_cfg_t;

/* Function to retrieve the configuration for a specific timer */
s10_timer_cfg_t *s10_timer_cfg(s10_timer_e id);

/* Function to get the name of a timer */
const char *s10_timer_get_name(s10_timer_e id);

/* Timer handler functions */
void s10_timer_context_request_handler(void *data);
void s10_timer_context_response_handler(void *data);
void s10_timer_forward_relocation_request_handler(void *data);
void s10_timer_forward_relocation_response_handler(void *data);
// ... other handler functions ...

#ifdef __cplusplus
}
#endif

#endif /* S10_TIMER_H */