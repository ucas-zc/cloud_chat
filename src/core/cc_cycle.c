#include "cc_cycle.h"
#include "cc_module.h"

// cycle初始化
cc_cycle_t *
cc_init_cycle(cc_cycle_t *old_cycle)
{
    cc_cycle_t *cycle;

    // 异常判断
    if (old_cycle == NULL) {
        return NULL;
    }

    // 更新时间
    cc_update_time();

    // 分配内存
    cycle = (cc_cycle_t*)(intptr_t)cc_malloc(sizeof(cc_cycle_t));
    if (cycle == NULL) {
        return NULL;
    }

    cycle->log = old_cycle->log;
    cycle->old_cycle = old_cycle;

    // 拷贝模块信息
    if (CC_ERROR == cc_cycle_modules(cycle)) {
        cc_free(cycle);
        return NULL;
    }

    // 模块初始化
    if (CC_ERROR == cc_init_modules(cycle)) {
        /** fatal **/
        exit(0);
    }

    return cycle;
}
