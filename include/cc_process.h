#ifndef __CC_PROCESS_H__
#define __CC_PROCESS_H__

#include "cc_core.h"
#include "cc_event.h"
#include "cc_cycle.h"
#include "cc_mem.h"

#define CC_PROCESS_SINGLE 0         /** 单进程启动 **/
#define CC_PROCESS_MASTER 1         /** master进程启动 **/
#define CC_PROCESS_SIGNALER 2       /** 进程处理信号 **/

extern cc_in32 cc_argc;
extern cc_char **cc_argv;
extern cc_char **cc_os_argv;

typedef pid_t cc_pid_t;

typedef struct cc_process_s cc_process_t;

typedef void (*cc_spawn_proc_pt) (cc_cycle_t *cycle, void *data);

/** 进程信息 **/
struct cc_process_s
{
    pid_t pid;              // 进程id
    pid_t ppid;             // 进程id
    cc_log_t *log;          // 进程日志头节点
};

extern cc_in32 cc_process;
extern cc_pid_t cc_pid;
extern cc_pid_t cc_ppid;

/** 开启master进程 **/
cc_in32 cc_start_master_process(cc_cycle_t *cycle);

#endif
