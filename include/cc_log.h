#ifndef __CC_LOG_H__
#define __CC_LOG_H__

#include "cc_types.h"
#include "cc_files.h"
#include "cc_string.h"
#include <unistd.h>
#include <stdlib.h>

// 日志级别
#define CC_LOG_CORE 0
#define CC_LOG_ALLOC 1
#define CC_LOG_MUTEX 2
#define CC_LOG_COMMUNICATION 3
// 日志级别对应字典
static const cc_char * log_level[] = {
    "core", "alloc", "mutex", "communication"
};

// 错误级别
#define CC_LOG_ALERT 0
#define CC_LOG_ERROR 1
#define CC_LOG_WARN 2
#define CC_LOG_NOTICE 3
#define CC_LOG_INFO 4
// 错误级别对应字典
static const cc_char * error_level[] = {
    "alert", "error", "warn", "notice", "info"
};

typedef struct cc_log_s cc_log_t;
typedef struct cc_log_node cc_log_n;

struct cc_log_s {
    FILE *fp;              // 日志文件
    cc_log_n *next;        // 指向下一个节点
    cc_log_n *tail;        // 指向尾节点
};

struct cc_log_node
{
    cc_in32 log_level;     // 日志等级
    cc_in32 error_level;   // 错误等级
    cc_char *data;         // 日志数据

    cc_log_n *next;        // 指向下一个节点
};

/** 日志初始化 **/
cc_log_t *cc_log_init(void);

/** 写日志 **/
cc_in32 cc_log_add(cc_in32 log_level, cc_log_t *log, 
				cc_in32 log_errno, const cc_char *data);

/** 输出日志 **/
cc_in32 cc_log_out(cc_log_t *log);

/** 输出错误信息 **/
static cc_inline void
cc_write_stderr(cc_char *text)
{
    (void)cc_write_fd(cc_stderr, text, cc_strlen(text));
}

#endif
