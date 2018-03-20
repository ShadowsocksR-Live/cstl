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
        push_back_cstl_slist(ll, &v, sizeof(int *));
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
    myItr = new_iterator_cstl_slist(pList);
    pElement = myItr->get_next(myItr);
    while (pElement) {
        void* value = myItr->get_value(pElement);
        printf("%d\n", **((int**)value));
        free(value);
        pElement = myItr->get_next(myItr);
    }
    delete_iterator_cstl_slist(myItr);
}

static void
replace_values_using_iterators(struct cstl_slist* pList) {
    struct cstl_iterator *myItr;
    struct cstl_object *pElement;
    printf("------------------------------------------------\n");
    myItr = new_iterator_cstl_slist(pList);
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
    delete_iterator_cstl_slist(myItr);
}

static void
test_with_iterators() {
    struct cstl_slist* pList = new_cstl_slist(free_element, compare_element);
    add_elements_to_list(pList, 1, 10);
    print_using_iterators(pList);
    replace_values_using_iterators(pList);
    print_using_iterators(pList);
    delete_cstl_slist(pList);
}

void
test_c_slist() {
    int i = 0;
    int *v;
    void* outValue;
    struct cstl_slist* list = new_cstl_slist(free_element, compare_element);

    add_elements_to_list(list, 1, 10);
    for_each_cstl_slist(list, print_e);

    i = 55;
    v = (int *)calloc(1, sizeof(int));
    memcpy(v, &i, sizeof(int));
    insert_cstl_slist(list, 5, &v, sizeof(int *));
    for_each_cstl_slist(list, print_e);

    remove_cstl_slist(list, 5);
    for_each_cstl_slist(list, print_e);

    remove_cstl_slist(list, 0);
    for_each_cstl_slist(list, print_e);

    remove_cstl_slist(list, 100);
    for_each_cstl_slist(list, print_e);

    i = 1;
    v = (int *)calloc(1, sizeof(int));
    memcpy(v, &i, sizeof(int));
    insert_cstl_slist(list, 1, &v, sizeof(int *));
    for_each_cstl_slist(list, print_e);

    i = 11;
    v = (int *)calloc(1, sizeof(int));
    memcpy(v, &i, sizeof(int));
    insert_cstl_slist(list, 11, &v, sizeof(int *));
    for_each_cstl_slist(list, print_e);

    i = 12;
    v = (int *)calloc(1, sizeof(int));
    memcpy(v, &i, sizeof(int));
    insert_cstl_slist(list, 200, &v, sizeof(int *));
    for_each_cstl_slist(list, print_e);

    remove_cstl_slist(list, list->size);
    for_each_cstl_slist(list, print_e);

    int *tmp = (int *)calloc(1, sizeof(int));
    *tmp = 10;
    if (cstl_true == find_cstl_slist(list, &tmp, &outValue)) {
        assert(*tmp == **((int**)outValue));
        free(outValue);
    }

    *tmp = 100;
    assert(cstl_false == find_cstl_slist(list, &tmp, &outValue));
    free(tmp);

    delete_cstl_slist(list);

    test_with_iterators();
}
