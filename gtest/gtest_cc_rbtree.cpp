#include "cc_rbtree.h"
#include "gtest/gtest.h"

namespace
{
    class TestRbTree : public testing::Test
    {
        void SetUp() override
        {};

        void TearDown() override
        {};
    };
}

cc_rbtree_t tree;
cc_rbtree_node_t sentinel;
std::vector<cc_rbtree_node_t*> list;

void pre_order(cc_rbtree_node_t *root)
{
    if (root != NULL && root != &sentinel) {
        list.push_back(root);
        pre_order(root->left);
        pre_order(root->right);
    }
}

TEST_F(TestRbTree, TEST_CC_RBTREE)
{
    cc_rbt_init(&tree, &sentinel, cc_rbtree_insert_timer_value);

    cc_rbtree_node_t *p = new cc_rbtree_node_t;
    p->key = 41;
    cc_rbtree_insert(&tree, p);

    p = new cc_rbtree_node_t;
    p->key = 38;
    cc_rbtree_insert(&tree, p);

    p = new cc_rbtree_node_t;
    p->key = 31;
    cc_rbtree_insert(&tree, p);

    p = new cc_rbtree_node_t;
    p->key = 12;
    cc_rbtree_insert(&tree, p);

    p = new cc_rbtree_node_t;
    p->key = 19;
    cc_rbtree_insert(&tree, p);

    p = new cc_rbtree_node_t;
    p->key = 8;
    cc_rbtree_insert(&tree, p);

    pre_order(tree.root);

    ASSERT_EQ(tree.root->key, 38);
    ASSERT_EQ(tree.root->color, BLACK);
    ASSERT_EQ(tree.root->left->key, 19);
    ASSERT_EQ(tree.root->left->color, RED);
    ASSERT_EQ(tree.root->left->left->key, 12);
    ASSERT_EQ(tree.root->left->left->color, BLACK);
    ASSERT_EQ(tree.root->left->left->left->key, 8);
    ASSERT_EQ(tree.root->left->left->left->color, RED);
    ASSERT_EQ(tree.root->left->right->key, 31);
    ASSERT_EQ(tree.root->left->right->color, BLACK);
    ASSERT_EQ(tree.root->right->key, 41);
    ASSERT_EQ(tree.root->right->color, BLACK);
}
