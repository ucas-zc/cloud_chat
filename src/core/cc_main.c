#ifndef _GTEST_

#include "cc_daemon.h"
#include "cc_cycle.h"
#include "cc_process.h"
#include "cloud_chat.h"

/** 全局变量 **/
static cc_in32 cc_show_version;
static cc_in32 cc_show_help;
static cc_char *cc_signal;

/** 函数声明 **/
static cc_in32 cc_get_options(cc_in32 argc, cc_char **argv);
static void cc_show_version_info();
static cc_in32 cc_save_args(cc_in32 argc, cc_char **argv, cc_log_t *log);

/** 主程序入口 **/
cc_in32
main(cc_in32 argc, cc_char **argv) 
{
    cc_log_t *log;
    cc_cycle_t init_cycle, *cycle;

    // 获取启动参数
    cc_get_options(argc, argv);

    // 显示软件版本号与参数
    if (cc_show_version == 1) {
        cc_show_version_info();
    }

    // 初始化时钟
    cc_init_time();

    // 获取父子进程id
    cc_pid = getpid();
    cc_ppid = getppid();

    // 初始化日志
    log = cc_log_init();
    if (log == NULL) {
        cc_write_stderr("init log failed!" CC_LINEFEED);
        return CC_ERROR;
    }

    // 初始化init_cycle
    cc_memzero(&init_cycle, sizeof(init_cycle));
    init_cycle.log = log;

    // 保存启动参数
    if (CC_ERROR == cc_save_args(argc, argv, init_cycle.log)) {
        cc_write_stderr("save args failed!" CC_LINEFEED);
        return CC_ERROR;
    }

    // 模块预初始化
    if (CC_ERROR == cc_preinit_modules()) {
        cc_write_stderr("preinit modules failed!" CC_LINEFEED);
        return CC_ERROR;
    }

    // 初始化cc_cycle_t
    cycle = (cc_cycle_t*)(intptr_t)cc_init_cycle(&init_cycle);
    if (cycle == NULL) {
        cc_write_stderr("init cycle failed!" CC_LINEFEED);
        return CC_ERROR;
    }

    // 开启守护进程
    if (CC_ERROR == cc_daemon(init_cycle.log)) {
        cc_write_stderr("daemon failed!" CC_LINEFEED);
        return CC_ERROR;
    }

    if (CC_ERROR == cc_start_master_process(&init_cycle)) {
        cc_write_stderr("start master process failed!" CC_LINEFEED);
        return CC_ERROR;
    }
    // worker进程工作
   // if (cc_process == CC_PROCESS_SINGLE) {
   //     cc_start_single_process(log);
   // } else {
   //     cc_start_master_process(log);
   // }

    return CC_OK;
}

/** 获取启动参数 **/
static cc_in32 
cc_get_options(cc_in32 argc, cc_char **argv)
{
    cc_in32 i;
    cc_char *options;

    // 循环获取参数
    for (i = 1; i < argc; ++i) {
        options = argv[i];
        // 参数不以'-'开头，报错
        if (*options++ != '-') {
            return CC_ERROR;
        }

        while (*options) {
            switch (*options++) {
            case '?':
            case 'h':
                cc_show_version = 1;
                cc_show_help = 1;
                break;
            // 显示版本号
            case 'v':
                cc_show_version = 1;
                break;
            case 's':
                if (*options) {
                    cc_signal = (cc_char*)options;
                } else if (argv[++i]) {
                    cc_signal = argv[i];
                } else {
                    cc_write_stderr("options \"-s\" requeires parameter!\n");
                    return CC_ERROR;
                }

                if (cc_strcmp(cc_signal, "stop") == 0
						|| cc_strcmp(cc_signal, "quit") == 0
						|| cc_strcmp(cc_signal, "reload") == 0)
                {
                    cc_process = CC_PROCESS_SIGNALER;
                    break;
                }
                
                cc_write_stderr("invalid options!\n");
                break;
            default:
                cc_write_stderr("invalid options!\n");
                break;
            }
        }
    }

    return CC_OK;
}

/** 显示软件版本号与参数 **/
static void 
cc_show_version_info()
{
    cc_write_stderr("cloud chat version: " CC_VERSION CC_LINEFEED);

    if (cc_show_help) {
        cc_write_stderr(
						"Usage: cloud_chat [-?hv]" CC_LINEFEED
						CC_LINEFEED
						"Options:" CC_LINEFEED
						"  -? -h: this help" CC_LINEFEED
						"  -v: show version and exit" CC_LINEFEED
					   );
    }
}

/** 保存启动参数 **/
static cc_in32 
cc_save_args(cc_in32 argc, cc_char **argv, cc_log_t *log)
{
    size_t len;
    cc_in32 i;

    // 保存系统参数个数与系统参数内容
    cc_os_argv = argv;
    cc_argc = argc;

    cc_argv = (cc_char**)cc_malloc(argc * sizeof(char *));
    if (cc_argv == NULL) {
        cc_log_add(CC_LOG_CORE, log, CC_LOG_ALLOC, 
						"cc_argv alloc failed!");
        return CC_ERROR;
    }

    // 拷贝启动参数
    for (i = 0; i < argc; ++i) {
        len = cc_strlen(argv[i]) + 1;

        cc_argv[i] = (cc_char*)cc_malloc(len);
        if (cc_argv[i] == NULL) {
            cc_log_add(CC_LOG_CORE, log, CC_LOG_ALLOC, 
			    			"cc_argv[i] alloc failed!");
            return CC_ERROR;
        }

        (void)cc_cpystrn(cc_argv[i], argv[i], len);
    }

    cc_argv[i] = NULL;

    return CC_OK;
}

#endif
