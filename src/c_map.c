/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **
 *  This file is part of cstl library
 *  Copyright (C) 2011 Avinash Dongre ( dongre.avinash@gmail.com )
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
#include <stdio.h>

struct cstl_map*
cstl_map_new(cstl_compare fn_c_k, cstl_destroy fn_k_d, cstl_destroy fn_v_d) {
    struct cstl_map* pMap = (struct cstl_map*)calloc(1, sizeof(struct cstl_map));
    if (pMap == (struct cstl_map*)0) {
        return (struct cstl_map*)0;
    }
    pMap->root = cstl_rb_new(fn_c_k, fn_k_d, fn_v_d);
    if (pMap->root == (struct cstl_rb*)0) {
        return (struct cstl_map*)0;
    }
    return pMap;
}

cstl_error
cstl_map_insert(struct cstl_map* pMap, void* key, size_t key_size, void* value, size_t value_size) {
    if (pMap == (struct cstl_map*)0) {
        return CSTL_MAP_NOT_INITIALIZED;
    }
    return cstl_rb_insert(pMap->root, key, key_size, value, value_size);
}

cstl_bool
cstl_map_exists(struct cstl_map* pMap, void* key) {
    cstl_bool found = cstl_false;
    struct cstl_rb_node* node;

    if (pMap == (struct cstl_map*)0) {
        return cstl_false;
    }
    node = cstl_rb_find(pMap->root, key);
    if (node != (struct cstl_rb_node*)0) {
        return cstl_true;
    }
    return found;
}

cstl_error
cstl_map_remove(struct cstl_map* pMap, void* key) {
    cstl_error rc = CSTL_ERROR_SUCCESS;
    struct cstl_rb_node* node;
    if (pMap == (struct cstl_map*)0) {
        return CSTL_MAP_NOT_INITIALIZED;
    }
    node = cstl_rb_remove(pMap->root, key);
    if (node != (struct cstl_rb_node*)0) {
        void* removed_node = (void *)0;
        if (pMap->root->destruct_k_fn) {
            if (cstl_object_get_raw(node->key, &removed_node) == CSTL_ERROR_SUCCESS) {
                pMap->root->destruct_k_fn(removed_node);
                free(removed_node);
            }
        }
        cstl_object_delete(node->key);

        if (pMap->root->destruct_v_fn) {
            if (cstl_object_get_raw(node->value, &removed_node) == CSTL_ERROR_SUCCESS) {
                pMap->root->destruct_v_fn(removed_node);
                free(removed_node);
            }
        }
        cstl_object_delete(node->value);

        free(node);
    }
    return rc;
}

cstl_bool
cstl_map_find(struct cstl_map* pMap, void* key, void**value) {
    struct cstl_rb_node* node;

    if (pMap == (struct cstl_map*)0) {
        return cstl_false;
    }
    node = cstl_rb_find(pMap->root, key);
    if (node == (struct cstl_rb_node*)0) {
        return cstl_false;
    }
    cstl_object_get_raw(node->value, value);

    return cstl_true;
}

cstl_error
cstl_map_delete(struct cstl_map* x) {
    cstl_error rc = CSTL_ERROR_SUCCESS;
    if (x != (struct cstl_map*)0) {
        rc = cstl_rb_delete(x->root);
        free(x);
    }
    return rc;
}

static struct cstl_rb_node *
minimum_c_map(struct cstl_map *x) {
    return cstl_rb_minimum(x->root, x->root->root);
}

static struct cstl_object*
get_next_c_map(struct cstl_iterator* pIterator) {
    if (!pIterator->pCurrentElement) {
        pIterator->pCurrentElement = minimum_c_map(pIterator->pContainer);
    } else {
        struct cstl_map *x = (struct cstl_map*)pIterator->pContainer;
        pIterator->pCurrentElement = cstl_rb_tree_successor(x->root, pIterator->pCurrentElement);
    }
    if (!pIterator->pCurrentElement) {
        return (struct cstl_object*)0;
    }
    return ((struct cstl_rb_node*)pIterator->pCurrentElement)->value;
}

static void*
get_value_c_map(void* pObject) {
    void* elem = (void *)0;
    cstl_object_get_raw(pObject, &elem);
    return elem;
}

static void
replace_value_c_map(struct cstl_iterator *pIterator, void* elem, size_t elem_size) {
    struct cstl_map *pMap = (struct cstl_map*)pIterator->pContainer;

    if (pMap->root->destruct_v_fn) {
        void* old_element;
        if (cstl_object_get_raw(pIterator->pCurrentElement, &old_element) == CSTL_ERROR_SUCCESS) {
            pMap->root->destruct_v_fn(old_element);
            free(old_element);
        }
    }
    cstl_object_replace_raw(((struct cstl_rb_node*)pIterator->pCurrentElement)->value, elem, elem_size);
}

struct cstl_iterator*
cstl_map_new_iterator(struct cstl_map* pMap) {
    struct cstl_iterator *itr = (struct cstl_iterator*)calloc(1, sizeof(struct cstl_iterator));
    itr->get_next = get_next_c_map;
    itr->get_value = get_value_c_map;
    itr->replace_value = replace_value_c_map;
    itr->pContainer = pMap;
    itr->pCurrent = 0;
    itr->pCurrentElement = (void*)0;
    return itr;
}

void
cstl_map_delete_iterator(struct cstl_iterator* pItr) {
    free(pItr);
}
