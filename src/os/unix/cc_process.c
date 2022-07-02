#include "cc_process.h"
#include <time.h>

// master 进程title
static cc_char master_process[] = "cloud_chat: master process";
// worker 进程title
static cc_char worker_process[] = "cloud_chat: worker process";

// 启动参数个数
cc_in32 cc_argc;
// 启动参数内容
cc_char **cc_argv;
// 启动参数内容（系统）
cc_char **cc_os_argv;

// 进程启动模式，单进程 or master+worker
cc_in32 cc_process;
// 子进程id
cc_pid_t cc_pid;
// 父进程id
cc_pid_t cc_ppid;

static cc_in32 cc_start_worker_processes(cc_cycle_t *cycle, cc_in32 num);
static cc_in32 cc_setproctitle(cc_char *title);
static cc_in32 cc_spawn_process(cc_cycle_t *cycle, cc_spawn_proc_pt proc, 
				void *data, char *name);
static void cc_worker_process_cycle(cc_cycle_t *cycle, void *data);

// master进程
cc_in32 
cc_start_master_process(cc_cycle_t *cycle)
{
    // 设置进程title
    cc_setproctitle(master_process);

    // 启动worker进程
    cc_start_worker_processes(cycle, 10);

    for ( ;; ) {
        // TODO
    }

    return CC_OK;
}

// worker进程
static cc_in32 
cc_start_worker_processes(cc_cycle_t *cycle, cc_in32 num)
{
    cc_in32 i;

    cc_log_add(CC_LOG_CORE, cycle->log, 
					CC_LOG_NOTICE, "start worker process");

    // 开启num个worker进程
    for (i = 0; i < num; ++i) {
        cc_spawn_process(cycle, cc_worker_process_cycle, 
						(void*)(intptr_t)i, worker_process);
    }

    return CC_OK;
}

// 启动进程
static cc_in32
cc_spawn_process(cc_cycle_t *cycle, cc_spawn_proc_pt proc, 
				void *data, char *name)
{
    cc_pid_t pid;

    pid = fork();
    switch (pid) {
        case -1:
            return CC_ERROR;
        case 0:
            cc_ppid = cc_pid;
            cc_pid = getpid();
            proc(cycle, data);
            break;
        default:
            break;
    }

    return pid;
}

// worker进程工作内容
static void
cc_worker_process_cycle(cc_cycle_t *cycle, void *data)
{
    // 设置worker进程title
    cc_setproctitle(worker_process);
    
    for ( ;; ) {
        cc_log_add(CC_LOG_CORE, cycle->log, 
						CC_LOG_NOTICE, "worker cycle");

        // 处理事件
        cc_process_events_and_timers(cycle);

        // 处理各类信号 TODO
        sleep(5);
        cc_log_out(cycle->log);
    }
}

// single启动
cc_in32 
cc_start_single_process(cc_log_t *log)
{
}

// 设置进程名称
static cc_in32
cc_setproctitle(cc_char *title)
{
    // 异常判断
    if (title == NULL) {
        return CC_ERROR;
    }
 
    cc_os_argv[1] = NULL;

    (void)cc_cpystrn(cc_os_argv[0], title, cc_strlen(title) + 1);

    return CC_OK;
}
