#ifndef __CC_STRING_H__
#define __CC_STRING_H__

#include <string.h>
#include "cc_types.h"

typedef struct {
    size_t      len;
    cc_char     *data;
} cc_str_t;

/** 计算字符串长度 **/
#define cc_strlen(s) strlen((const char*)s)
/** 浅拷贝 **/
#define cc_memcpy(dst, src, n) memcpy(dst, src, n)
/** 浅拷贝，返回字符串末尾地址 **/
#define cc_cpymem(dst, src, n) (((cc_char*)memcpy(dst, src, n)) + (n))
#define cc_strcmp(s1, s2) strcmp((const char*)s1, (const char*)s2)
#define cc_memmove(dst, src, n)   (void) memmove(dst, src, n)

/** 计算字符串长度 **/
size_t cc_strnlen(cc_char *p, size_t n);
/** 拷贝字符串 **/
cc_char *cc_cpystrn(cc_char *dst, cc_char *src, size_t n);

#endif
