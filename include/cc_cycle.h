#ifndef __CC_CYCLE_H__
#define __CC_CYCLE_H__

#include "cc_core.h"
#include "cc_log.h"
#include "cc_times.h"

struct cc_cycle_s
{
    void ****conf_ctx;                        // 保存着所有模块存储配置项的结构体的指针，
                                              // 它首先是一个数组，每个数组成员又是一个指针，
                                              // 这个指针指向另一个存储着指针的数组
    cc_log_t *log;                            // 日志指针
    cc_module_t **modules;                    // 所有模块信息
    cc_in32 modules_n;                        // 模块总数
    cc_connection_t *free_connection;         // 可用连接池
    cc_event_t *read_events;                  // 读事件
    cc_event_t *write_events;                 // 写事件
    cc_in32 connection_n;                     // 当前进程可连接总数
    cc_cycle_t *old_cycle;                    // 旧的cc_cycle_t
};

cc_cycle_t * cc_init_cycle(cc_cycle_t *old_cycle);

#endif
