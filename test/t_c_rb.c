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

#include "c_lib.h"
#include "c_rb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BLACK 0
#define RED   1

#define rb_sentinel &tree->sentinel 

static const void* get_key ( struct cstl_rb* tree, struct cstl_rb_node* node) {
    (void)tree;
    if ( node ) 
        return cstl_object_get_data(node->key);
    return (void*)0;
}

static struct cstl_rb_node* get_left (struct cstl_rb* tree, struct cstl_rb_node* node ) {
    if ( node->left != rb_sentinel && node->left != (struct cstl_rb_node*)0 )
        return node->left;
    return (struct cstl_rb_node*)0 ;
}

static struct cstl_rb_node* get_right (struct cstl_rb* tree, struct cstl_rb_node* node ){
    (void)tree;
    if ( node->right != rb_sentinel && node->right != (struct cstl_rb_node*)0 )
        return node->right;
    return (struct cstl_rb_node*)0 ;
}

static struct cstl_rb_node* get_parent ( struct cstl_rb* tree,struct cstl_rb_node* node ) {
    if ( node->parent != rb_sentinel && node->parent != (struct cstl_rb_node*)0 )
        return node->parent;
    return (struct cstl_rb_node*)0 ;
}

int compare_rb_e (const void*  l, const void* r ) {

    int left =  0;
    int right = 0;

    if ( l ) left  = *(int*)l;
    if ( r ) right = *(int*)r;

    if ( left < right ) return -1;
    if ( left == right ) return 0;

    return 1;
}

typedef struct test_data_tree {
    int element;
    int left;
    int right;
    int parent;
    int color;
} TS;


static struct cstl_rb_node* __find_c_rb ( struct cstl_rb* tree, cstl_compare fn_c, const void* key ) {
    struct cstl_rb_node* node = tree->root;
    int compare_result = 0;

    compare_result = (fn_c)(key, cstl_object_get_data(node->key));
    while ((node != rb_sentinel) && (compare_result = (fn_c)(key, cstl_object_get_data(node->key))) != 0 ){
        if ( compare_result < 0 ) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return node;
}
struct cstl_rb_node* find(struct cstl_rb* tree, void* key ) {
    return __find_c_rb ( tree, tree->compare_fn, key );
}

static void update_values ( void* v, int *l, int *r, int *p , int *e, struct cstl_rb* tree ) {
    struct cstl_rb_node* x;
    if ( get_key(tree, (struct cstl_rb_node*)v)) 
        *e = *(int*)get_key (tree, (struct cstl_rb_node*)v);
    x = get_left(tree, (struct cstl_rb_node*)v);
    if ( x )
        *l = *(int*)get_key(tree,x);
    x = get_right(tree, (struct cstl_rb_node*)v);
    if (x) 
        *r = *(int*)get_key(tree,x);
    x = get_parent ( tree, (struct cstl_rb_node*)v );
    if (x)
        *p = *(int*)get_key(tree,x);
}

static void test_each_elements(int l,int r, int p, int e,void* v, TS ts[], int i, struct cstl_rb* tree) {
    assert ( ts[i].element == e);
    if (ts[i].left != 0 ) {
        assert ( ts[i].left == l);
    } else {
        assert ((void* )0 == (void* )get_key(tree,get_left(tree,(struct cstl_rb_node*)v)));
    }
    if ( ts[i].right != 0 ) {
        assert (ts[i].right == r);
    } else {
        assert ((void* )0 == (void* )get_key(tree,get_right(tree,(struct cstl_rb_node*)v)));
    }
    if (ts[i].parent != 0 ) {
        assert (ts[i].parent == p);
    } else {
        assert ((void* )0 == (void* )get_key(tree,get_parent(tree,(struct cstl_rb_node*)v)));
    }
}

static void test_all_elements(struct cstl_rb* tree, TS ts[], int size) {
    int i = 0;
    for ( i = 0; i < size; i++) {
        void* v = (void*)0;
        int l,r,p,e;
        v = find ( tree, &ts[i].element);
        update_values( v, &l,&r,&p,&e, tree);
        test_each_elements(l,r,p,e,v, ts, i, tree);
    }
}

static struct cstl_rb* create_tree(TS ts[], int size) {
    int i = 0;
    struct cstl_rb* tree = cstl_rb_new(compare_rb_e, (cstl_destroy)0, (cstl_destroy)0);
    for ( i = 0; i < size; i++) {
        cstl_rb_insert( tree, &(ts[i].element), sizeof((ts[i].element)), (void*)0, 0);
    }
    return tree;
}


void test_c_rb() {
    int size;
    int size_after_delete;
    int i = 0;
    struct cstl_rb* tree;
    struct cstl_rb_node* node;

    TS ts[] = {
        {15,6,18,0,BLACK},{6,3,9,15,RED},{18,17,20,15,BLACK},
        {3,2,4,6,BLACK},{7,0,0,9,RED},{17,0,0,18,RED},
        {20,0,0,18,RED},{2,0,0,3,RED},{4,0,0,3,RED},{13,0,0,9,RED},	
        {9,7,13,6,BLACK}
    };
    TS ts_delete_leaf_13[] = {
        {15,6,18,0,BLACK},{6,3,9,15,RED},{18,17,20,15,BLACK},
        {3,2,4,6,BLACK},{7,0,0,9,RED},{17,0,0,18,RED},
        {20,0,0,18,RED},{2,0,0,3,RED},{4,0,0,3,RED},
        {9,7,0,6,BLACK}
    };
    TS ts_delete_9[] = {
        {15,6,18,0,BLACK},{6,3,7,15,RED},{18,17,20,15,BLACK},
        {3,2,4,6,RED},{7,0,0,6,RED},{17,0,0,18,RED},
        {20,0,0,18,RED},{2,0,0,3,RED},{4,0,0,3,RED}
    };
    TS ts_delete_15[] = {
        {6,3,7,17,RED},{18,0,20,17,BLACK},
        {3,2,4,6,RED},{7,0,0,6,RED},{17,6,18,0,RED},
        {20,0,0,18,RED},{2,0,0,3,RED},{4,0,0,3,RED}
    };
    TS ts_insert_1[] = {
        {6,3,17,0,BLACK},{18,0,20,17,BLACK},
        {3,2,4,6,RED},{7,0,0,17,RED},{17,7,18,6,RED},
        {20,0,0,18,RED},{2,1,0,3,BLACK},{4,0,0,3,BLACK},
        {1,0,0,2,RED}
    };

    size = (sizeof(ts)/sizeof(TS));
    tree = create_tree(ts,size);
    test_all_elements(tree, ts, size);
    {
        i = 13;
        size = (sizeof(ts)/sizeof(TS));
        size_after_delete = (sizeof(ts_delete_leaf_13)/sizeof(TS));
        node = cstl_rb_remove( tree, &i);
        if ( node != (struct cstl_rb_node*)0  ) {
            cstl_object_delete ( node->key);
            free ( node);
        }
        test_all_elements(tree, ts_delete_leaf_13, size_after_delete);
    }
    {
        i = 9;
        size_after_delete = (sizeof(ts_delete_9)/sizeof(TS));
        node = cstl_rb_remove( tree, &i);
        if ( node != (struct cstl_rb_node*)0  ) {
            cstl_object_delete ( node->key);
            free ( node);
        }
        test_all_elements(tree, ts_delete_9, size_after_delete);
    }
    {
        i = 15;
        size_after_delete = (sizeof(ts_delete_15)/sizeof(TS));
        node = cstl_rb_remove( tree, &i);
        if ( node != (struct cstl_rb_node*)0  ) {
            cstl_object_delete ( node->key);
            free ( node);
        }
        test_all_elements(tree, ts_delete_15, size_after_delete);
    }
    {
        int i = 1;
        cstl_rb_insert( tree, &i, sizeof(i), (void*)0, 0);
        size_after_delete = (sizeof(ts_insert_1)/sizeof(TS));
        test_all_elements(tree, ts_insert_1, size_after_delete);
    }
    {
        cstl_rb_delete(tree);
    }
}
