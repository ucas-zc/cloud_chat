#ifndef __CC_ATOMIC_H__
#define __CC_ATOMIC_H__

#include "cc_config.h"

typedef cc_in32 cc_atomic_int_t;
typedef cc_uin32 cc_atomic_uint_t;
typedef volatile cc_atomic_uint_t cc_atomic_t;

static cc_inline cc_atomic_uint_t
cc_atomic_cmp_set(cc_atomic_t *lock, cc_atomic_uint_t old,
    cc_atomic_uint_t set)
{
    if (*lock == old) {
        *lock = set;
        return 1;
    }

    return 0;
}

#define cc_trylock(lock)  (*(lock) == 0 && cc_atomic_cmp_set(lock, 0, 1))
#define cc_unlock(lock)    *(lock) = 0

#endif
