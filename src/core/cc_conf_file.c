#include "cc_conf_file.h"

#define CC_CONF_BUFFER  4096

static cc_in32 cc_conf_read_token(cc_conf_t *cf);
static cc_in32 cc_conf_handler(cc_conf_t *cf, cc_in32 last);

// 配置解析
cc_char *
cc_conf_parse(cc_conf_t *cf, cc_str_t *filename)
{
    cc_char *rv;
    cc_in32 rc;
    cc_fd_t fd;
    cc_conf_file_t *prev, conf_file;
    cc_buf_t buf;

    enum {
        parse_file = 0,
        parse_block,
        parse_param
    } type;

    // 异常判断
    if (cf == NULL) 
        return CC_CONF_ERROR;

    if (filename) {
        // 打开一个配置文件
        fd = cc_open_file(filename->data, CC_FILE_RDONLY, CC_FILE_OPEN, 0);

        // 非法文件
        if (fd == CC_INVALID_FILE) {
            // TODO log

            return CC_CONF_ERROR;
        }

        prev = cf->conf_file;
        cf->conf_file = &conf_file;

        // 获取文件状态
        if (cc_fd_info(fd, &cf->conf_file->file.info) == CC_FILE_ERROR) {
            // TODO log
        }

        cf->conf_file->buffer = &buf;

        buf.start = cc_malloc(CC_CONF_BUFFER);
        if (buf.start == NULL) {
            goto failed;
        }

        buf.pos = buf.start;
        buf.last = buf.start;
        buf.end = buf.last + CC_CONF_BUFFER; 
        buf.temporary = 1;

        cf->conf_file->file.fd = fd;
        cf->conf_file->file.name.len = filename->len;
        cf->conf_file->file.name.data = filename->data;
        cf->conf_file->file.offset = 0;
        cf->conf_file->file.log = cf->log;
        cf->conf_file->line = 1;

        type = parse_file;
    // 在遇到带{}的配置项时，filename参数常为空
    // file.fd 描述符为该配置项所在文件被打开时的描述符
    } else if (cf->conf_file->file.fd != CC_INVALID_FILE) {
        type = parse_block;
    // 解析参数
    } else {
        type = parse_param;
    }

    for ( ;; ) {
        // 解析配置项，每找到一个配置项或者出错时就返回
        rc = cc_conf_read_token(cf);

        // 出错
        if (rc == CC_ERROR) {
            goto done;
        }

        // "{}"配置解析已完成
        if (rc == CC_CONF_BLOCK_DONE) {
            if (type != parse_block) {
                // TODO log
                goto failed;
            }

            goto done;
        }

        // 配置文件解析完成
        if (rc == CC_CONF_FILE_DONE) {
            if (type == parse_block) {
                // TODO log
                goto failed;
            }

            goto done;
        }

        // "{}"配置
        if (rc == CC_CONF_BLOCK_START) {
            if (type == parse_param) {
                // TODO log
                goto failed;
            }
        }

        // 已找到一个有效的配置项，调用用户指定的回调函数
        if (cf->handler) {
            if (rc == CC_CONF_BLOCK_START) {
                // TODO log
                goto failed;
            }

            rv = (*cf->handler)(cf, NULL, cf->handler_conf);
            if (rv == CC_CONF_OK) {
                continue;
            }

            if (rv == CC_CONF_ERROR) {
                goto failed;
            }

            // TODO log

            goto failed;
        }

        if (CC_ERROR == cc_conf_handler(cf, rc)) {
            goto failed;
        }
    }

failed:
    rc = CC_ERROR;

done:
    if (filename) {
        if (cf->conf_file->buffer->start) {
            cc_free(cf->conf_file->buffer->start);
        }

        if (cc_close_file(fd) == CC_FILE_ERROR) {
            // TODO log
            rc = CC_ERROR;
        }

        cf->conf_file = prev;
    }

    if (rc == CC_ERROR) {
        return CC_CONF_ERROR;
    }
    
    return CC_CONF_OK;
}

static cc_in32
cc_conf_read_token(cc_conf_t *cf)
{
    cc_char     *start, ch, *src, *dst;
    off_t        file_size;
    size_t       len;
    ssize_t      n, size;
    cc_uin32     found, need_space, last_space, sharp_comment, variable;
    cc_uin32     quoted, s_quoted, d_quoted, start_line;
    cc_str_t     *word;
    cc_buf_t     *b;

    // 标志位,表示找到一个token
    found = 0;
    // 标志位,表示此时需要一个token分隔符,即token前面的分隔符
    // 一般刚刚解析完一对双引号或者单引号,此时设置need_space为1,
    // 表示期待下一个字符为分隔符
    need_space = 0;
    // 标志位,表示上一个字符为token分隔符
    last_space = 1;
    // 注释(#)
    sharp_comment = 0;
    // 遇到字符$后,表示一个变量
    variable = 0;
    // 标志位,表示上一个字符为反引号
    quoted = 0;
    // 标志位,表示已扫描一个双引号,期待另一个双引号
    s_quoted = 0;
    // 标志位,表示已扫描一个单引号,期待另一个单引号
    d_quoted = 0;

    // token的个数
    cf->args->nelts = 0;
    b = cf->conf_file->buffer;
    start = b->pos;
    start_line = cf->conf_file->line;

    file_size = cc_file_size(&cf->conf_file->file.info);

    for ( ;; ) {
        // 文件解析完毕
        if (b->pos > b->last) {
            if (cf->conf_file->file.offset >= file_size) {
                if (cf->args->nelts > 0 || !last_space) {
                    if (cf->conf_file->file.fd == CC_INVALID_FILE) {
                        // TODO log

                        return CC_ERROR;
                    }

                    // TODO log
                    return CC_ERROR;
                }

                return CC_CONF_FILE_DONE;
            }

            len = b->pos - start;

            if (len == CC_CONF_BUFFER) {
                cf->conf_file->line = start_line;

                if (d_quoted) {
                    ch = '"';
                } else if (s_quoted) {
                    ch = '\'';
                } else {
                    // TODO log

                    return CC_ERROR;
                }

                // TODO log
                return CC_ERROR;
            }

            if (len) {
                cc_memmove(b->start, start, len);
            }

            size = (ssize_t) (file_size - cf->conf_file->file.offset);
            if (size > b->end - (b->start + len)) {
                size = b->end - (b->start + len);
            }

            n = cc_read_file(&cf->conf_file->file, b->start + len, size,
                             cf->conf_file->file.offset);
        }
    }

    return CC_OK;
}

static cc_in32
cc_conf_handler(cc_conf_t *cf, cc_in32 last)
{
    return CC_ERROR;
}
