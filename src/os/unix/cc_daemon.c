#include "cc_daemon.h"
#include "cc_process.h"

// 守护进程
cc_in32 
cc_daemon(cc_log_t *log)
{
    cc_in32 fd;
 
    // fork子进程
    switch (fork()) {
        case -1:
            cc_log_add(CC_LOG_CORE, log, CC_LOG_ALERT, "fork() failed!\0");
            return CC_ERROR;
        case 0:
            break;
        default:
            exit(0);
    }

    // 更新父子进程id
    cc_ppid = cc_pid;
    cc_pid = getpid();

    if (setsid() == -1) {
        cc_log_add(CC_LOG_CORE, log, CC_LOG_ALERT, "setsid() failed!\0");
        return CC_ERROR;
    }

    umask(0);

    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        cc_log_add(CC_LOG_CORE, log, CC_LOG_ALERT, 
						"open(\"/dev/null\") failed!\0");
        return CC_ERROR;
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        cc_log_add(CC_LOG_CORE, log, CC_LOG_ALERT, "dup2(STDIN) failed!\0");
        return CC_ERROR;
    }

    if (dup2(fd, STDOUT_FILENO) == -1) {
        cc_log_add(CC_LOG_CORE, log, CC_LOG_ALERT, "dup2(STDOUT) failed!\0");
        return CC_ERROR;
    }

    if (fd > STDERR_FILENO) {
        if (close(fd) == -1) {
            cc_log_add(CC_LOG_CORE, log, CC_LOG_ALERT, 
						    "close(\"/dev/null\") failed!\0");
            return CC_ERROR;
        }
    }

    return CC_OK;
}
