#include "cc_time.h"

void cc_localtime(time_t s, cc_tm_t *tm)
{
    cc_tm_t  *t;

    t = localtime(&s);
    *tm = *t;
    tm->cc_tm_mon++;
    tm->cc_tm_year += 1900;
}
