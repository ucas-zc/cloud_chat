#include "cc_rbtree.h"

static void cc_rbtree_left_rotate(cc_rbtree_node_t **root, 
	cc_rbtree_node_t *sentinel, cc_rbtree_node_t *node);
static void cc_rbtree_right_rotate(cc_rbtree_node_t **root, 
	cc_rbtree_node_t *sentinel, cc_rbtree_node_t *node);

// 插入一个节点
void 
cc_rbtree_insert(cc_rbtree_t *tree, cc_rbtree_node_t *node)
{
    cc_rbtree_node_t **root, *tmp, *sentinel;

    if (tree == NULL || node == NULL) {
        return;
    }

    root = &tree->root;
    sentinel = tree->sentinel;

    // 空树，将新节点设置为根节点
    if (*root == sentinel) {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        cc_rbt_black(node);
        *root = node;

        return;
    }

    // 插入新的节点
    tree->insert(*root, node, sentinel);

    // 开始平衡操作
    while (node != *root && cc_rbt_is_red(node->parent)) {
        if (node->parent == node->parent->parent->left) {
            tmp = node->parent->parent->right;

            if (cc_rbt_is_red(tmp)) {
                cc_rbt_black(node->parent);
                cc_rbt_black(tmp);
                cc_rbt_red(node->parent->parent);
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    // 左旋
                    cc_rbtree_left_rotate(root, sentinel, node);
                }

                cc_rbt_black(node->parent);
                cc_rbt_red(node->parent->parent);
                // 右旋
                cc_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }
		} else {
            tmp = node->parent->parent->left;
            
            if (cc_rbt_is_red(tmp)) {
                cc_rbt_black(node->parent);
                cc_rbt_black(tmp);
                cc_rbt_red(node->parent->parent);
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    // 右旋
                    cc_rbtree_right_rotate(root, sentinel, node);
                }

                cc_rbt_black(node->parent);
                cc_rbt_red(node->parent->parent);
                // 左旋
                cc_rbtree_left_rotate(root, sentinel, node->parent->parent);
            }
        }
    }

    cc_rbt_black(*root);

    return;
}

// 插入一个value
void
cc_rbtree_insert_value(cc_rbtree_node_t *tmp, cc_rbtree_node_t *node,
    cc_rbtree_node_t *sentinel)
{
    cc_rbtree_node_t  **p;

    for ( ;; ) {
        p = (node->key < tmp->key) ? &tmp->left : &tmp->right;

        if (*p == sentinel) {
            break;
        }

        tmp = *p;
    }

    *p = node;
    node->parent = tmp;
    node->left = sentinel;
    node->right = sentinel;
    cc_rbt_red(node);
}

void
cc_rbtree_insert_timer_value(cc_rbtree_node_t *tmp, cc_rbtree_node_t *node,
    cc_rbtree_node_t *sentinel)
{
    cc_rbtree_node_t  **p;

    for ( ;; ) {
        p = ((cc_rbtree_key_int_t) (node->key - tmp->key) < 0)
            ? &tmp->left : &tmp->right;

        if (*p == sentinel) {
            break;
        }

        tmp = *p;
    }

    *p = node;
    node->parent = tmp;
    node->left = sentinel;
    node->right = sentinel;
    cc_rbt_red(node);
}

// 删除一个节点
void 
cc_rbtree_delete(cc_rbtree_t *tree, cc_rbtree_node_t *node)
{
    cc_uin32           red;
    cc_rbtree_node_t  **root, *sentinel, *subst, *tmp, *w;

    /* 删除节点 */
    root = &tree->root;
    sentinel = tree->sentinel;
    if (node->left == sentinel) {
        tmp = node->right;
        subst = node;
    } else if (node->right == sentinel) {
        tmp = node->left;
        subst = node;
    } else {
        subst = (cc_rbtree_node_t*)cc_rbtree_min(node->right, sentinel);
        tmp = subst->right;
    }

    if (subst == *root) {
        *root = tmp;
        cc_rbt_black(tmp);

        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key = 0;

        return;
    }

    red = cc_rbt_is_red(subst);
    if (subst == subst->parent->left) {
        subst->parent->left = tmp;
    } else {
        subst->parent->right = tmp;
    }

    if (subst == node) {
        tmp->parent = subst->parent;
    } else {
        if (subst->parent == node) {
            tmp->parent = subst;
        } else {
            tmp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        cc_rbt_copy_color(subst, node);

        if (node == *root) {
            *root = subst;
        } else {
            if (node == node->parent->left) {
                node->parent->left = subst;
            } else {
                node->parent->right = subst;
            }
        }

        if (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        if (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0;

    if (red) {
        return;
    }

    // 调整
    while (tmp != *root && cc_rbt_is_black(tmp)) {
        if (tmp == tmp->parent->left) {
            w = tmp->parent->right;

            if (cc_rbt_is_red(w)) {
                cc_rbt_black(w);
                cc_rbt_red(tmp->parent);
                cc_rbtree_left_rotate(root, sentinel, tmp->parent);
                w = tmp->parent->right;
            }

            if (cc_rbt_is_black(w->left) && cc_rbt_is_black(w->right)) {
                cc_rbt_red(w);
                tmp = tmp->parent;
            } else {
                if (cc_rbt_is_black(w->right)) {
                    cc_rbt_black(w->left);
                    cc_rbt_red(w);
                    cc_rbtree_right_rotate(root, sentinel, w);
                    w = tmp->parent->right;
                }

                cc_rbt_copy_color(w, tmp->parent);
                cc_rbt_black(tmp->parent);
                cc_rbt_black(w->right);
                cc_rbtree_left_rotate(root, sentinel, tmp->parent);
                tmp = *root;
            }
        } else {
            w = tmp->parent->left;

            if (cc_rbt_is_red(w)) {
                cc_rbt_black(w);
                cc_rbt_red(tmp->parent);
                cc_rbtree_right_rotate(root, sentinel, tmp->parent);
                w = tmp->parent->left;
            }

            if (cc_rbt_is_black(w->left) && cc_rbt_is_black(w->right)) {
                cc_rbt_red(w);
                tmp = tmp->parent;
            } else {
                if (cc_rbt_is_black(w->left)) {
                    cc_rbt_black(w->right);
                    cc_rbt_red(w);
                    cc_rbtree_left_rotate(root, sentinel, w);
                    w = tmp->parent->left;
                }

                cc_rbt_copy_color(w, tmp->parent);
                cc_rbt_black(tmp->parent);
                cc_rbt_black(w->left);
                cc_rbtree_right_rotate(root, sentinel, tmp->parent);
                tmp = *root;
            }
        }
    }

    cc_rbt_black(tmp);
}

// 左旋
static void cc_rbtree_left_rotate(cc_rbtree_node_t **root, 
	cc_rbtree_node_t *sentinel, cc_rbtree_node_t *node)
{
    cc_rbtree_node_t  *tmp;

    tmp = node->right;
    node->right = tmp->left;

    if (tmp->left != sentinel) {
        tmp->left->parent = node;
    }

    tmp->parent = node->parent;

    if (node == *root) {
        *root = tmp;
    } else if (node == node->parent->left) {
        node->parent->left = tmp;
    } else {
        node->parent->right = tmp;
    }

    tmp->left = node;
    node->parent = tmp;
}

// 右旋
static void cc_rbtree_right_rotate(cc_rbtree_node_t **root, 
	cc_rbtree_node_t *sentinel, cc_rbtree_node_t *node)
{
    cc_rbtree_node_t  *tmp;

    tmp = node->left;
    node->left = tmp->right;

    if (tmp->right != sentinel) {
        tmp->right->parent = node;
    }

    tmp->parent = node->parent;

    if (node == *root) {
        *root = tmp;
    } else if (node == node->parent->right) {
        node->parent->right = tmp;
    } else {
        node->parent->left = tmp;
    }

    tmp->right = node;
    node->parent = tmp;
}
