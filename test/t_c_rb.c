/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
 *  This file is part of cstl library
 *  Copyright (C) 2011 Avinash Dongre ( dongre.avinash@gmail.com )
 *  Copyright (C) 2018 ssrlive ( ssrlivebox@gmail.com )
 * 
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 * 
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/

#include "c_rb.h"
#include "c_stl_lib.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const void *get_key(struct cstl_rb *tree, struct cstl_rb_node *node)
{
    (void)tree;
    if (node)
        return cstl_object_get_data(node->key);
    return (void *)0;
}

static struct cstl_rb_node *get_left(struct cstl_rb *tree,
                                     struct cstl_rb_node *node)
{
    if (node->left != rb_sentinel(tree) &&
        node->left != (struct cstl_rb_node *)0)
        return node->left;
    return (struct cstl_rb_node *)0;
}

static struct cstl_rb_node *get_right(struct cstl_rb *tree,
                                      struct cstl_rb_node *node)
{
    (void)tree;
    if (node->right != rb_sentinel(tree) &&
        node->right != (struct cstl_rb_node *)0)
        return node->right;
    return (struct cstl_rb_node *)0;
}

static struct cstl_rb_node *get_parent(struct cstl_rb *tree,
                                       struct cstl_rb_node *node)
{
    if (node->parent != rb_sentinel(tree) &&
        node->parent != (struct cstl_rb_node *)0)
        return node->parent;
    return (struct cstl_rb_node *)0;
}

int compare_rb_e(const void *l, const void *r)
{
    int left  = *(int *)l;
    int right = *(int *)r;

    if (left < right) {
        return -1;
    } else if (left == right) {
        return 0;
    } else {
        return 1;
    }
}

typedef struct test_data_tree {
    int element;
    int left;
    int right;
    int parent;
    enum cstl_rb_color color;
} TS;

static void retrieve_values(struct cstl_rb_node *v, TS *data,
                            struct cstl_rb *tree)
{
    struct cstl_rb_node *x = NULL;
    data->element          = *(int *)get_key(tree, v);
    data->color            = v->color;
    if ((x = get_left(tree, v)))
        data->left = *(int *)get_key(tree, x);
    if ((x = get_right(tree, v)))
        data->right = *(int *)get_key(tree, x);
    if ((x = get_parent(tree, v)))
        data->parent = *(int *)get_key(tree, x);
}

static void test_each_elements(TS *lhs, TS *rhs)
{
    assert(lhs->element == rhs->element);
    assert(lhs->color == rhs->color);
    assert(lhs->left == rhs->left);
    assert(lhs->right == rhs->right);
    assert(lhs->parent == rhs->parent);
    (void)lhs;
    (void)rhs;
}

static void test_all_elements(struct cstl_rb *tree, TS ts[], int size)
{
    int i = 0;
    for (i = 0; i < size; i++) {
        struct test_data_tree data;
        struct cstl_rb_node *v = cstl_rb_find(tree, &ts[i].element);
        memset(&data, 0, sizeof(data));
        retrieve_values(v, &data, tree);
        test_each_elements(&data, &ts[i]);
    }
}

static struct cstl_rb *create_tree(TS ts[], int size)
{
    int i = 0;
    struct cstl_rb *tree =
        cstl_rb_new(compare_rb_e, (cstl_destroy)0, (cstl_destroy)0);
    for (i = 0; i < size; i++) {
        cstl_rb_insert(tree, &(ts[i].element), sizeof((ts[i].element)),
                       (void *)0, 0);
    }
    return tree;
}

void test_c_rb()
{
    int size;
    int size_after_delete;
    int i = 0;
    struct cstl_rb *tree;
    struct cstl_rb_node *node;

    TS ts[] = {
        { 15, 6, 18, 0, cstl_black },   { 6, 3, 9, 15, cstl_red },
        { 18, 17, 20, 15, cstl_black }, { 3, 2, 4, 6, cstl_black },
        { 7, 0, 0, 9, cstl_red },       { 17, 0, 0, 18, cstl_red },
        { 20, 0, 0, 18, cstl_red },     { 2, 0, 0, 3, cstl_red },
        { 4, 0, 0, 3, cstl_red },       { 13, 0, 0, 9, cstl_red },
        { 9, 7, 13, 6, cstl_black },
    };
    TS ts_delete_leaf_13[] = {
        { 15, 6, 18, 0, cstl_black },   { 6, 3, 9, 15, cstl_red },
        { 18, 17, 20, 15, cstl_black }, { 3, 2, 4, 6, cstl_black },
        { 7, 0, 0, 9, cstl_red },       { 17, 0, 0, 18, cstl_red },
        { 20, 0, 0, 18, cstl_red },     { 2, 0, 0, 3, cstl_red },
        { 4, 0, 0, 3, cstl_red },       { 9, 7, 0, 6, cstl_black },
    };
    TS ts_delete_9[] = {
        { 15, 6, 18, 0, cstl_black },   { 6, 3, 7, 15, cstl_red },
        { 18, 17, 20, 15, cstl_black }, { 3, 2, 4, 6, cstl_black },
        { 7, 0, 0, 6, cstl_black },     { 17, 0, 0, 18, cstl_red },
        { 20, 0, 0, 18, cstl_red },     { 2, 0, 0, 3, cstl_red },
        { 4, 0, 0, 3, cstl_red },
    };
    TS ts_delete_15[] = {
        { 6, 3, 7, 17, cstl_red },    { 18, 0, 20, 17, cstl_black },
        { 3, 2, 4, 6, cstl_black },   { 7, 0, 0, 6, cstl_black },
        { 17, 6, 18, 0, cstl_black }, { 20, 0, 0, 18, cstl_red },
        { 2, 0, 0, 3, cstl_red },     { 4, 0, 0, 3, cstl_red },
    };
    TS ts_insert_1[] = {
        { 6, 3, 17, 0, cstl_black }, { 18, 0, 20, 17, cstl_black },
        { 3, 2, 4, 6, cstl_red },    { 7, 0, 0, 17, cstl_black },
        { 17, 7, 18, 6, cstl_red },  { 20, 0, 0, 18, cstl_red },
        { 2, 1, 0, 3, cstl_black },  { 4, 0, 0, 3, cstl_black },
        { 1, 0, 0, 2, cstl_red },
    };

    size = (sizeof(ts) / sizeof(TS));
    tree = create_tree(ts, size);
    test_all_elements(tree, ts, size);
    {
        i                 = 13;
        size              = (sizeof(ts) / sizeof(TS));
        size_after_delete = (sizeof(ts_delete_leaf_13) / sizeof(TS));
        node              = cstl_rb_remove(tree, &i);
        if (node != (struct cstl_rb_node *)0) {
            cstl_rb_node_clearup(node, cstl_true);
        }
        test_all_elements(tree, ts_delete_leaf_13, size_after_delete);
    }
    {
        i                 = 9;
        size_after_delete = (sizeof(ts_delete_9) / sizeof(TS));
        node              = cstl_rb_remove(tree, &i);
        if (node != (struct cstl_rb_node *)0) {
            cstl_rb_node_clearup(node, cstl_true);
        }
        test_all_elements(tree, ts_delete_9, size_after_delete);
    }
    {
        i                 = 15;
        size_after_delete = (sizeof(ts_delete_15) / sizeof(TS));
        node              = cstl_rb_remove(tree, &i);
        if (node != (struct cstl_rb_node *)0) {
            cstl_rb_node_clearup(node, cstl_true);
        }
        test_all_elements(tree, ts_delete_15, size_after_delete);
    }
    {
        int i = 1;
        cstl_rb_insert(tree, &i, sizeof(i), (void *)0, 0);
        size_after_delete = (sizeof(ts_insert_1) / sizeof(TS));
        test_all_elements(tree, ts_insert_1, size_after_delete);
    }
    {
        cstl_rb_delete(tree);
    }
}

void test_c_rb2(void)
{
    struct cstl_rb_node *node;
    int i;
    struct cstl_rb *t = cstl_rb_new(compare_rb_e, NULL, NULL);

    for (i = 0; i < 5000; i++) {
        int x = rand() % 10000;
        int y = rand() % 10000;
        if (CSTL_RBTREE_KEY_DUPLICATE ==
            cstl_rb_insert(t, &x, sizeof(x), &y, sizeof(y))) {
            continue;
        }
        node = cstl_rb_find(t, &x);
        assert(*((int *)cstl_object_get_data(node->value)) == y);
    }
    for (i = 0; i < 60000; i++) {
        int x = rand() % 10000;
        node  = cstl_rb_remove(t, &x);
        if (node != NULL) {
            cstl_rb_node_clearup(node, cstl_true);
        }
    }
    cstl_rb_delete(t);
}
