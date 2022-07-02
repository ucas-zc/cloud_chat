#ifndef __CC_ARRAY_H__
#define __CC_ARRAY_H__

#include "cc_types.h"

typedef struct cc_array_s
{
    void        *elts;
    cc_uin32    nelts;
}cc_array_t;

void *cc_array_push(cc_array_t *a);

#endif
