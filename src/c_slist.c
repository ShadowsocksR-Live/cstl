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

struct cstl_slist*
cstl_slist_new(cstl_destroy fn_d, cstl_compare fn_c) {
    struct cstl_slist* pSlist = (struct cstl_slist*)calloc(1, sizeof(struct cstl_slist));
    pSlist->head = (struct cstl_slist_node*)0;
    pSlist->destruct_fn = fn_d;
    pSlist->compare_key_fn = fn_c;
    pSlist->size = 0;
    return pSlist;
}

void
cstl_slist_delete(struct cstl_slist* pSlist) {
    while (pSlist->size != 0) {
        cstl_slist_remove(pSlist, 0);
    }
    free(pSlist);
}

cstl_error
cstl_slist_push_back(struct cstl_slist* pSlist, void* elem, size_t elem_size) {
    return cstl_slist_insert(pSlist, pSlist->size, elem, elem_size);
}

static void
__cstl_slist_remove(struct cstl_slist* pSlist, struct cstl_slist_node* pSlistNode) {
    if (pSlist->destruct_fn) {
        void *elem = (void *)cstl_object_get_data(pSlistNode->elem);
        if (elem) {
            pSlist->destruct_fn(elem);
        }
    }
    cstl_object_delete(pSlistNode->elem);

    free(pSlistNode);
}

void
cstl_slist_remove(struct cstl_slist* pSlist, int pos) {
    int i = 0;

    struct cstl_slist_node* current = pSlist->head;
    struct cstl_slist_node* previous = (struct cstl_slist_node*)0;

    if (pos > (pSlist->size - 1)) { return; }

    if (pos == 0) {
        pSlist->head = current->next;
        __cstl_slist_remove(pSlist, current);
        pSlist->size--;
        return;
    }
    for (i = 0; i < pos; ++i) {
        previous = current;
        current = current->next;
    }
    previous->next = current->next;
    __cstl_slist_remove(pSlist, current);

    pSlist->size--;
}

cstl_error
cstl_slist_insert(struct cstl_slist* pSlist, int pos, void* elem, size_t elem_size) {
    int i = 0;
    struct cstl_slist_node* current = pSlist->head;
    struct cstl_slist_node* new_node = (struct cstl_slist_node*)0;
    struct cstl_slist_node* previous = (struct cstl_slist_node*)0;

    if (pos > pSlist->size) {
        pos = pSlist->size;
    }

    new_node = (struct cstl_slist_node*)calloc(1, sizeof(struct cstl_slist_node));
    new_node->next = (struct cstl_slist_node*)0;
    new_node->elem = cstl_object_new(elem, elem_size);
    if (!new_node->elem) {
        free(new_node);
        return CSTL_SLIST_INSERT_FAILED;
    }

    if (pos == 0) {
        new_node->next = pSlist->head;
        pSlist->head = new_node;
        pSlist->size++;
        return CSTL_ERROR_SUCCESS;
    }

    for (i = 0; i < pos; ++i) {
        previous = current;
        current = current->next;
    }

    previous->next = new_node;
    new_node->next = current;
    pSlist->size++;

    return CSTL_ERROR_SUCCESS;
}

void
cstl_slist_for_each(struct cstl_slist* pSlist, void(*fn)(const void *elem, void *p), void *p) {
    struct cstl_slist_node* current = pSlist->head;
    if (fn == NULL) {
        return;
    }
    while (current != (struct cstl_slist_node*)0) {
        fn(cstl_object_get_data(current->elem), p);
        current = current->next;
    }
}

const void *
cstl_slist_find(struct cstl_slist* pSlist, void* find_value) {
    struct cstl_slist_node* current = pSlist->head;
    while (current != (struct cstl_slist_node*)0) {
        const void *tmp = cstl_object_get_data(current->elem);
        if ((pSlist->compare_key_fn)(find_value, tmp) == 0) {
            return tmp;
        }
        current = current->next;
    }
    return NULL;
}

const void * cstl_slist_element_at(struct cstl_slist* pSlist, int pos) {
    struct cstl_slist_node* current = pSlist->head;
    int index = 0;
    if (pos > pSlist->size) {
        pos = pSlist->size;
    }
    for (index=0; index<pos; ++index) {
        current = current->next;
    }
    return current ? cstl_object_get_data(current->elem) : NULL;
}

size_t cstl_slist_size(struct cstl_slist* pSlist) {
    return pSlist ? pSlist->size : 0;
}

static struct cstl_object*
cstl_slist_get_next(struct cstl_iterator* pIterator) {
    struct cstl_slist *pSlist = (struct cstl_slist*)pIterator->pContainer;
    if (!pIterator->pCurrentElement) {
        pIterator->pCurrentElement = (struct cstl_slist_node*)pSlist->head;
    } else {
        pIterator->pCurrentElement = ((struct cstl_slist_node*)pIterator->pCurrentElement)->next;
    }
    if (!pIterator->pCurrentElement) {
        return (struct cstl_object*)0;
    }
    return ((struct cstl_slist_node*)pIterator->pCurrentElement)->elem;
}

static const void*
cstl_slist_get_value(void* pObject) {
    return cstl_object_get_data((struct cstl_object*)pObject);
}

static void
cstl_slist_replace_value(struct cstl_iterator *pIterator, void* elem, size_t elem_size) {
    struct cstl_slist*  pSlist = (struct cstl_slist*)pIterator->pContainer;
    struct cstl_object *pObj = ((struct cstl_slist_node*)pIterator->pCurrentElement)->elem;

    if (pSlist->destruct_fn) {
        void *old_element = (void *) cstl_object_get_data(pObj);
        if (old_element) {
            pSlist->destruct_fn(old_element);
        }
    }
    cstl_object_replace_raw(pObj, elem, elem_size);
}

struct cstl_iterator*
cstl_slist_new_iterator(struct cstl_slist* pSlist) {
    struct cstl_iterator *itr = (struct cstl_iterator*) calloc(1, sizeof(struct cstl_iterator));
    itr->get_next = cstl_slist_get_next;
    itr->get_value = cstl_slist_get_value;
    itr->replace_value = cstl_slist_replace_value;
    itr->pContainer = pSlist;
    itr->pCurrentElement = (void*)0;
    itr->pCurrent = 0;
    return itr;
}

void
cstl_slist_delete_iterator(struct cstl_iterator* pItr) {
    free(pItr);
}
