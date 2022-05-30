#include "rb-tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rbt_tree;

struct rbt_node {
    struct rbt_node *left;
    struct rbt_node *right;
    struct rbt_node *parent;
    rbt_color color;
    void *key;
    struct rbt_tree *tree;
};

struct rbt_tree {
    struct rbt_node *root;
    struct rbt_node *nil;
    struct rbt_node sentinel;
    bool allow_dup;
    rbt_node_destruct node_destruct;
    rbt_node_compare node_compare;
};

static void debug_verify_properties(struct rbt_tree *);
static void debug_verify_property_1(struct rbt_tree *, struct rbt_node *);
static void debug_verify_property_2(struct rbt_tree *, struct rbt_node *);
static int debug_node_color(struct rbt_tree *, struct rbt_node *n);
static void debug_verify_property_4(struct rbt_tree *, struct rbt_node *);
static void debug_verify_property_5(struct rbt_tree *, struct rbt_node *);
static void debug_verify_property_5_helper(struct rbt_tree *, struct rbt_node *,
                                           int, int *);

bool rbt_node_is_valid(const struct rbt_node *node)
{
    assert(node);
    assert(node->tree);
    return node && node != node->tree->nil;
}

rbt_color rbt_node_get_color(const struct rbt_node *node)
{
    assert(node);
    return node->color;
}

struct rbt_node *rbt_node_get_left(const struct rbt_node *node)
{
    assert(node);
    return node->left;
}

struct rbt_node *rbt_node_get_right(const struct rbt_node *node)
{
    assert(node);
    return node->right;
}

struct rbt_node *rbt_node_get_parent(const struct rbt_node *node)
{
    assert(node);
    return node->parent;
}

const void *rbt_node_get_key(const struct rbt_node *node)
{
    struct rbt_tree *tree;
    assert(node);
    tree = node->tree;
    assert(tree);
    return (node != tree->nil) ? node->key : (void *)0;
}

static void _do_node_destruct(struct rbt_node *node)
{
    struct rbt_tree *tree;
    assert(node);
    tree = node->tree;
    assert(tree);
    if (node != tree->nil) {
        if (tree->node_destruct) {
            tree->node_destruct(node->key);
        }
    } else {
        assert(false);
    }
}

static void __left_rotate(struct rbt_tree *T, struct rbt_node *x)
{
    struct rbt_node *y = x->right;
    x->right           = y->left;
    if (y->left != T->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == T->nil) {
        T->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left   = x;
    x->parent = y;
}

static void __right_rotate(struct rbt_tree *T, struct rbt_node *x)
{
    struct rbt_node *y = x->left;
    x->left            = y->right;
    if (y->right != T->nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == T->nil) {
        T->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right  = x;
    x->parent = y;
}

#define rb_sentinel(tree) &(tree)->sentinel

struct rbt_tree *rbt_tree_create(bool allow_dup, rbt_node_compare cmp,
                                 rbt_node_destruct dest)
{
    struct rbt_tree *tree = (struct rbt_tree *)calloc(1, sizeof(*tree));
    if (tree != (struct rbt_tree *)0) {
        tree->node_compare    = cmp;
        tree->node_destruct   = dest;
        tree->nil             = rb_sentinel(tree);
        tree->root            = tree->nil;
        tree->sentinel.left   = tree->nil;
        tree->sentinel.right  = tree->nil;
        tree->sentinel.parent = tree->nil;
        tree->sentinel.tree   = tree;
        tree->sentinel.color  = rbt_black;
        tree->allow_dup       = allow_dup;

        /* make code checker happy */
        debug_verify_properties(tree);
    }

    return tree;
}

struct rbt_node *rbt_tree_get_root(struct rbt_tree *tree)
{
    assert(tree);
    return tree->root;
}

static void __rb_insert_fixup(struct rbt_tree *T, struct rbt_node *z)
{
    while (z->parent->color == rbt_red) {
        if (z->parent == z->parent->parent->left) {
            struct rbt_node *y = z->parent->parent->right;
            if (y->color == rbt_red) {
                z->parent->color         = rbt_black;
                y->color                 = rbt_black;
                z->parent->parent->color = rbt_red;
                z                        = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    __left_rotate(T, z);
                }
                z->parent->color         = rbt_black;
                z->parent->parent->color = rbt_red;
                __right_rotate(T, z->parent->parent);
            }
        } else {
            struct rbt_node *y = z->parent->parent->left;
            if (y->color == rbt_red) {
                z->parent->color         = rbt_black;
                y->color                 = rbt_black;
                z->parent->parent->color = rbt_red;
                z                        = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    __right_rotate(T, z);
                }
                z->parent->color         = rbt_black;
                z->parent->parent->color = rbt_red;
                __left_rotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = rbt_black;
}

#if 1

struct rbt_node *rbt_tree_find(struct rbt_tree *tree, const void *key)
{
    struct rbt_node *x;
    int c = 0;
    assert(tree);
    assert(key);
    x = tree->root;
    while ((x != tree->nil) && (c = tree->node_compare(key, x->key)) != 0) {
        x = (c < 0) ? x->left : x->right;
    }
    return x;
}

#else

static struct rbt_node *__tree_search(struct rbt_node *x, const void*k)
{
    struct rbt_tree *tree;
    int cmp;
    assert(x);
    assert(k);
    tree = x->tree;
    assert(tree->node_compare);
    if (x == tree->nil || (cmp = tree->node_compare(k, x->key)) == 0) {
        return x;
    }
    if (cmp < 0) {
        return __tree_search(x->left, k);
    } else {
        return __tree_search(x->right, k);
    }
}

struct rbt_node *rbt_tree_find(struct rbt_tree *tree, const void *key)
{
    assert(tree);
    assert(key);
    return __tree_search(tree->root, key);
}

#endif

static struct rbt_node *_create_node(struct rbt_tree *tree, void *key, size_t s)
{
    struct rbt_node *node = (struct rbt_node *)calloc(1, sizeof(*node));
    if (node) {
        node->left   = tree->nil;
        node->right  = tree->nil;
        node->color  = rbt_red;
        node->parent = tree->nil;
        node->tree   = tree;

        assert(key && s);
        node->key = calloc(s, sizeof(char));
        if (node->key == NULL) {
            free(node);
            node = NULL;
        } else {
            memcpy(node->key, key, s);
        }
    }
    assert(node);
    return node;
}

static void _node_destroy(struct rbt_node *node)
{
    assert(node);
    if (node) {
        _do_node_destruct(node);
        free(node->key);
        node->key = NULL;
        free(node);
    }
}

static int _rb_node_compare(struct rbt_node *lhs, struct rbt_node *rhs)
{
    struct rbt_tree *tree = lhs->tree;
    assert(tree == rhs->tree);
    return tree->node_compare(lhs->key, rhs->key);
}

static void __rb_insert(struct rbt_tree *T, struct rbt_node *z)
{
    struct rbt_node *y = T->nil;
    struct rbt_node *x = T->root;
    while (x != T->nil) {
        y = x;
        if (_rb_node_compare(z, x) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    z->parent = y;
    if (y == T->nil) {
        T->root = z;
    } else if (_rb_node_compare(z, y) < 0) {
        y->left = z;
    } else {
        y->right = z;
    }
    z->left  = T->nil;
    z->right = T->nil;
    z->color = rbt_red;
    __rb_insert_fixup(T, z);
}

rbt_status rbt_tree_insert(struct rbt_tree *tree, void *key, size_t size)
{
    struct rbt_node *x;
    if (tree->allow_dup == false) {
        if (rbt_tree_find(tree, key) != tree->nil) {
            return RBT_STATUS_KEY_DUPLICATE;
        }
    }
    x = _create_node(tree, key, size);
    if (x == (struct rbt_node *)NULL) {
        return RBT_STATUS_MEMORY_OUT;
    }
    __rb_insert(tree, x);

#ifndef NDEBUG
    debug_verify_properties(tree);
#endif
    return RBT_STATUS_SUCCESS;
}

static void __rb_delete_fixup(struct rbt_tree *T, struct rbt_node *x)
{
    while (x != T->root && x->color == rbt_black) {
        if (x == x->parent->left) {
            struct rbt_node *w = x->parent->right;
            if (w->color == rbt_red) {
                w->color         = rbt_black;
                x->parent->color = rbt_red;
                __left_rotate(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == rbt_black && w->right->color == rbt_black) {
                w->color = rbt_red;
                x        = x->parent;
            } else {
                if (w->right->color == rbt_black) {
                    w->left->color = rbt_black;
                    w->color       = rbt_red;
                    __right_rotate(T, w);
                    w = x->parent->right;
                }
                w->color         = x->parent->color;
                x->parent->color = rbt_black;
                w->right->color  = rbt_black;
                __left_rotate(T, x->parent);
                x = T->root;
            }
        } else {
            struct rbt_node *w = x->parent->left;
            if (w->color == rbt_red) {
                w->color         = rbt_black;
                x->parent->color = rbt_red;
                __right_rotate(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == rbt_black && w->left->color == rbt_black) {
                w->color = rbt_red;
                x        = x->parent;
            } else {
                if (w->left->color == rbt_black) {
                    w->right->color = rbt_black;
                    w->color        = rbt_red;
                    __left_rotate(T, w);
                    w = x->parent->left;
                }
                w->color         = x->parent->color;
                x->parent->color = rbt_black;
                w->left->color   = rbt_black;
                __right_rotate(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = rbt_black;
}

static void __rb_transplant(struct rbt_tree *T, struct rbt_node *u,
                            struct rbt_node *v)
{
    if (u->parent == T->nil) {
        T->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

struct rbt_node *__tree_minimum(struct rbt_node *x)
{
    struct rbt_tree *T;
    assert(x);
    T = x->tree;
    assert(T);
    while (x->left != T->nil) {
        x = x->left;
    }
    return x;
}

static void __rb_delete(struct rbt_tree *T, struct rbt_node *z)
{
    struct rbt_node *x, *y = z;
    rbt_color y_original_color = y->color;
    if (z->left == T->nil) {
        x = z->right;
        __rb_transplant(T, z, z->right);
    } else if (z->right == T->nil) {
        x = z->left;
        __rb_transplant(T, z, z->left);
    } else {
        y                = __tree_minimum(z->right);
        y_original_color = y->color;
        x                = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            __rb_transplant(T, y, y->right);
            y->right         = z->right;
            y->right->parent = y;
        }
        __rb_transplant(T, z, y);
        y->left         = z->left;
        y->left->parent = y;
        y->color        = z->color;
    }
    if (y_original_color == rbt_black) {
        __rb_delete_fixup(T, x);
    }
}

rbt_status rbt_tree_remove_node(struct rbt_tree *tree, const void *key)
{
    struct rbt_node *z = rbt_tree_find(tree, key);
    if (z == tree->nil) {
        return RBT_STATUS_KEY_NOT_EXIST;
    }
    __rb_delete(tree, z);
    _node_destroy(z);

#ifndef NDEBUG
    debug_verify_properties(tree);
#endif
    return RBT_STATUS_SUCCESS;
}

#if 1

rbt_status rbt_tree_destroy(struct rbt_tree *tree)
{
    rbt_status rc      = RBT_STATUS_SUCCESS;
    struct rbt_node *z = tree->root;

    while (z != tree->nil) {
        if (z->left != tree->nil) {
            z = z->left;
        } else if (z->right != tree->nil) {
            z = z->right;
        } else {
            if (z->parent != tree->nil) {
                z = z->parent;
                if (z->left != tree->nil) {
                    _node_destroy(z->left);
                    z->left = tree->nil;
                } else if (z->right != tree->nil) {
                    _node_destroy(z->right);
                    z->right = tree->nil;
                }
            } else {
                _node_destroy(z);
                z = tree->nil;
            }
        }
    }
    free(tree);
    return rc;
}

#else

void _rbt_node_destroy_recurse(struct rbt_node *node)
{
    if (node != tree->nil) {
        if (node->left != tree->nil) {
            _rbt_node_destroy_recurse(node->left);
        }
        if (node->right != tree->nil) {
            _rbt_node_destroy_recurse(node->right);
        }
        _node_destroy(node);
    }
}

rbt_status rbt_tree_destroy(struct rbt_tree *tree)
{
    if (tree) {
        _rbt_node_destroy_recurse(tree->root);
        free(tree);
    }
    return RBT_STATUS_SUCCESS;
}

#endif

struct rbt_node *rbt_tree_minimum(struct rbt_tree *tree, struct rbt_node *x)
{
    (void)tree;
    return __tree_minimum(x);
}

struct rbt_node *__tree_maximum(struct rbt_node *x)
{
    struct rbt_tree *tree;
    assert(x);
    tree = x->tree;
    assert(tree);
    if (x == NULL || x == tree->nil) {
        return x;
    }
    while (x->right != tree->nil) {
        x = x->right;
    }
    return x;
}

struct rbt_node *rbt_tree_maximum(struct rbt_tree *tree, struct rbt_node *x)
{
    (void)tree;
    return __tree_maximum(x);
}

bool rbt_tree_is_empty(struct rbt_tree *tree)
{
    assert(tree);
    return (tree->root == tree->nil) ? true : false;
}

struct rbt_node *rbt_tree_successor(struct rbt_tree *tree, struct rbt_node *x)
{
    struct rbt_node *y;
    assert(tree);
    assert(x);
    y = tree->nil;
    if (x->right != tree->nil) {
        return __tree_minimum(x->right);
    }
    if (x == __tree_maximum(tree->root)) {
        return tree->nil;
    }
    y = x->parent;
    while ((y != tree->nil) && (x == y->right)) {
        x = y;
        y = y->parent;
    }
    return y;
}

static void _inorder_tree_walk(struct rbt_node *x, rbt_node_walk_cb cb, void *p)
{
    struct rbt_tree *tree;
    assert(x);
    tree = x->tree;
    assert(tree);
    if (x != tree->nil) {
        _inorder_tree_walk(x->left, cb, p);
        if (cb) {
            cb(x, p);
        }
        _inorder_tree_walk(x->right, cb, p);
    }
}

void rbt_inorder_walk(struct rbt_tree *tree, rbt_node_walk_cb cb, void *p)
{
    assert(tree);
    assert(cb);
    _inorder_tree_walk(tree->root, cb, p);
}

/*
struct rbt_node *
cstl_rb_get_next(struct rbt_tree* tree, struct rbt_node**current, struct rbt_node**pre) {
    struct rbt_node* prev_current;
    assert(tree);
    assert(current);
    assert(pre);
    while ((*current) != tree->nil) {
        if ((*current)->left == tree->nil) {
            prev_current = (*current);
            (*current) = (*current)->right;
            return prev_current;
        } else {
            (*pre) = (*current)->left;
            while (((*pre)->right != tree->nil) && (*pre)->right != (*current))
                (*pre) = (*pre)->right;
            if ((*pre)->right != tree->nil) {
                (*pre)->right = (*current);
                (*current) = (*current)->left;
            } else {
                (*pre)->right = tree->nil;
                prev_current = (*current);
                (*current) = (*current)->right;
                return prev_current;
            }
        }
    }
    return tree->nil;
} */

void debug_verify_properties(struct rbt_tree *t)
{
    debug_verify_property_1(t, t->root);
    debug_verify_property_2(t, t->root);
    debug_verify_property_4(t, t->root);
    debug_verify_property_5(t, t->root);
}

void debug_verify_property_1(struct rbt_tree *tree, struct rbt_node *n)
{
    if (n == tree->nil) {
        return;
    }
    assert(debug_node_color(tree, n) == rbt_red ||
           debug_node_color(tree, n) == rbt_black);
    debug_verify_property_1(tree, n->left);
    debug_verify_property_1(tree, n->right);
}

void debug_verify_property_2(struct rbt_tree *tree, struct rbt_node *root)
{
    (void)tree;
    (void)root;
    assert(debug_node_color(tree, root) == rbt_black);
}

int debug_node_color(struct rbt_tree *tree, struct rbt_node *n)
{
    (void)tree;
    return (n == tree->nil) ? rbt_black : n->color;
}

void debug_verify_property_4(struct rbt_tree *tree, struct rbt_node *n)
{
    if (debug_node_color(tree, n) == rbt_red) {
        assert(debug_node_color(tree, n->left) == rbt_black);
        assert(debug_node_color(tree, n->right) == rbt_black);
        assert(debug_node_color(tree, n->parent) == rbt_black);
    }
    if (n == tree->nil) {
        return;
    }
    debug_verify_property_4(tree, n->left);
    debug_verify_property_4(tree, n->right);
}

void debug_verify_property_5(struct rbt_tree *tree, struct rbt_node *root)
{
    int black_count_path = -1;
    debug_verify_property_5_helper(tree, root, 0, &black_count_path);
}

void debug_verify_property_5_helper(struct rbt_tree *tree, struct rbt_node *n,
                                    int black_count, int *_black_count)
{
    if (debug_node_color(tree, n) == rbt_black) {
        black_count++;
    }
    if (n == tree->nil) {
        if (*_black_count == -1) {
            *_black_count = black_count;
        } else {
            assert(black_count == *_black_count);
        }
        return;
    }
    debug_verify_property_5_helper(tree, n->left, black_count, _black_count);
    debug_verify_property_5_helper(tree, n->right, black_count, _black_count);
}
