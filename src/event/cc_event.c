#include "cc_event.h"
#include "cc_event_timer.h"
#include "cc_event_accept.h"

// 时间精度
cc_uin32 cc_timer_resolution;
// 连接锁
cc_uin32 cc_use_accept_mutex;
cc_shmtx_t cc_accept_mutex;
cc_uin32 cc_accept_mutex_held;
cc_uin32 cc_accept_events;
cc_msec_t cc_accept_mutex_delay;
cc_in32 cc_accept_disabled;

// 处理事件与定时器
void
cc_process_events_and_timers(cc_cycle_t *cycle)
{
    cc_uin32 flags;
    cc_msec_t timer, delta;

    // 异常判断
    if (cycle == NULL) {
        return;
    }

    // 时间精度
    if (cc_timer_resolution) {
        timer = CC_TIMER_INFINITE;
        flags = 0;
    } else {
        // 找到当前红黑树当中的最小的事件
        timer = cc_event_find_timer();
        flags = CC_UPDATE_TIME;
    }

    // 获取连接锁，处理“惊群事件”
    if (cc_use_accept_mutex) {
        if (cc_accept_disabled > 0) {
            cc_accept_disabled--;
        } else {
            if (cc_trylock_accept_mutex(cycle) == CC_ERROR) {
                return;
            }

            if (cc_accept_mutex_held) {
                flags |= CC_POST_EVENTS;
            } else {
                if (flags == CC_TIMER_INFINITE 
						|| timer > cc_accept_mutex_delay)
                {
                    timer = cc_accept_mutex_delay;
                }
            }
        }
    }

    return;
}
