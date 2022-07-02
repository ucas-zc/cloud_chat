#ifndef __CC_SHMTX_H__
#define __CC_SHMTX_H__

#include "cc_types.h"
#include "cc_files.h"

typedef struct 
{
    cc_fd_t fd;
    cc_char *name;
    cc_uin32 spin;
}cc_shmtx_t;

cc_in32 cc_shmtx_trylock(cc_shmtx_t *mtx);
cc_in32 cc_shmtx_unlock(cc_shmtx_t *mtx);

#endif
