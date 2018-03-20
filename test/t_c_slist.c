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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

static void
free_element(void* ptr) {
    if (ptr) {
        void *p = *((void **)ptr);
        if (p) {
            free(p);
        }
    }
}

void
add_elements_to_list(struct cstl_slist* ll, int x, int y) {
    int i = 0;
    for (i = x; i <= y; i++) {
        int *v = (int *)calloc(1, sizeof(int));
        memcpy(v, &i, sizeof(int));
        cstl_slist_push_back(ll, &v, sizeof(int *));
    }
}
void
print_e(void* ptr) {
    if (ptr) {
        printf("%d\n", **((int**)ptr));
    }
}

static int
compare_element(void* left, void* right) {
    int *l = *((int**)left);
    int *r = *((int**)right);
    return *l == *r;
}
static void
print_using_iterators(struct cstl_slist* pList) {
    struct cstl_iterator *myItr;
    struct cstl_object *pElement;
    printf("------------------------------------------------\n");
    myItr = cstl_slist_new_iterator(pList);
    pElement = myItr->get_next(myItr);
    while (pElement) {
        void* value = myItr->get_value(pElement);
        printf("%d\n", **((int**)value));
        free(value);
        pElement = myItr->get_next(myItr);
    }
    cstl_slist_delete_iterator(myItr);
}

static void
replace_values_using_iterators(struct cstl_slist* pList) {
    struct cstl_iterator *myItr;
    struct cstl_object *pElement;
    printf("------------------------------------------------\n");
    myItr = cstl_slist_new_iterator(pList);
    pElement = myItr->get_next(myItr);
    while (pElement) {
        void* old_value = myItr->get_value(pElement);
        int new_value = **((int**)old_value);
        new_value = new_value * 2;

        int *v = (int *)calloc(1, sizeof(int));
        *v = new_value;
        myItr->replace_value(myItr, &v, sizeof(int *));
        free(old_value);

        pElement = myItr->get_next(myItr);
    }
    cstl_slist_delete_iterator(myItr);
}

static void
test_with_iterators() {
    struct cstl_slist* pList = cstl_slist_new(free_element, compare_element);
    add_elements_to_list(pList, 1, 10);
    print_using_iterators(pList);
    replace_values_using_iterators(pList);
    print_using_iterators(pList);
    cstl_slist_delete(pList);
}

void
test_c_slist() {
    int i = 0;
    int *v;
    void* outValue;
    struct cstl_slist* list = cstl_slist_new(free_element, compare_element);

    add_elements_to_list(list, 1, 10);
    cstl_slist_for_each(list, print_e);

    i = 55;
    v = (int *)calloc(1, sizeof(int));
    memcpy(v, &i, sizeof(int));
    cstl_slist_insert(list, 5, &v, sizeof(int *));
    cstl_slist_for_each(list, print_e);

    cstl_slist_remove(list, 5);
    cstl_slist_for_each(list, print_e);

    cstl_slist_remove(list, 0);
    cstl_slist_for_each(list, print_e);

    cstl_slist_remove(list, 100);
    cstl_slist_for_each(list, print_e);

    i = 1;
    v = (int *)calloc(1, sizeof(int));
    memcpy(v, &i, sizeof(int));
    cstl_slist_insert(list, 1, &v, sizeof(int *));
    cstl_slist_for_each(list, print_e);

    i = 11;
    v = (int *)calloc(1, sizeof(int));
    memcpy(v, &i, sizeof(int));
    cstl_slist_insert(list, 11, &v, sizeof(int *));
    cstl_slist_for_each(list, print_e);

    i = 12;
    v = (int *)calloc(1, sizeof(int));
    memcpy(v, &i, sizeof(int));
    cstl_slist_insert(list, 200, &v, sizeof(int *));
    cstl_slist_for_each(list, print_e);

    cstl_slist_remove(list, list->size);
    cstl_slist_for_each(list, print_e);

    int *tmp = (int *)calloc(1, sizeof(int));
    *tmp = 10;
    if (cstl_true == cstl_slist_find(list, &tmp, &outValue)) {
        assert(*tmp == **((int**)outValue));
        free(outValue);
    }

    *tmp = 100;
    assert(cstl_false == cstl_slist_find(list, &tmp, &outValue));
    free(tmp);

    cstl_slist_delete(list);

    test_with_iterators();
}
