#ifndef N14_TIMER_H
#define N14_TIMER_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    N14_TIMER_BASE = 0,

    N14_TIMER_FIRST,
    N14_TIMER_SECOND,
    // ... other timers specific to N14 interface ...

    MAX_NUM_OF_N14_TIMER,

} n14_timer_e;

// Configuration structure for N14 timers if needed
typedef struct n14_timer_cfg_s {
    bool have;
    int max_count;
    ogs_time_t duration;
} n14_timer_cfg_t;

// Function to retrieve the configuration for a specific timer
n14_timer_cfg_t *n14_timer_cfg(n14_timer_e id);

// Function to get the name of a timer
const char *n14_timer_get_name(n14_timer_e id);

// Timer handler functions
void n14_timer_first_handler(void *data);
void n14_timer_second_handler(void *data);
// ... other handler functions ...

#ifdef __cplusplus
}
#endif

#endif /* N14_TIMER_H */