#include "cc_module.h"

// 模块名称
cc_char *cc_modules_name[] = {
		"cc_epoll_module"
		"cc_event_module",
};

// 模块详细信息
cc_module_t *cc_modules[] = {
};

// 模块总数
cc_in32 cc_modules_n;

// 模块预初始化
cc_in32
cc_preinit_modules(void)
{
    cc_in32 i;
 
    for (i = 0; cc_modules[i]; ++i) {
        cc_modules[i]->index = i;
        cc_modules[i]->name = cc_modules_name[i];
    }

    cc_modules_n = i;

    return CC_OK;
}

// 模块内容拷贝
cc_in32
cc_cycle_modules(cc_cycle_t *cycle)
{
    // 异常判断
    if (cycle == NULL) {
        return CC_ERROR;
    }

    // 给module分配内存
    cycle->modules = (cc_module_t**)(intptr_t)cc_malloc((cc_modules_n + 1) 
					* sizeof(cc_module_t *));
	if (cycle->modules == NULL) {
        return CC_ERROR;
    }

    // 拷贝内容
    cc_memcpy(cycle->modules, cc_modules, cc_modules_n 
					* sizeof(cc_module_t *));
    cycle->modules_n = cc_modules_n;
	
    return CC_OK;	
}

// 模块初始化
cc_in32 
cc_init_modules(cc_cycle_t *cycle)
{
    cc_in32 i;

    // 异常判断
    if (cycle == NULL) {
        return CC_ERROR;
    }
   
    // 执行模块的初始化接口
    for (i = 0; cycle->modules[i]; ++i) {
        if (cycle->modules[i]->init_module) {
            if (cycle->modules[i]->init_module(cycle)) {
                return CC_ERROR;
            }
        }
    }

    return CC_OK;
}
