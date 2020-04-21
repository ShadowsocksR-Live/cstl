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
add_elements_to_list(struct cstl_list* ll, int x, int y) {
    int i = 0;
    for (i = x; i <= y; i++) {
        int *v = (int *)calloc(1, sizeof(int));
        memcpy(v, &i, sizeof(int));
        cstl_list_push_back(ll, &v, sizeof(int *));
    }
}
void
print_e(const void* ptr, void *p) {
    (void)p;
    if (ptr) {
        printf("%d\n", **((int**)ptr));
    }
}

static int
compare_element(const void* left, const void* right) {
    int *l = *((int**)left);
    int *r = *((int**)right);
    return (*l - *r);
}
static void
print_using_iterators(struct cstl_list* pList) {
    struct cstl_iterator *myItr;
    const void *pElement;
    printf("------------------------------------------------\n");
    myItr = cstl_list_new_iterator(pList);
    while ((pElement = myItr->next(myItr)) != NULL) {
        const void* value = myItr->current_value(myItr);
        printf("%d\n", **((int**)value));
    }
    cstl_list_delete_iterator(myItr);
}

static void
replace_values_using_iterators(struct cstl_list* pList) {
    struct cstl_iterator *myItr;
    const void *pElement;
    printf("------------------------------------------------\n");
    myItr = cstl_list_new_iterator(pList);
    while ((pElement = myItr->next(myItr)) != NULL) {
        int *v;
        const void* old_value = myItr->current_value(myItr);
        int new_value = **((int**)old_value);
        new_value = new_value * 2;

        v = (int *)calloc(1, sizeof(int));
        *v = new_value;
        myItr->replace_current_value(myItr, &v, sizeof(int *));
    }
    cstl_list_delete_iterator(myItr);
}

static void
test_with_iterators() {
    struct cstl_list* pList = cstl_list_new(free_element, compare_element);
    add_elements_to_list(pList, 1, 10);
    print_using_iterators(pList);
    replace_values_using_iterators(pList);
    print_using_iterators(pList);
    cstl_list_destroy(pList);
}

void
test_c_slist() {
    int *tmp;
    int i = 0;
    int *v;
    const void* outValue;
    struct cstl_list* list = cstl_list_new(free_element, compare_element);

    add_elements_to_list(list, 1, 10);
    cstl_list_for_each(list, print_e, NULL);

    i = 55;
    v = (int *)calloc(1, sizeof(int));
    *v = i;
    cstl_list_insert(list, 4, &v, sizeof(int *));
    cstl_list_for_each(list, print_e, NULL);

    cstl_list_remove(list, 4);
    cstl_list_for_each(list, print_e, NULL);

    cstl_list_remove(list, 0);
    cstl_list_for_each(list, print_e, NULL);

    cstl_list_remove(list, 100);
    cstl_list_for_each(list, print_e, NULL);

    i = 1;
    v = (int *)calloc(1, sizeof(int));
    *v = i;
    cstl_list_insert(list, 0, &v, sizeof(int *));
    cstl_list_for_each(list, print_e, NULL);

    i = 11;
    v = (int *)calloc(1, sizeof(int));
    memcpy(v, &i, sizeof(int));
    cstl_list_insert(list, 10, &v, sizeof(int *));
    cstl_list_for_each(list, print_e, NULL);

    i = 12;
    v = (int *)calloc(1, sizeof(int));
    memcpy(v, &i, sizeof(int));
    cstl_list_insert(list, 200, &v, sizeof(int *));
    cstl_list_for_each(list, print_e, NULL);

    cstl_list_remove(list, cstl_list_count(list) - 1);
    cstl_list_for_each(list, print_e, NULL);

    tmp = (int *)calloc(1, sizeof(int));
    *tmp = 10;
    if ((outValue = cstl_list_find(list, &tmp))) {
        assert(*tmp == **((int**)outValue));
    }

    *tmp = 100;
    outValue = cstl_list_find(list, &tmp);
    assert(outValue == NULL);
    free(tmp);

    outValue = cstl_list_element_at(list, 7);
    assert(**((int**)outValue) == 8);

    assert(cstl_list_size(list) == 11);

    cstl_list_destroy(list);

    test_with_iterators();
}
