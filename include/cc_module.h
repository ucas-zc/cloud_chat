#ifndef __CC_MODULE_H__
#define __CC_MODULE_H__

#include "cc_core.h"
#include "cc_cycle.h"

#define CC_MODULE_EPOLL 0

extern cc_char *cc_modules_name[];
extern cc_module_t *cc_modules[];
extern cc_in32 cc_modules_n;

// 模块信息结构
struct cc_module_s
{
    // 模块id
    cc_in32 index;
    // 模块名称
    cc_char *name;
    // 模块上下文
    void *ctx;
    // 模块类型
    cc_in32 type;
    // 初始化模块接口
    cc_in32 (*init_module)(cc_cycle_t *cycle);
};

// 模块内容初始化
cc_in32 cc_preinit_modules(void);

// 模块内容拷贝
cc_in32 cc_cycle_modules(cc_cycle_t *cycle);

// 模块初始化
cc_in32 cc_init_modules(cc_cycle_t *cycle);

#endif
