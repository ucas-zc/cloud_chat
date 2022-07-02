#ifndef __CC_MEM_H__
#define __CC_MEM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cc_memzero(buf, n) (void)memset(buf, 0, n)
#define cc_memset(buf, c, n) (void)memset(buf, c, n)

/** 分配内存 **/
void *cc_malloc(size_t size);

/** 内存释放 **/
void cc_free(void *p);

#endif
