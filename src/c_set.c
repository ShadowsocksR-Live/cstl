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
#include <stdio.h>

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

cstl_bool
cstl_set_find(struct cstl_set* pSet, void* key, const void** outKey) {
    struct cstl_rb_node* node;

    if (pSet == (struct cstl_set*)0) {
        return cstl_false;
    }
    node = cstl_rb_find(pSet->root, key);
    if (node == (struct cstl_rb_node*)0) {
        return cstl_false;
    }
    if (outKey) {
        *outKey = cstl_object_get_data(node->key);
    }
    return cstl_true;
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

static struct cstl_object*
cstl_set_get_next(struct cstl_iterator* pIterator) {
    if (!pIterator->pCurrentElement) {
        pIterator->pCurrentElement = cstl_set_minimum(pIterator->pContainer);
    } else {
        struct cstl_set *x = (struct cstl_set*)pIterator->pContainer;
        pIterator->pCurrentElement = cstl_rb_tree_successor(x->root, pIterator->pCurrentElement);
    }
    if (!pIterator->pCurrentElement) {
        return (struct cstl_object*)0;
    }
    return ((struct cstl_rb_node*)pIterator->pCurrentElement)->key;
}

static const void*
cstl_set_get_value(void* pObject) {
    return cstl_object_get_data((struct cstl_object*)pObject);
}

struct cstl_iterator*
cstl_set_new_iterator(struct cstl_set* pSet) {
    struct cstl_iterator *itr = (struct cstl_iterator*) calloc(1, sizeof(struct cstl_iterator));
    itr->get_next = cstl_set_get_next;
    itr->get_value = cstl_set_get_value;
    itr->pContainer = pSet;
    itr->pCurrent = 0;
    itr->pCurrentElement = (void*)0;
    return itr;
}

void
cstl_set_delete_iterator(struct cstl_iterator* pItr) {
    free(pItr);
}
