#ifndef __CC_EVENT_TIMER_H__
#define __CC_EVENT_TIMER_H__

#include "cc_config.h"
#include "cc_core.h"
#include "cc_event.h"
#include "cc_rbtree.h"
#include "cc_times.h"

#define CC_TIMER_LAZY_DELAY 300

#define cc_add_timer cc_event_add_timer
#define cc_del_timer cc_event_del_timer

extern cc_rbtree_t cc_event_timer_rbtree;

// 定时器初始化
cc_in32 cc_event_timer_init(cc_log_t *log);
// 查找最小时间值
cc_msec_t cc_event_find_timer(void);
// 处理所有到期节点
cc_in32 cc_event_expire_timers(void);

// 删除事件
static cc_inline void 
cc_event_del_timer(cc_event_t *ev)
{
    cc_rbtree_delete(&cc_event_timer_rbtree, &ev->timer);

    ev->timer_set = 0;
}

// 添加事件
static cc_inline void
cc_event_add_timer(cc_event_t *ev, cc_msec_t timer)
{
    cc_msec_t      key;
    cc_msec_int_t  diff;

    key = cc_current_msec + timer;
    if (ev->timer_set) {
        diff = (cc_msec_int_t)(key - ev->timer.key);
        if (cc_abs(diff) < CC_TIMER_LAZY_DELAY) {
            return;
        }

        cc_del_timer(ev);
    }

    ev->timer.key = key;
    cc_event_rbtree_insert(&cc_event_timer_rbtree, &ev->timer);
    ev->timer_set = 1;
}

#endif
