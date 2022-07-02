#ifndef __CC_DAEMON_H__
#define __CC_DAEMON_H__

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cc_types.h"
#include "cc_log.h"

/** 守护进程 **/
cc_in32 cc_daemon(cc_log_t *log);

#endif
