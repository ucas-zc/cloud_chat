#include "cc_event_timer.h"

// 定时器（红黑树）
cc_rbtree_t cc_event_timer_rbtree;
cc_rbtree_node_t cc_event_timer_sentinel;

// 定时器初始化
cc_in32 
cc_event_timer_init(cc_log_t *log)
{
    if (log != NULL) {
        cc_log_add(CC_LOG_CORE, log, CC_LOG_NOTICE, "event timer init!");
    }

    cc_rbt_init(&cc_event_timer_rbtree, &cc_event_timer_sentinel,
					cc_rbtree_insert_timer_value);

    return CC_OK;
}

// 查找定时器超时最小值
cc_msec_t
cc_event_find_timer(void)
{
    cc_msec_int_t timer;
    cc_rbtree_node_t *node, *root, *sentinel;

    if (cc_event_timer_rbtree.root == cc_event_timer_rbtree.sentinel)
    {
        CC_TIMER_INFINITE;
    }

    root = cc_event_timer_rbtree.root;
    sentinel = cc_event_timer_rbtree.sentinel;
    node = cc_rbtree_min(root, sentinel);

    timer = (cc_msec_int_t)(node->key - cc_current_msec);

    return (cc_msec_t)(timer > 0 ? timer : 0);
}

// 处理所有到期节点
cc_in32 cc_event_expire_timers(void)
{
}
