#ifndef __CC_FILES_H__
#define __CC_FILES_H__

#include "cc_core.h"
#include "cc_config.h"
#include "cc_mem.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define CC_LINEFEED "\x0a"

#define cc_stderr STDERR_FILENO

#define CC_INVALID_FILE -1
#define CC_FILE_ERROR -1

#define CC_FILE_RDONLY          O_RDONLY
#define CC_FILE_WRONLY          O_WRONLY
#define CC_FILE_OPEN            0

#define cc_close_file           close
// 获取文件状态
#define cc_fd_info(fd, sb)      fstat(fd, sb)
// 打开一个文件
#define cc_open_file(name, mode, create, access)   \
    open((const char *) name, mode|create, access)

// 文件实际大小
#define cc_file_size(sb)        (sb)->st_size

typedef struct stat cc_file_info_t;

cc_in32 cc_trylock_fd(cc_fd_t fd);
cc_in32 cc_unlock_fd(cc_fd_t fd);
ssize_t cc_read_file(cc_file_t *file, cc_char *buf, 
    size_t size, off_t offset);

static cc_inline ssize_t
cc_write_fd(cc_fd_t fd, void *buf, size_t len)
{
    return write(fd, buf, len);
}

#endif