#include "cc_epoll_module.h"

static cc_uin32 nevents;     /** epoll_wait返回最大事件数量 **/
static cc_in32 ep = -1;
static struct epoll_event *event_list;

static cc_event_module_t cc_epoll_module_ctx = {
    "epoll",
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        &cc_epoll_init
    }
};

cc_module_t cc_epoll_module = {
	CC_MODULE_EPOLL,
    "cc_epoll_module",
    &cc_epoll_module_ctx,
    NULL, // TODO
    CC_EVENT_MODULE,
    NULL
};

typedef struct 
{
    cc_uin32 events;         /* 表示epoll_wait函数返回的最大事件数 */
    cc_uin32 aio_requests;   /* 并发处理异步IO事件个数 */
}cc_epoll_conf_t;

// epoll模块初始化
cc_in32
cc_epoll_init(cc_cycle_t *cycle)
{
    cc_epoll_conf_t *conf;

    // 异常处理
    if (cycle == NULL) {
        return CC_ERROR;
    }

    // 获取epoll配置 TODO

    // 创建epoll事件表
    if (ep == -1) {
        // 考虑到浏览器模式会开启2个连接，因此 / 2
        ep = epoll_create(cycle->connection_n / 2);
        if (ep == -1) {
            return CC_ERROR;
        }
    }

    // 现有event_list个数小于配置项所指定的值conf->events，
    // 则先释放event_list
    if (nevents < conf->events) {
        if (event_list) {
            cc_free(event_list);
        }

        event_list = (struct epoll_event*)(intptr_t)cc_malloc(conf->events 
						* sizeof(struct epoll_event));
		if (event_list == NULL) {
            return CC_ERROR;
        }
    }

    nevents = conf->events;

    return CC_OK;
}
