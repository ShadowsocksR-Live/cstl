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

struct cstl_map {
    struct cstl_rb* root;
    cstl_bool map_changed;
};

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
cstl_map_insert(struct cstl_map* pMap, const void* key, size_t key_size, const void* value, size_t value_size) {
    cstl_error rc = CSTL_ERROR_SUCCESS;
    if (pMap == (struct cstl_map*)0) {
        return CSTL_MAP_NOT_INITIALIZED;
    }
    rc = cstl_rb_insert(pMap->root, key, key_size, value, value_size);
    if (rc == CSTL_ERROR_SUCCESS) {
        pMap->map_changed = cstl_true;
    }
    return rc;
}

cstl_bool
cstl_map_exists(struct cstl_map* pMap, const void* key) {
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
cstl_map_replace(struct cstl_map* pMap, const void* key, const void* value,  size_t value_size) {
    struct cstl_rb_node* node;
    if (pMap == (struct cstl_map*)0) {
        return CSTL_MAP_NOT_INITIALIZED;
    }
    node = cstl_rb_find(pMap->root, key);
    if (node == (struct cstl_rb_node*)0) {
        return CSTL_RBTREE_KEY_NOT_FOUND;
    }

    if (pMap->root->destruct_v_fn) {
        void* old_element = (void *)cstl_object_get_data(node->value);
        if (old_element) {
            pMap->root->destruct_v_fn(old_element);
        }
    }
    cstl_object_replace_raw(node->value, value, value_size);
    return CSTL_ERROR_SUCCESS;
}


cstl_error
cstl_map_remove(struct cstl_map* pMap, const void* key) {
    cstl_error rc = CSTL_ERROR_SUCCESS;
    struct cstl_rb_node* node;
    if (pMap == (struct cstl_map*)0) {
        return CSTL_MAP_NOT_INITIALIZED;
    }
    node = cstl_rb_remove(pMap->root, key);
    if (node != (struct cstl_rb_node*)0) {
        void* removed_node = (void *)0;
        if (pMap->root->destruct_k_fn) {
            removed_node = (void *) cstl_object_get_data(node->key);
            if (removed_node) {
                pMap->root->destruct_k_fn(removed_node);
            }
        }
        cstl_object_delete(node->key);

        if (pMap->root->destruct_v_fn) {
            removed_node = (void *) cstl_object_get_data(node->value);
            if (removed_node) {
                pMap->root->destruct_v_fn(removed_node);
            }
        }
        cstl_object_delete(node->value);

        free(node);
        pMap->map_changed = cstl_true;
    }
    return rc;
}

const void *
cstl_map_find(struct cstl_map* pMap, const void* key) {
    struct cstl_rb_node* node;

    if (pMap == (struct cstl_map*)0) {
        return (void *)0;
    }
    node = cstl_rb_find(pMap->root, (void *) key);
    if (node == (struct cstl_rb_node*)0) {
        return (void *)0;
    }
    return cstl_object_get_data(node->value);
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
cstl_map_minimum(struct cstl_map *x) {
    return cstl_rb_minimum(x->root, x->root->root);
}

static const void *
cstl_map_iter_get_next(struct cstl_iterator* pIterator) {
    struct cstl_map *x = (struct cstl_map*)pIterator->pContainer;
    struct cstl_rb_node *ptr = NULL;
    if (!pIterator->current_element) {
        pIterator->current_element = cstl_map_minimum(x);
    } else {
        pIterator->current_element = cstl_rb_tree_successor(x->root, (struct cstl_rb_node*)pIterator->current_element);
    }
    ptr = (struct cstl_rb_node*)pIterator->current_element;
    if (ptr==NULL || ptr->key==NULL) {
        return NULL;
    }
    return ptr;
}

static const void*
cstl_map_iter_get_key(struct cstl_iterator *pIterator) {
    struct cstl_rb_node *current = (struct cstl_rb_node*)pIterator->current_element;
    return cstl_object_get_data(current->key);
}

static const void*
cstl_map_iter_get_value(struct cstl_iterator *pIterator) {
    struct cstl_rb_node* current = (struct cstl_rb_node*)pIterator->current_element;
    return cstl_object_get_data(current->value);
}

static void
cstl_map_iter_replace_value(struct cstl_iterator *pIterator, void* elem, size_t elem_size) {
    struct cstl_map *pMap = (struct cstl_map*)pIterator->pContainer;
    struct cstl_rb_node* node = (struct cstl_rb_node*)pIterator->current_element;

    if (pMap->root->destruct_v_fn) {
        void *old_element = (void *) cstl_object_get_data(node->value);
        if (old_element) {
            pMap->root->destruct_v_fn(old_element);
        }
    }
    cstl_object_replace_raw(node->value, elem, elem_size);
}

struct cstl_iterator*
cstl_map_new_iterator(struct cstl_map* pMap) {
    struct cstl_iterator *itr = (struct cstl_iterator*)calloc(1, sizeof(struct cstl_iterator));
    itr->next = cstl_map_iter_get_next;
    itr->current_key = cstl_map_iter_get_key;
    itr->current_value = cstl_map_iter_get_value;
    itr->replace_current_value = cstl_map_iter_replace_value;
    itr->pContainer = pMap;
    itr->current_index = 0;
    itr->current_element = (void*)0;
    pMap->map_changed = cstl_false;
    return itr;
}

void
cstl_map_delete_iterator(struct cstl_iterator* pItr) {
    free(pItr);
}

void cstl_map_traverse(struct cstl_map *map, map_iter_callback cb, void *p) {
    struct cstl_iterator *iterator;
    const void *element;
    cstl_bool stop = cstl_false;
    if (map==NULL || cb==NULL) {
        return;
    }
    iterator = cstl_map_new_iterator(map);
    while( (element = iterator->next(iterator)) ) {
        const void *key = iterator->current_key(iterator);
        const void *value = iterator->current_value(iterator);
        cb(map, key, value, &stop, p);
        if (stop != cstl_false) { break; }
        if (map->map_changed) {
            cstl_map_delete_iterator(iterator);
            iterator = cstl_map_new_iterator(map);
        }
    }
    cstl_map_delete_iterator(iterator);
}
