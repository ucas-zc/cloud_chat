#ifndef __CC_CONF_FILE_H__
#define __CC_CONF_FILE_H__

#include "cc_core.h"
#include "cc_string.h"
#include "cc_file.h"
#include "cc_buf.h"
#include "cc_log.h"
#include "cc_array.h"
#include "cc_cycle.h"
#include "cc_module.h"

#define CC_CONF_OK NULL
#define CC_CONF_ERROR (void*) -1

#define CC_CONF_MODULE      0x464E4F43  /* "CONF" */

#define CC_CONF_BLOCK       0x00000100
#define CC_CONF_FLAG        0x00000200
#define CC_CONF_ANY         0x00000400
#define CC_CONF_1MORE       0x00000800
#define CC_CONF_2MORE       0x00001000

#define CC_DIRECT_CONF      0x00010000
#define CC_MAIN_CONF        0x01000000

#define CC_CONF_NOARGS      0x00000001
#define CC_CONF_TAKE1       0x00000002
#define CC_CONF_TAKE2       0x00000004
#define CC_CONF_TAKE3       0x00000008
#define CC_CONF_TAKE4       0x00000010
#define CC_CONF_TAKE5       0x00000020
#define CC_CONF_TAKE6       0x00000040
#define CC_CONF_TAKE7       0x00000080

#define CC_CONF_MAX_ARGS    8

#define CC_CONF_BLOCK_START 1
#define CC_CONF_BLOCK_DONE  2
#define CC_CONF_FILE_DONE   3

typedef char *(*cc_conf_handler_pt)(cc_conf_t *cf,
    cc_command_t *dummy, void *conf);

struct cc_command_s
{
    cc_str_t name;
    cc_uin32 type;
    char *(*set)(cc_conf_t *cf, cc_command_t *cmd, void *conf);
};

typedef struct cc_conf_file_s
{
    cc_file_t file;
    cc_buf_t *buffer;
    cc_uin32 line;
}cc_conf_file_t;

struct cc_conf_s
{
    cc_array_t *args;
    cc_cycle_t *cycle;
    cc_conf_file_t *conf_file;
    cc_log_t *log;
    void *ctx;
    cc_uin32 module_type;
    cc_uin32 cmd_type;
    cc_conf_handler_pt handler;
    void *handler_conf;
};

cc_char *cc_conf_parse(cc_conf_t *cf, cc_str_t *filename);

#endif
