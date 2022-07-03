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

#include "c_set.h"
#include <assert.h>
#include <stdio.h>
#include "c_stl_lib.h"
#include "rb-tree.h"

struct cstl_set {
    struct rbt_tree* tree;
};

struct cstl_set* cstl_set_new(cstl_compare fn_c, cstl_destroy fn_d)
{
    struct cstl_set* s = (struct cstl_set*)calloc(1, sizeof(struct cstl_set));
    if (s == (struct cstl_set*)0) {
        return (struct cstl_set*)0;
    }
    s->tree = rbt_tree_create(malloc, free, 0, fn_c, fn_d);
    if (s->tree == (struct rbt_tree*)0) {
        free(s);
        return (struct cstl_set*)0;
    }
    return s;
}

cstl_error cstl_set_insert(struct cstl_set* pSet, void* key, size_t key_size)
{
    rbt_status e;
    if (pSet == (struct cstl_set*)0) {
        return CSTL_SET_NOT_INITIALIZED;
    }
    e = rbt_tree_insert(pSet->tree, key, key_size);
    return e == rbt_status_success ? CSTL_ERROR_SUCCESS : CSTL_ERROR_ERROR;
}

int cstl_set_is_key_exists(struct cstl_set* pSet, void* key)
{
    int found = 0;
    struct rbt_node* node;

    if (pSet == (struct cstl_set*)0) {
        return 0;
    }
    node = rbt_tree_find(pSet->tree, key);
    if (rbt_node_is_valid(node)) {
        return 1;
    }
    return found;
}

cstl_error cstl_set_remove(struct cstl_set* pSet, void* key)
{
    if (pSet == (struct cstl_set*)0) {
        return CSTL_SET_NOT_INITIALIZED;
    }
    return (cstl_error)rbt_tree_remove_node(pSet->tree, key);
}

const void* cstl_set_find(struct cstl_set* pSet, const void* key)
{
    struct rbt_node* node;

    if (pSet == (struct cstl_set*)0) {
        return NULL;
    }
    node = rbt_tree_find(pSet->tree, key);
    if (node == (struct rbt_node*)0) {
        return NULL;
    }
    return rbt_node_get_key(node);
}

cstl_error cstl_set_delete(struct cstl_set* x)
{
    cstl_error rc = CSTL_ERROR_SUCCESS;
    if (x != (struct cstl_set*)0) {
        rc = (cstl_error)rbt_tree_destroy(x->tree);
        free(x);
    }
    return rc;
}

static struct rbt_node* cstl_set_minimum(struct cstl_set* x)
{
    return rbt_tree_minimum(x->tree, rbt_tree_get_root(x->tree));
}

static const void* cstl_set_get_next(struct cstl_iterator* pIterator)
{
    struct cstl_set* x = (struct cstl_set*)pIterator->pContainer;
    struct rbt_node* ptr = NULL;
    if (NULL == pIterator->current_element) {
        pIterator->current_element = cstl_set_minimum(x);
    }
    else {
        pIterator->current_element =
            rbt_tree_successor(x->tree,
                               (struct rbt_node*)pIterator->current_element);
    }
    ptr = (struct rbt_node*)pIterator->current_element;
    if (ptr == NULL || rbt_node_get_key(ptr) == NULL) {
        return NULL;
    }
    return ptr;
}

static const void* cstl_set_get_key(struct cstl_iterator* pIterator)
{
    struct rbt_node* current = (struct rbt_node*)pIterator->current_element;
    return rbt_node_get_key(current);
}

static const void* cstl_set_get_value(struct cstl_iterator* pIterator)
{
    return cstl_set_get_key(pIterator);
}

struct cstl_iterator* cstl_set_new_iterator(struct cstl_set* pSet)
{
    struct cstl_iterator* itr =
        (struct cstl_iterator*)calloc(1, sizeof(struct cstl_iterator));
    if (itr) {
        itr->next = cstl_set_get_next;
        itr->current_key = cstl_set_get_key;
        itr->current_value = cstl_set_get_value;
        itr->pContainer = pSet;
        itr->current_index = 0;
        itr->current_element = (void*)0;
    }
    return itr;
}

void cstl_set_delete_iterator(struct cstl_iterator* pItr)
{
    free(pItr);
}

void cstl_set_container_traverse(struct cstl_set* set, fn_cstl_set_iter fn,
                                 void* p)
{
    struct cstl_iterator* iterator;
    const void* element;
    int stop = 0;
    if (set == NULL || fn == NULL) {
        return;
    }
    iterator = cstl_set_new_iterator(set);
    while ((element = iterator->next(iterator))) {
        const void* obj = *((const void**)iterator->current_value(iterator));
        fn(set, obj, &stop, p);
        if (stop != 0) {
            break;
        }
    }
    cstl_set_delete_iterator(iterator);
}

void cstl_set_container_add(struct cstl_set* set, void* obj)
{
    assert(set && obj);
    cstl_set_insert(set, &obj, sizeof(void*));
}

void cstl_set_container_remove(struct cstl_set* set, void* obj)
{
    assert(0 != cstl_set_is_key_exists(set, &obj));
    cstl_set_remove(set, &obj);
}
