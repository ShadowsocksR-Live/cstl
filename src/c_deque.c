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
#include <string.h>

#define cstl_deque_INDEX(x)  ((char *)(pDeq)->pElements + (sizeof(struct cstl_object) * (x)))

static cstl_error
insert_c_deque(struct cstl_deque* pDeq, int index, void* elem, size_t elem_size) {
    cstl_error rc = CSTL_ERROR_SUCCESS;
    struct cstl_object* pObject = new_cstl_object(elem, elem_size);
    if (!pObject)
        return CSTL_ARRAY_INSERT_FAILED;

    pDeq->pElements[index] = pObject;
    pDeq->no_of_elements++;
    return rc;
}

static struct cstl_deque*
grow_deque(struct cstl_deque* pDeq) {

    pDeq->no_max_elements = pDeq->no_max_elements * 2;
    pDeq->pElements = (struct cstl_object**) realloc(pDeq->pElements,
        pDeq->no_max_elements * sizeof(struct cstl_object*));
    return pDeq;
}

struct cstl_deque*
new_cstl_deque(int deq_size, cstl_compare fn_c, cstl_destroy fn_d) {

    struct cstl_deque* pDeq = (struct cstl_deque*)malloc(sizeof(struct cstl_deque));
    if (pDeq == (struct cstl_deque*)0)
        return (struct cstl_deque*)0;

    pDeq->no_max_elements = deq_size < 8 ? 8 : deq_size;
    pDeq->pElements = (struct cstl_object**) malloc(pDeq->no_max_elements * sizeof(struct cstl_object*));

    if (pDeq == (struct cstl_deque*)0)
        return (struct cstl_deque*)0;

    pDeq->compare_fn = fn_c;
    pDeq->destruct_fn = fn_d;
    pDeq->head = (int)pDeq->no_max_elements / 2;
    pDeq->tail = pDeq->head + 1;
    pDeq->no_of_elements = 0;

    return pDeq;
}

cstl_error
push_back_cstl_deque(struct cstl_deque* pDeq, void* elem, size_t elem_size) {
    if (pDeq == (struct cstl_deque*)0)
        return CSTL_DEQUE_NOT_INITIALIZED;

    if (pDeq->tail == pDeq->no_max_elements)
        pDeq = grow_deque(pDeq);

    insert_c_deque(pDeq, pDeq->tail, elem, elem_size);
    pDeq->tail++;

    return CSTL_ERROR_SUCCESS;
}

cstl_error
push_front_cstl_deque(struct cstl_deque* pDeq, void* elem, size_t elem_size) {
    cstl_error rc = CSTL_ERROR_SUCCESS;
    int to = 0;
    int from = 0;
    int count = 0;

    if (pDeq->head == 0) {
        pDeq = grow_deque(pDeq);
        to = (pDeq->no_max_elements - pDeq->no_of_elements) / 2;
        from = pDeq->head + 1;
        count = pDeq->tail - from + 1;
        memmove(&(pDeq->pElements[to]), &(pDeq->pElements[from]), count * sizeof(struct cstl_object*));
        pDeq->head = to - 1;
        pDeq->tail = pDeq->head + count;
    }
    insert_c_deque(pDeq, pDeq->head, elem, elem_size);
    pDeq->head--;
    return rc;
}

cstl_error
front_cstl_deque(struct cstl_deque* pDeq, void* elem) {
    if (pDeq == (struct cstl_deque*)0)
        return CSTL_DEQUE_NOT_INITIALIZED;
    element_at_cstl_deque(pDeq, pDeq->head + 1, elem);
    return CSTL_ERROR_SUCCESS;
}

cstl_error
back_cstl_deque(struct cstl_deque* pDeq, void* elem) {
    if (pDeq == (struct cstl_deque*)0)
        return CSTL_DEQUE_NOT_INITIALIZED;
    element_at_cstl_deque(pDeq, pDeq->tail - 1, elem);
    return CSTL_ERROR_SUCCESS;
}

cstl_error
pop_back_cstl_deque(struct cstl_deque* pDeq) {
    if (pDeq == (struct cstl_deque*)0)
        return CSTL_DEQUE_NOT_INITIALIZED;

    if (pDeq->destruct_fn) {
        void* elem;
        if (element_at_cstl_deque(pDeq, pDeq->tail - 1, &elem) == CSTL_ERROR_SUCCESS) {
            pDeq->destruct_fn(elem);
            free(elem);
        }
    }
    delete_cstl_object(pDeq->pElements[pDeq->tail - 1]);
    pDeq->tail--;
    pDeq->no_of_elements--;

    return CSTL_ERROR_SUCCESS;
}

cstl_error
pop_front_cstl_deque(struct cstl_deque* pDeq) {

    if (pDeq == (struct cstl_deque*)0)
        return CSTL_DEQUE_NOT_INITIALIZED;

    if (pDeq->destruct_fn) {
        void* elem;
        if (element_at_cstl_deque(pDeq, pDeq->head + 1, &elem) == CSTL_ERROR_SUCCESS) {
            pDeq->destruct_fn(elem);
            free(elem);
        }
    }
    delete_cstl_object(pDeq->pElements[pDeq->head + 1]);

    pDeq->head++;
    pDeq->no_of_elements--;

    return CSTL_ERROR_SUCCESS;
}

cstl_bool
empty_cstl_deque(struct cstl_deque* pDeq) {
    if (pDeq == (struct cstl_deque*)0)
        return cstl_true;

    return pDeq->no_of_elements == 0 ? cstl_true : cstl_false;
}

int
size_cstl_deque(struct cstl_deque* pDeq) {
    if (pDeq == (struct cstl_deque*)0)
        return cstl_true;

    return pDeq->no_of_elements - 1;
}

cstl_error
element_at_cstl_deque(struct cstl_deque* pDeq, int index, void**elem) {

    cstl_error rc = CSTL_ERROR_SUCCESS;

    if (!pDeq)
        return CSTL_DEQUE_NOT_INITIALIZED;

    get_raw_cstl_object(pDeq->pElements[index], elem);
    return rc;
}

cstl_error
delete_cstl_deque(struct cstl_deque* pDeq) {
    int i = 0;

    if (pDeq == (struct cstl_deque*)0)
        return CSTL_ERROR_SUCCESS;

    if (pDeq->destruct_fn) {
        for (i = pDeq->head + 1; i < pDeq->tail; i++) {
            void* elem;
            if (element_at_cstl_deque(pDeq, i, &elem) == CSTL_ERROR_SUCCESS) {
                pDeq->destruct_fn(elem);
                free(elem);
            }
        }
    }
    for (i = pDeq->head + 1; i < pDeq->tail; i++) {
        delete_cstl_object(pDeq->pElements[i]);
    }
    free(pDeq->pElements);
    free(pDeq);

    return CSTL_ERROR_SUCCESS;
}

static struct cstl_object*
get_next_c_deque(struct cstl_iterator* pIterator) {
    struct cstl_deque *pDeq = (struct cstl_deque*)pIterator->pContainer;
    int index = ((struct cstl_iterator*)pIterator)->pCurrent;

    if (index < 0 || index >= pDeq->tail) {
        return (struct cstl_object*)0;
    }
    pIterator->pCurrentElement = pDeq->pElements[pIterator->pCurrent++];
    return pIterator->pCurrentElement;
}

static void*
get_value_c_deque(void* pObject) {
    void* elem;
    get_raw_cstl_object(pObject, &elem);
    return elem;
}

static void
replace_value_c_deque(struct cstl_iterator *pIterator, void* elem, size_t elem_size) {
    struct cstl_deque*  pDeq = (struct cstl_deque*)pIterator->pContainer;
    if (pDeq->destruct_fn) {
        void* old_element;
        if (get_raw_cstl_object(pIterator->pCurrentElement, &old_element) == CSTL_ERROR_SUCCESS) {
            pDeq->destruct_fn(old_element);
            free(old_element);
        }
    }
    replace_raw_cstl_object(pIterator->pCurrentElement, elem, elem_size);
}

struct cstl_iterator*
new_iterator_cstl_deque(struct cstl_deque* pDeq) {
    struct cstl_iterator *itr = (struct cstl_iterator*) calloc(1, sizeof(struct cstl_iterator));
    itr->get_next = get_next_c_deque;
    itr->get_value = get_value_c_deque;
    itr->replace_value = replace_value_c_deque;
    itr->pCurrent = pDeq->head + 1;
    itr->pContainer = pDeq;
    return itr;
}

void
delete_iterator_cstl_deque(struct cstl_iterator* pItr) {
    free(pItr);
}
