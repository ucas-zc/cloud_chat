#include "cc_event_accept.h"

static cc_in32 cc_enable_accept_events(cc_cycle_t *cycle);
static cc_in32 cc_disable_accept_events(cc_cycle_t *cycle);

// 尝试获取连接锁
cc_in32 
cc_trylock_accept_mutex(cc_cycle_t *cycle)
{
    if (cc_shmtx_trylock(&cc_accept_mutex)) {
        if (cc_accept_mutex_held && cc_accept_events == 0) {
            return CC_OK;
        }

        if (cc_enable_accept_events(cycle) == CC_ERROR) {
            cc_shmtx_unlock(&cc_accept_mutex);
            return CC_ERROR;
        }

        cc_accept_events = 0;
        cc_accept_mutex_held = 1;
        return CC_OK;
    }

    if (cc_accept_mutex_held) {
        if (cc_disable_accept_events(cycle) == CC_ERROR) {
            return CC_ERROR;
        }

        cc_accept_mutex_held = 0;
    }

    return CC_OK;
}

// 启用连接事件
static cc_in32
cc_enable_accept_events(cc_cycle_t *cycle)
{
    return CC_OK;
}

// 禁用连接事件
static cc_in32
cc_disable_accept_events(cc_cycle_t *cycle)
{
    return CC_OK;
}
