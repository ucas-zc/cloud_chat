#ifndef __CC_BUF_H__
#define __CC_BUF_H__

#include "cc_types.h"

typedef struct cc_buf_s cc_buf_t;

struct cc_buf_s
{
    cc_char *pos;                     // 当前将要扫描字符位置                 
    cc_char *last;                    // 为处理字符的尾部
    cc_char *start;                   // buffer的开始
    cc_char *end;                     // buffer的终止

    unsigned temporary:1;             // buffer内容可改变
};

#endif
