#include "cc_string.h"

/** 拷贝字符串 **/
cc_char *
cc_cpystrn(cc_char *dst, cc_char *src, size_t n)
{
    if (n == 0) {
        return dst;
    }

    while (--n) {
        *dst = *src;
        
        if (*dst == '\0') {
            return dst;
        }

        dst++;
        src++;
    }

    *dst = '\0';

    return dst;
}
