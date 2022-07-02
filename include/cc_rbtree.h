#ifndef __CC_RBTREE_H__
#define __CC_RBTREE_H__

#include "cc_types.h"
#include "cc_config.h"

#define RED 1           // 红色标记
#define BLACK 0         // 黑色标记

typedef cc_uin32 cc_rbtree_key_t;
typedef cc_in32 cc_rbtree_key_int_t;

typedef struct cc_rbtree_node_s cc_rbtree_node_t;

struct cc_rbtree_node_s
{
    cc_rbtree_key_t key;
    cc_rbtree_node_t *left;
    cc_rbtree_node_t *right;
    cc_rbtree_node_t *parent;
    cc_uin8 color;
    cc_uin8 data;
};

typedef struct cc_rbtree_s cc_rbtree_t;
typedef void (*cc_rbtree_insert_pt) (cc_rbtree_node_t *root,
    cc_rbtree_node_t *node, cc_rbtree_node_t *sentinel);

struct cc_rbtree_s
{
    cc_rbtree_node_t *root;
    cc_rbtree_node_t *sentinel;
    cc_rbtree_insert_pt insert;
};

// 红黑树初始化
#define cc_rbt_init(tree, s, i)       \
    cc_rbt_sentinel_init(s);          \
    (tree)->root = s;                   \
    (tree)->sentinel = s;               \
    (tree)->insert = i;

// 插入一个节点
void cc_rbtree_insert(cc_rbtree_t *tree, cc_rbtree_node_t *node);
// 插入一个value
void cc_rbtree_insert_value(cc_rbtree_node_t *tmp, 
				cc_rbtree_node_t *node, cc_rbtree_node_t *sentinel);
// 插入一个定时器value
void cc_rbtree_insert_timer_value(cc_rbtree_node_t *tmp, 
				cc_rbtree_node_t *node, cc_rbtree_node_t *sentinel);
// 删除一个节点
void cc_rbtree_delete(cc_rbtree_t *tree, cc_rbtree_node_t *node);

#define cc_rbt_red(node)           ((node)->color = RED)
#define cc_rbt_black(node)         ((node)->color = BLACK)
#define cc_rbt_is_red(node)        ((node)->color)
#define cc_rbt_is_black(node)      (!cc_rbt_is_red(node))
#define cc_rbt_copy_color(n1, n2)  (n1->color = n2->color)

// 哨兵节点必须是黑色的
#define cc_rbt_sentinel_init(s)    cc_rbt_black(s)

static cc_inline cc_rbtree_node_t *
cc_rbtree_min(cc_rbtree_node_t *node, cc_rbtree_node_t *sentinel)
{
    while (node->left != sentinel) {
        node = node->left;
    }

    return node;
}

#endif
