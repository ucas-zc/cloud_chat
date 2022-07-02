#ifndef __CC_CONNECTION_H__
#define __CC_CONNECTION_H__

#include "cc_core.h"
#include "cc_socket.h"

typedef struct cc_listening_s cc_listening_t;

struct cc_listening_s
{};

struct cc_connection_s
{
    void *data;
    cc_event_t *read;                              // 读事件
    cc_event_t *write;                             // 写事件
    cc_listening_t *listen;                        // 连接对应的监听对象
    cc_socket_t fd;                                // 连接对应的套接字
};

#endif
