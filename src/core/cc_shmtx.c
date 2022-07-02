#include "cc_shmtx.h"

// 上锁
cc_in32
cc_shmtx_trylock(cc_shmtx_t *mtx)
{
    return cc_trylock_fd(mtx->fd) == 0 ? 1 : 0;
}

// 解锁
cc_in32
cc_shmtx_unlock(cc_shmtx_t *mtx)
{
    return cc_unlock_fd(mtx->fd) == 0 ? 1 : 0;
}
