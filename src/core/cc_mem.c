#include "cc_mem.h"

/** 分配内存 **/
void *
cc_malloc(size_t size)
{
    void *p;

    p = malloc(size);
    if (p) {
        cc_memzero(p, size);
    }

    return p;
}

/** 内存释放 **/
void cc_free(void *p)
{
    if (p != NULL) {
        free(p);
        p = NULL;
    }
}
