#ifndef __CC_EPOLL_MODULE_H__
#define __CC_EPOLL_MODULE_H__

#include <sys/epoll.h>
#include "cc_core.h"
#include "cc_cycle.h"
#include "cc_module.h"
#include "cc_event.h"

// epoll初始化
cc_in32 cc_epoll_init(cc_cycle_t *cycle);

#endif
