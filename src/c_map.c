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

#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include "c_stl_lib.h"
#include "rb-tree.h"

struct cstl_map {
    struct rbt_tree* tree;
    int map_changed;
    cstl_compare fn_c_k;
    cstl_destroy fn_k_d;
    cstl_destroy fn_v_d;
};

struct cstl_map_item {
    struct cstl_map* pMap;
    void* key;
    void* value;
};

static void __map_item_destruct(struct cstl_map_item* item);

static void _item_destruct(void* p)
{
    struct cstl_map_item* item = (struct cstl_map_item*)p;
    assert(item);
    __map_item_destruct(item);
}

static int _item_compare(const void* lhs, const void* rhs)
{
    struct cstl_map* pMap;
    struct cstl_map_item* item_lhs = (struct cstl_map_item*)lhs;
    struct cstl_map_item* item_rhs = (struct cstl_map_item*)rhs;
    assert(item_lhs);
    assert(item_rhs);
    pMap = item_lhs->pMap;
    assert(pMap == item_rhs->pMap);
    assert(pMap->fn_c_k);
    return pMap->fn_c_k(item_lhs->key, item_rhs->key);
}

struct cstl_map* cstl_map_new(cstl_compare fn_c_k, cstl_destroy fn_k_d,
                              cstl_destroy fn_v_d)
{
    struct cstl_map* pMap = (struct cstl_map*)calloc(1, sizeof(*pMap));
    if (pMap) {
        pMap->map_changed = 0;
        pMap->fn_c_k = fn_c_k;
        pMap->fn_k_d = fn_k_d;
        pMap->fn_v_d = fn_v_d;
        pMap->tree =
            rbt_tree_create(malloc, free, 0, _item_compare, _item_destruct);
        if (pMap->tree == (struct rbt_tree*)NULL) {
            free(pMap);
            pMap = NULL;
        }
    }
    return pMap;
}

static void _map_item_value_destroy(struct cstl_map_item* item)
{
    struct cstl_map* pMap = item->pMap;
    if (pMap->fn_v_d) {
        pMap->fn_v_d(item->value);
    }
    free(item->value);
    item->value = NULL;
}

static void _map_item_set_value(struct cstl_map_item* item, const void* value,
                                size_t value_size)
{
    if (item->value) {
        _map_item_value_destroy(item);
    }
    if (value && value_size) {
        item->value = calloc(value_size, sizeof(char));
        assert(item->value);
        memcpy(item->value, value, value_size);
    }
}

static void map_item_init(struct cstl_map* pMap, struct cstl_map_item* item,
                          const void* key, size_t key_size, const void* value,
                          size_t value_size)
{
    assert(pMap);
    assert(key && key_size);
    item->pMap = pMap;
    item->key = calloc(key_size, sizeof(char));
    assert(item->key);
    memcpy(item->key, key, key_size);
    _map_item_set_value(item, value, value_size);
}

static void __map_item_destruct(struct cstl_map_item* item)
{
    if (item) {
        struct cstl_map* pMap = item->pMap;
        assert(pMap);

        if (pMap->fn_k_d) {
            pMap->fn_k_d(item->key);
        }
        free(item->key);
        item->key = NULL;

        _map_item_value_destroy(item);
    }
}

cstl_error cstl_map_insert(struct cstl_map* pMap, const void* key,
                           size_t key_size, const void* value,
                           size_t value_size)
{
    cstl_error rc = CSTL_ERROR_SUCCESS;
    rbt_status rcrb;
    struct cstl_map_item dummy;
    if (pMap == (struct cstl_map*)NULL) {
        return CSTL_MAP_NOT_INITIALIZED;
    }
    if (cstl_map_is_key_exists(pMap, key)) {
        return CSTL_RBTREE_KEY_DUPLICATE;
    }
    memset(&dummy, 0, sizeof(dummy));
    map_item_init(pMap, &dummy, key, key_size, value, value_size);

    rcrb = rbt_tree_insert(pMap->tree, &dummy, sizeof(dummy));
    if (rcrb == rbt_status_success) {
        pMap->map_changed = 1;
    }
    else {
        assert(0);
    }
    return rc;
}

int cstl_map_is_key_exists(struct cstl_map* pMap, const void* key)
{
    struct rbt_node* node;

    struct cstl_map_item dummy;
    dummy.pMap = pMap;
    dummy.key = (void*)key;

    if (pMap == (struct cstl_map*)0) {
        return 0;
    }
    node = rbt_tree_find(pMap->tree, &dummy);
    return rbt_node_is_valid(node);
}

cstl_error cstl_map_replace(struct cstl_map* pMap, const void* key,
                            const void* value, size_t value_size)
{
    struct rbt_node* node;
    struct cstl_map_item* data;

    struct cstl_map_item dummy;
    dummy.pMap = pMap;
    dummy.key = (void*)key;

    if (pMap == (struct cstl_map*)0) {
        return CSTL_MAP_NOT_INITIALIZED;
    }
    node = rbt_tree_find(pMap->tree, &dummy);
    if (!rbt_node_is_valid(node)) {
        return CSTL_RBTREE_KEY_NOT_FOUND;
    }

    data = (struct cstl_map_item*)rbt_node_get_key(node);
    _map_item_set_value(data, value, value_size);
    return CSTL_ERROR_SUCCESS;
}

cstl_error cstl_map_remove(struct cstl_map* pMap, const void* key)
{
    cstl_error rc = CSTL_ERROR_SUCCESS;
    rbt_status rcrb;

    struct cstl_map_item dummy;
    dummy.pMap = pMap;
    dummy.key = (void*)key;

    if (pMap == (struct cstl_map*)0) {
        return CSTL_MAP_NOT_INITIALIZED;
    }
    rcrb = rbt_tree_remove_node(pMap->tree, &dummy);
    if (rbt_status_success == rcrb) {
        pMap->map_changed = 1;
    }
    return rc;
}

const void* cstl_map_find(struct cstl_map* pMap, const void* key)
{
    struct rbt_node* node;
    struct cstl_map_item* data;

    struct cstl_map_item dummy;
    dummy.pMap = pMap;
    dummy.key = (void*)key;

    if (pMap == (struct cstl_map*)0) {
        return (void*)0;
    }
    node = rbt_tree_find(pMap->tree, (void*)&dummy);
    if (0 == rbt_node_is_valid(node)) {
        return (void*)0;
    }
    data = (struct cstl_map_item*)rbt_node_get_key(node);
    return data->value;
}

cstl_error cstl_map_delete(struct cstl_map* x)
{
    cstl_error rc = CSTL_ERROR_SUCCESS;
    if (x != (struct cstl_map*)0) {
        rbt_tree_destroy(x->tree);
        free(x);
    }
    return rc;
}

static struct rbt_node* cstl_map_minimum(struct cstl_map* x)
{
    return rbt_tree_minimum(x->tree, rbt_tree_get_root(x->tree));
}

static const void* cstl_map_iter_get_next(struct cstl_iterator* pIterator)
{
    struct cstl_map* x = (struct cstl_map*)pIterator->pContainer;
    struct rbt_node* ptr = NULL;
    if (NULL == pIterator->current_element) {
        pIterator->current_element = cstl_map_minimum(x);
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

static const void* cstl_map_iter_get_key(struct cstl_iterator* pIterator)
{
    struct rbt_node* current = (struct rbt_node*)pIterator->current_element;
    struct cstl_map_item* d = (struct cstl_map_item*)rbt_node_get_key(current);
    return d->key;
}

static const void* cstl_map_iter_get_value(struct cstl_iterator* pIterator)
{
    struct rbt_node* current = (struct rbt_node*)pIterator->current_element;
    struct cstl_map_item* d = (struct cstl_map_item*)rbt_node_get_key(current);
    return d->value;
}

static void cstl_map_iter_replace_value(struct cstl_iterator* pIterator,
                                        void* elem, size_t elem_size)
{
    /* struct cstl_map *pMap = (struct cstl_map *)pIterator->pContainer; */
    struct rbt_node* current = (struct rbt_node*)pIterator->current_element;
    struct cstl_map_item* d = (struct cstl_map_item*)rbt_node_get_key(current);
    _map_item_set_value(d, elem, elem_size);
}

struct cstl_iterator* cstl_map_new_iterator(struct cstl_map* pMap)
{
    struct cstl_iterator* itr =
        (struct cstl_iterator*)calloc(1, sizeof(struct cstl_iterator));
    if (itr) {
        itr->next = cstl_map_iter_get_next;
        itr->current_key = cstl_map_iter_get_key;
        itr->current_value = cstl_map_iter_get_value;
        itr->replace_current_value = cstl_map_iter_replace_value;
        itr->pContainer = pMap;
        itr->current_index = 0;
        itr->current_element = (void*)0;
        pMap->map_changed = 0;
    }
    return itr;
}

void cstl_map_delete_iterator(struct cstl_iterator* pItr)
{
    free(pItr);
}

void cstl_map_traverse(struct cstl_map* map, map_iter_callback cb, void* p)
{
    struct cstl_iterator* iterator;
    const void* element;
    int stop = 0;
    if (map == NULL || cb == NULL) {
        return;
    }
    iterator = cstl_map_new_iterator(map);
    while ((element = iterator->next(iterator))) {
        const void* key = iterator->current_key(iterator);
        const void* value = iterator->current_value(iterator);
        cb(map, key, value, &stop, p);
        if (stop != 0) {
            break;
        }
        if (map->map_changed) {
            cstl_map_delete_iterator(iterator);
            iterator = cstl_map_new_iterator(map);
        }
    }
    cstl_map_delete_iterator(iterator);
}

void cstl_map_const_traverse(struct cstl_map* map, fn_map_walker fn, void* p)
{
    struct cstl_iterator* iterator;
    int stop = 0;
    const void* element;
    if (map == NULL || fn == NULL) {
        return;
    }
    iterator = cstl_map_new_iterator(map);
    while ((element = iterator->next(iterator))) {
        const void* key = iterator->current_key(iterator);
        const void* value = iterator->current_value(iterator);
        fn(key, value, &stop, p);
        if (stop != 0) {
            break;
        }
    }
    cstl_map_delete_iterator(iterator);
}
