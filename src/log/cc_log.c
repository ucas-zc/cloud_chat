#include "cc_log.h"

static cc_log_t cc_log;

/** 日志初始化 **/
cc_log_t *
cc_log_init(void) 
{
    // 打开日志文件
    cc_log.fp = fopen(CC_LOG_PATH, "aw+");
    if (cc_log.fp == NULL) {
        return NULL;
    }

    cc_log.next = NULL;

    return &cc_log;
}

/** 输出错误级别日志 **/
cc_in32
cc_log_add(cc_in32 log_level, cc_log_t *log, 
				cc_in32 log_errno, const cc_char *data)
{
    cc_log_n *p;
    cc_in32 _data_len;

    // 异常判断 
    if (log == NULL) {
        return CC_ERROR;
    }

    // 分配内存
    if (!(p = (cc_log_n*)(intptr_t)cc_malloc(sizeof(cc_log_n))))
    {
        return CC_ERROR;
    }

    // 日志节点赋值
    p->log_level = log_level;
    p->error_level = log_errno;

    // 拷贝日志信息
    if (data != NULL) {
        _data_len = strnlen(data, 128);
        p->data = (cc_char*)(intptr_t)cc_malloc(_data_len);

        if (p->data != NULL) {
            strncpy(p->data, data, _data_len);
        }
    } else {
        p->data = NULL;
    }

    // 插入日志链表中
    if (log->tail != NULL) {
        log->tail->next = p;
    } else {
        log->next = p;
    }
    p->next = NULL;
    log->tail = p;

    return CC_OK;
}

/** 输出日志到文件 **/
cc_in32 cc_log_out(cc_log_t *log)
{
    cc_log_n *p;
    cc_log_n *tmp;
    cc_char log_stream[128];

    // 异常判断
    if (log == NULL) {
        return CC_ERROR;
    }

    for (p = log->next; p != NULL; p = p->next) {
        // 备份当前节点地址
        tmp = p;

        // 输出日志信息
		if (log->fp == NULL) {
            return CC_ERROR;
        }

        // 拼接日志体
		snprintf(log_stream, 128, "process: pid[%d], ppid[%d], log_level[%s], error_level[%s], msg[%s]\n\0", 
						getpid(), getppid(), log_level[p->log_level], 
						error_level[p->error_level], p->data);
        fwrite(log_stream, cc_strlen(log_stream), sizeof(char), log->fp);
        fseek(log->fp, 0, SEEK_SET);

        // 删除节点
        log->next = p->next;

        if (tmp) {
            cc_free(tmp);
        }
    }

    return CC_OK;
}
