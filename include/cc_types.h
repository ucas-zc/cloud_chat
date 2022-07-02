#ifndef __CC_TYPES_H__
#define __CC_TYPES_H__

#include <stdio.h>

// 变量类型
typedef char cc_char;
typedef unsigned char cc_uin8;
typedef unsigned short int cc_uin16;
typedef unsigned int cc_uin32;
typedef unsigned long long int cc_uin64;
typedef signed char cc_in8;
typedef signed short int cc_in16;
typedef signed int cc_in32;
typedef signed long long int cc_in64;
typedef int cc_fd_t;              /** 文件描述符 **/

// 返回值类型
#define CC_OK 0
#define CC_ERROR -1

#endif
