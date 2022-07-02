#include "cc_files.h"

// 上锁
cc_in32
cc_trylock_fd(cc_fd_t fd)
{
    struct flock fl;
    
    cc_memzero(&fl, sizeof(struct flock));
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLK, &fl) == -1) {
        return -1;
    }

    return 0;
}

// 解锁
cc_in32
cc_unlock_fd(cc_fd_t fd)
{
    struct flock fl;
    
    cc_memzero(&fl, sizeof(struct flock));
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;

    if (fcntl(fd, F_SETLKW, &fl) == -1) {
        return -1;
    }

    return 0;
}
