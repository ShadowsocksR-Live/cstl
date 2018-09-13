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

struct cstl_list_node {
    struct cstl_object* elem;
    struct cstl_list_node *next;
};

struct cstl_list {
    struct cstl_list_node* head;
    cstl_destroy destruct_fn;
    cstl_compare compare_key_fn;
    size_t size;
};

struct cstl_list*
cstl_list_new(cstl_destroy fn_d, cstl_compare fn_c) {
    struct cstl_list* pList = (struct cstl_list*)calloc(1, sizeof(struct cstl_list));
    pList->head = (struct cstl_list_node*)0;
    pList->destruct_fn = fn_d;
    pList->compare_key_fn = fn_c;
    pList->size = 0;
    return pList;
}

size_t cstl_list_count(struct cstl_list* pList) {
    return pList->size;
}

void
cstl_list_destroy(struct cstl_list* pList) {
    cstl_list_clear(pList);
    free(pList);
}

void cstl_list_clear(struct cstl_list* pList) {
    while (pList->size != 0) {
        cstl_list_remove(pList, 0);
    }
}

cstl_error
cstl_list_push_back(struct cstl_list* pList, void* elem, size_t elem_size) {
    return cstl_list_insert(pList, pList->size, elem, elem_size);
}

static void
__cstl_slist_remove(struct cstl_list* pList, struct cstl_list_node* pSlistNode) {
    if (pList->destruct_fn) {
        void *elem = (void *)cstl_object_get_data(pSlistNode->elem);
        if (elem) {
            pList->destruct_fn(elem);
        }
    }
    cstl_object_delete(pSlistNode->elem);

    free(pSlistNode);
}

void
cstl_list_remove(struct cstl_list* pList, size_t pos) {
    size_t i = 0;

    struct cstl_list_node* current = pList->head;
    struct cstl_list_node* previous = (struct cstl_list_node*)0;

    if (pos >= pList->size) { return; }

    if (pos == 0) {
        pList->head = current->next;
        __cstl_slist_remove(pList, current);
        pList->size--;
        return;
    }
    for (i = 0; i < pos; ++i) {
        previous = current;
        current = current->next;
    }
    previous->next = current->next;
    __cstl_slist_remove(pList, current);

    pList->size--;
}

cstl_error
cstl_list_insert(struct cstl_list* pList, size_t pos, void* elem, size_t elem_size) {
    size_t i = 0;
    struct cstl_list_node* current = pList->head;
    struct cstl_list_node* new_node = (struct cstl_list_node*)0;
    struct cstl_list_node* previous = (struct cstl_list_node*)0;

    if (pos > pList->size) {
        pos = pList->size;
    }

    new_node = (struct cstl_list_node*)calloc(1, sizeof(struct cstl_list_node));
    new_node->next = (struct cstl_list_node*)0;
    new_node->elem = cstl_object_new(elem, elem_size);
    if (!new_node->elem) {
        free(new_node);
        return CSTL_SLIST_INSERT_FAILED;
    }

    if (pos == 0) {
        new_node->next = pList->head;
        pList->head = new_node;
        pList->size++;
        return CSTL_ERROR_SUCCESS;
    }

    for (i = 0; i < pos; ++i) {
        previous = current;
        current = current->next;
    }

    previous->next = new_node;
    new_node->next = current;
    pList->size++;

    return CSTL_ERROR_SUCCESS;
}

void
cstl_list_for_each(struct cstl_list* pList, void(*fn)(const void *elem, void *p), void *p) {
    struct cstl_list_node* current = pList->head;
    if (fn == NULL) {
        return;
    }
    while (current != (struct cstl_list_node*)0) {
        fn(cstl_object_get_data(current->elem), p);
        current = current->next;
    }
}

const void *
cstl_list_find(struct cstl_list* pList, void* find_value) {
    struct cstl_list_node* current = pList->head;
    while (current != (struct cstl_list_node*)0) {
        const void *tmp = cstl_object_get_data(current->elem);
        if (pList->compare_key_fn(find_value, tmp) == 0) {
            return tmp;
        }
        current = current->next;
    }
    return NULL;
}

const void * cstl_list_element_at(struct cstl_list* pList, size_t pos) {
    struct cstl_list_node* current = NULL;
    size_t index = 0;
    if (pList==NULL || pList->size==0) {
        return NULL;
    }
    if (pos >= pList->size) {
        pos = (pList->size - 1);
    }
    current = pList->head;
    for (index=0; index<pos; ++index) {
        current = current->next;
    }
    return current ? cstl_object_get_data(current->elem) : NULL;
}

size_t cstl_list_size(struct cstl_list* pList) {
    return pList ? pList->size : 0;
}

static const void *
cstl_list_get_next(struct cstl_iterator* pIterator) {
    struct cstl_list *pList = (struct cstl_list*)pIterator->pContainer;
    if (!pIterator->current_element) {
        pIterator->current_element = (struct cstl_list_node*)pList->head;
    } else {
        pIterator->current_element = ((struct cstl_list_node*)pIterator->current_element)->next;
    }
    return pIterator->current_element;
}

static const void*
cstl_list_get_value(struct cstl_iterator *pIterator) {
    struct cstl_object *pObj = ((struct cstl_list_node*)pIterator->current_element)->elem;
    return cstl_object_get_data(pObj);
}

static void
cstl_list_replace_value(struct cstl_iterator *pIterator, void* elem, size_t elem_size) {
    struct cstl_list*  pList = (struct cstl_list*)pIterator->pContainer;
    struct cstl_object *pObj = ((struct cstl_list_node*)pIterator->current_element)->elem;

    if (pList->destruct_fn) {
        void *old_element = (void *) cstl_object_get_data(pObj);
        if (old_element) {
            pList->destruct_fn(old_element);
        }
    }
    cstl_object_replace_raw(pObj, elem, elem_size);
}

struct cstl_iterator*
cstl_list_new_iterator(struct cstl_list* pList) {
    struct cstl_iterator *itr = (struct cstl_iterator*) calloc(1, sizeof(struct cstl_iterator));
    itr->next = cstl_list_get_next;
    itr->current_value = cstl_list_get_value;
    itr->replace_current_value = cstl_list_replace_value;
    itr->pContainer = pList;
    itr->current_element = (void*)0;
    itr->current_index = 0;
    return itr;
}

void
cstl_list_delete_iterator(struct cstl_iterator* pItr) {
    free(pItr);
}
