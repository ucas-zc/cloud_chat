#ifndef __CC_TIME_H__
#define __CC_TIME_H__

#include <sys/time.h>
#include <time.h>
#include "cc_rbtree.h"

typedef cc_rbtree_key_t cc_msec_t;
typedef cc_rbtree_key_int_t  cc_msec_int_t;

typedef struct tm cc_tm_t;

#define cc_tm_sec    tm_sec
#define cc_tm_min    tm_min
#define cc_tm_hour   tm_hour
#define cc_tm_mday   tm_mday
#define cc_tm_mon    tm_mon
#define cc_tm_year   tm_year
#define cc_tm_wday   tm_wday
#define cc_tm_isdst  tm_isdst

#define cc_tm_sec_t    int
#define cc_tm_min_t    int
#define cc_tm_hour_t   int
#define cc_tm_mday_t   int
#define cc_tm_mon_t    int
#define cc_tm_year_t   int
#define cc_tm_wday_t   int

#define cc_gettimeofday(tp) (void)gettimeofday(tp, NULL);

#define cc_timezone(isdst) (- (isdst ? timezone + 3600 : timezone) / 60)

void cc_localtime(time_t s, cc_tm_t *tm);

#endif
