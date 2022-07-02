#ifndef __CC_TIMES_H__
#define __CC_TIMES_H__

#include "cc_time.h"
#include "cc_string.h"
#include "cc_atomic.h"

typedef struct {
    time_t      sec;
    cc_uin32  msec;
    cc_in32   gmtoff;
} cc_time_t;

extern volatile cc_msec_t cc_current_msec;

void cc_init_time(void);
void cc_update_time(void);
void cc_gmtime(time_t t, cc_tm_t *tp);

#endif
