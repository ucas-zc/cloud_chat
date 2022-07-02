#ifndef __CC_FILE_H__
#define __CC_FILE_H__

#include "cc_core.h"
#include "cc_files.h"
#include "cc_log.h"

struct cc_file_s 
{
    cc_fd_t fd;
    cc_str_t name;
    cc_file_info_t info;
    off_t offset;
    cc_log_t *log;
};

#endif
