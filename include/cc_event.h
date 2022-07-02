#ifndef __CC_EVENT_H__
#define __CC_EVENT_H__

#include "cc_core.h"
#include "cc_rbtree.h"
#include "cc_log.h"
#include "cc_time.h"
#include "cc_shmtx.h"

#define CC_EVENT_MODULE 0x544E5645  /* "EVNT" ascii */

#define CC_TIMER_INFINITE (cc_msec_t)-1
#define CC_UPDATE_TIME 1
#define CC_POST_EVENTS 2

extern cc_uin32 cc_use_accept_mutex;
extern cc_shmtx_t cc_accept_mutex;
extern cc_uin32 cc_accept_mutex_held;
extern cc_uin32 cc_accept_events;
extern cc_msec_t cc_accept_mutex_delay;
extern cc_in32 cc_accept_disabled;

struct cc_event_s
{
    void *data;                             // 事件处理函数入参
    unsigned timer_set:1;                   // 定时器设置标记，1表示已设置，0表示未设置
    cc_event_handler_pt handler;            // 事件处理函数
    cc_log_t *log;                          // 日志
    cc_rbtree_node_t timer;                // 定时器节点
};

typedef struct 
{
    cc_in32 (*add)(cc_event_t *ev);               /** 添加一个event **/
    cc_in32 (*del)(cc_event_t *ev);               /** 删除一个event **/
    cc_in32 (*enable)(cc_event_t *ev);            /** 启用一个event **/
    cc_in32 (*disable)(cc_event_t *ev);           /** 禁用一个event **/
    cc_in32 (*add_conn)(cc_connection_t *conn);   /** 添加一个连接  **/
    cc_in32 (*del_conn)(cc_connection_t *conn);   /** 删除一个连接  **/
    cc_in32 (*process_event)(cc_cycle_t *cycle);  /** 处理当前事件  **/
    cc_in32 (*init)(cc_cycle_t *cycle);           /** 初始化事件    **/
}cc_event_action_t;

typedef struct 
{
    cc_char *name;                          // 名称
    cc_event_action_t actions;              // IO模型的各种接口
}cc_event_module_t;

// 处理事件与定时器
void cc_process_events_and_timers(cc_cycle_t *cycle);

#endif
