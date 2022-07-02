#ifndef __CC_CORE_H__
#define __CC_CORE_H__

#include "cc_types.h"

#define LF     (cc_char) '\n'
#define CR     (cc_char) '\r'

typedef struct cc_event_s cc_event_t;
typedef struct cc_connection_s cc_connection_t;
typedef struct cc_module_s cc_module_t;
typedef struct cc_command_s cc_command_t;
typedef struct cc_cycle_s cc_cycle_t;
typedef struct cc_conf_s cc_conf_t;
typedef struct cc_file_s cc_file_t;

typedef void (*cc_event_handler_pt)(cc_event_t *ev);
typedef void (*cc_connection_handler_pt)(cc_connection_t *conn);

#define cc_abs(value)       (((value) >= 0) ? (value) : - (value))

#endif
