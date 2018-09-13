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

struct cstl_set {
    struct cstl_rb* root;
};

struct cstl_set*
cstl_set_new(cstl_compare fn_c, cstl_destroy fn_d) {
    struct cstl_set* pSet = (struct cstl_set*)calloc(1, sizeof(struct cstl_set));
    if (pSet == (struct cstl_set*)0) {
        return (struct cstl_set*)0;
    }
    pSet->root = cstl_rb_new(fn_c, fn_d, (void*)0);
    if (pSet->root == (struct cstl_rb*)0) {
        return (struct cstl_set*)0;
    }
    return pSet;
}

cstl_error
cstl_set_insert(struct cstl_set* pSet, void* key, size_t key_size) {
    if (pSet == (struct cstl_set*)0) {
        return CSTL_SET_NOT_INITIALIZED;
    }
    return cstl_rb_insert(pSet->root, key, key_size, (void*)0, 0);
}

cstl_bool
cstl_set_exists(struct cstl_set* pSet, void* key) {
    cstl_bool found = cstl_false;
    struct cstl_rb_node* node;

    if (pSet == (struct cstl_set*)0) {
        return cstl_false;
    }
    node = cstl_rb_find(pSet->root, key);
    if (node != (struct cstl_rb_node*)0) {
        return cstl_true;
    }
    return found;
}

cstl_error
cstl_set_remove(struct cstl_set* pSet, void* key) {
    cstl_error rc = CSTL_ERROR_SUCCESS;
    struct cstl_rb_node* node;
    if (pSet == (struct cstl_set*)0) {
        return CSTL_SET_NOT_INITIALIZED;
    }
    node = cstl_rb_remove(pSet->root, key);
    if (node != (struct cstl_rb_node*)0) {
        if (pSet->root->destruct_k_fn) {
            void *key = (void *) cstl_object_get_data(node->key);
            if (key) {
                pSet->root->destruct_k_fn(key);
            }
        }
        cstl_object_delete(node->key);

        free(node);
    }
    return rc;
}

const void * cstl_set_find(struct cstl_set* pSet, const void* key) {
    struct cstl_rb_node* node;

    if (pSet == (struct cstl_set*)0) {
        return NULL;
    }
    node = cstl_rb_find(pSet->root, key);
    if (node == (struct cstl_rb_node*)0) {
        return NULL;
    }
    return cstl_object_get_data(node->key);
}

cstl_error
cstl_set_delete(struct cstl_set* x) {
    cstl_error rc = CSTL_ERROR_SUCCESS;
    if (x != (struct cstl_set*)0) {
        rc = cstl_rb_delete(x->root);
        free(x);
    }
    return rc;
}

static struct cstl_rb_node *
cstl_set_minimum(struct cstl_set *x) {
    return cstl_rb_minimum(x->root, x->root->root);
}

static const void *
cstl_set_get_next(struct cstl_iterator* pIterator) {
    struct cstl_set *x = (struct cstl_set*)pIterator->pContainer;
    struct cstl_rb_node *ptr = NULL;
    if (!pIterator->current_element) {
        pIterator->current_element = cstl_set_minimum(x);
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
cstl_set_get_key(struct cstl_iterator* pIterator) {
    struct cstl_rb_node *current = (struct cstl_rb_node*)pIterator->current_element;
    return cstl_object_get_data(current->key);
}

static const void*
cstl_set_get_value(struct cstl_iterator *pIterator) {
    return cstl_set_get_key(pIterator);
}

struct cstl_iterator*
cstl_set_new_iterator(struct cstl_set* pSet) {
    struct cstl_iterator *itr = (struct cstl_iterator*) calloc(1, sizeof(struct cstl_iterator));
    itr->next = cstl_set_get_next;
    itr->current_key = cstl_set_get_key;
    itr->current_value = cstl_set_get_value;
    itr->pContainer = pSet;
    itr->current_index = 0;
    itr->current_element = (void*)0;
    return itr;
}

void
cstl_set_delete_iterator(struct cstl_iterator* pItr) {
    free(pItr);
}
