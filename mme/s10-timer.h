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

/* forward declaration */
typedef enum {
    S10_TIMER_BASE = 0,

    S10_TIMER_YOUR_FIRST_TIMER_NAME,
    S10_TIMER_YOUR_SECOND_TIMER_NAME,
    // ... other timers specific to S10 interface ...

    MAX_NUM_OF_S10_TIMER,

} s10_timer_e;

// Configuration structure for S10 timers if needed
typedef struct s10_timer_cfg_s {
    bool have;
    int max_count;
    ogs_time_t duration;
} s10_timer_cfg_t;

// Function to retrieve the configuration for a specific timer
s10_timer_cfg_t *s10_timer_cfg(s10_timer_e id);

// Function to get the name of a timer
const char *s10_timer_get_name(s10_timer_e id);

// Timer handler functions
void s10_timer_your_first_timer_handler(void *data);
void s10_timer_your_second_timer_handler(void *data);
// ... other handler functions ...

#ifdef __cplusplus
}
#endif

#endif /* S10_TIMER_H */