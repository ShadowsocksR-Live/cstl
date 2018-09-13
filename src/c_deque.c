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
#include <string.h>

struct cstl_deque {
    struct cstl_object**pElements;
    size_t capacity;
    size_t count;
    size_t head;
    size_t tail;
    cstl_compare compare_fn;
    cstl_destroy destruct_fn;
};

#define cstl_deque_INDEX(x)  ((char *)(pDeq)->pElements + (sizeof(struct cstl_object) * (x)))

static cstl_error
cstl_deque_insert(struct cstl_deque* pDeq, size_t index, void* elem, size_t elem_size) {
    cstl_error rc = CSTL_ERROR_SUCCESS;
    struct cstl_object* pObject = cstl_object_new(elem, elem_size);
    if (!pObject) {
        return CSTL_ARRAY_INSERT_FAILED;
    }
    pDeq->pElements[index] = pObject;
    pDeq->count++;
    return rc;
}

static struct cstl_deque*
cstl_deque_grow(struct cstl_deque* pDeq) {
    size_t size;
    pDeq->capacity = pDeq->capacity * 2;
    size = pDeq->capacity * sizeof(struct cstl_object*);
    pDeq->pElements = (struct cstl_object**) realloc(pDeq->pElements, size);
    return pDeq;
}

struct cstl_deque*
cstl_deque_new(size_t deq_size, cstl_compare fn_c, cstl_destroy fn_d) {
    struct cstl_deque* pDeq = (struct cstl_deque*)calloc(1, sizeof(struct cstl_deque));
    if (pDeq == (struct cstl_deque*)0) {
        return (struct cstl_deque*)0;
    }
    pDeq->capacity = deq_size < 8 ? 8 : deq_size;
    pDeq->pElements = (struct cstl_object**) calloc(pDeq->capacity, sizeof(struct cstl_object*));

    if (pDeq == (struct cstl_deque*)0) {
        return (struct cstl_deque*)0;
    }
    pDeq->compare_fn = fn_c;
    pDeq->destruct_fn = fn_d;
    pDeq->head = pDeq->capacity / 2;
    pDeq->tail = pDeq->head + 1;
    pDeq->count = 0;

    return pDeq;
}

size_t cstl_deque_count(struct cstl_deque *deque) {
    return deque->count;
}

cstl_error
cstl_deque_push_back(struct cstl_deque* pDeq, void* elem, size_t elem_size) {
    if (pDeq == (struct cstl_deque*)0) {
        return CSTL_DEQUE_NOT_INITIALIZED;
    }
    if (pDeq->tail == pDeq->capacity) {
        pDeq = cstl_deque_grow(pDeq);
    }
    cstl_deque_insert(pDeq, pDeq->tail, elem, elem_size);
    pDeq->tail++;

    return CSTL_ERROR_SUCCESS;
}

cstl_error
cstl_deque_push_front(struct cstl_deque* pDeq, void* elem, size_t elem_size) {
    cstl_error rc = CSTL_ERROR_SUCCESS;
    size_t to = 0;
    size_t from = 0;
    size_t count = 0;

    if (pDeq->head == 0) {
        pDeq = cstl_deque_grow(pDeq);
        to = (pDeq->capacity - pDeq->count) / 2;
        from = pDeq->head + 1;
        count = pDeq->tail - from;
        memmove(&(pDeq->pElements[to]), &(pDeq->pElements[from]), count * sizeof(struct cstl_object*));
        pDeq->head = to - 1;
        pDeq->tail = pDeq->head + count + 1;
    }
    cstl_deque_insert(pDeq, pDeq->head, elem, elem_size);
    pDeq->head--;
    return rc;
}

const void * cstl_deque_front(struct cstl_deque* pDeq) {
    if (pDeq) {
        return cstl_deque_element_at(pDeq, 0);
    }
    return (struct cstl_deque*)0;
}

const void * cstl_deque_back(struct cstl_deque* pDeq) {
    if (pDeq) {
        return cstl_deque_element_at(pDeq, pDeq->count - 1);
    }
    return (struct cstl_deque*)0;
}

cstl_error
cstl_deque_pop_back(struct cstl_deque* pDeq) {
    if (pDeq == (struct cstl_deque*)0) {
        return CSTL_DEQUE_NOT_INITIALIZED;
    }
    if (pDeq->destruct_fn) {
        void *elem = (void *) cstl_deque_element_at(pDeq, pDeq->count - 1);
        if ( elem ) {
            pDeq->destruct_fn(elem);
        }
    }
    cstl_object_delete(pDeq->pElements[pDeq->tail - 1]);
    pDeq->tail--;
    pDeq->count--;

    return CSTL_ERROR_SUCCESS;
}

cstl_error
cstl_deque_pop_front(struct cstl_deque* pDeq) {
    if (pDeq == (struct cstl_deque*)0) {
        return CSTL_DEQUE_NOT_INITIALIZED;
    }
    if (pDeq->destruct_fn) {
        void *elem = (void *) cstl_deque_element_at(pDeq, 0);
        if ( elem ) {
            pDeq->destruct_fn(elem);
        }
    }
    cstl_object_delete(pDeq->pElements[pDeq->head + 1]);

    pDeq->head++;
    pDeq->count--;

    return CSTL_ERROR_SUCCESS;
}

cstl_bool
cstl_deque_empty(struct cstl_deque* pDeq) {
    if (pDeq == (struct cstl_deque*)0) {
        return cstl_true;
    }
    return pDeq->count == 0 ? cstl_true : cstl_false;
}

size_t
cstl_deque_size(struct cstl_deque* pDeq) {
    if (pDeq == (struct cstl_deque*)0) {
        return cstl_true;
    }
    return pDeq->count;
}

const void *
cstl_deque_element_at(struct cstl_deque* pDeq, size_t index) {
    if ((pDeq==NULL) || (index >= pDeq->count)) {
        return NULL;
    }
    return cstl_object_get_data(pDeq->pElements[(pDeq->head + 1) + index]);
}

cstl_error
cstl_deque_delete(struct cstl_deque* pDeq) {
    size_t i = 0;

    if (pDeq == (struct cstl_deque*)0) {
        return CSTL_ERROR_SUCCESS;
    }
    if (pDeq->destruct_fn) {
        for (i = 0; i < pDeq->count; ++i) {
            void *elem = (void *) cstl_deque_element_at(pDeq, i);
            if ( elem ) {
                pDeq->destruct_fn(elem);
            }
        }
    }
    for (i = pDeq->head + 1; i < pDeq->tail; i++) {
        cstl_object_delete(pDeq->pElements[i]);
    }
    free(pDeq->pElements);
    free(pDeq);

    return CSTL_ERROR_SUCCESS;
}

static const void *
cstl_deque_get_next(struct cstl_iterator* pIterator) {
    struct cstl_deque *pDeq = (struct cstl_deque*)pIterator->pContainer;
    size_t index = pIterator->current_index;

    if (index <= pDeq->head || index >= pDeq->tail) {
        return (const void *)0;
    }
    pIterator->current_element = pDeq->pElements[pIterator->current_index++];
    return pIterator->current_element;
}

static const void*
cstl_deque_get_value(struct cstl_iterator *pIterator) {
    struct cstl_object *element = (struct cstl_object *)pIterator->current_element;
    return cstl_object_get_data(element);
}

static void
cstl_deque_replace_value(struct cstl_iterator *pIterator, void* elem, size_t elem_size) {
    struct cstl_deque*  pDeq = (struct cstl_deque*)pIterator->pContainer;
    struct cstl_object *currentElement = (struct cstl_object *)pIterator->current_element;
    if (pDeq->destruct_fn) {
        void *old_element = (void *) cstl_object_get_data(currentElement);
        if (old_element) {
            pDeq->destruct_fn(old_element);
        }
    }
    cstl_object_replace_raw(currentElement, elem, elem_size);
}

struct cstl_iterator*
cstl_deque_new_iterator(struct cstl_deque* pDeq) {
    struct cstl_iterator *itr = (struct cstl_iterator*) calloc(1, sizeof(struct cstl_iterator));
    itr->next = cstl_deque_get_next;
    itr->current_value = cstl_deque_get_value;
    itr->replace_current_value = cstl_deque_replace_value;
    itr->current_index = pDeq->head + 1;
    itr->pContainer = pDeq;
    return itr;
}

void
cstl_deque_delete_iterator(struct cstl_iterator* pItr) {
    free(pItr);
}
