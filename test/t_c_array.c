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

static int
compare_e(const void* left, const void* right) {
    int *l = (int*)left;
    int *r = (int*)right;
    return (*l - *r);
}

static int
compare_e_ptr(const void* left, const void* right) {
    int *l = *((int**)left);
    int *r = *((int**)right);
    return (*l - *r);
}

static int
compare_e_str(const void* left, const void* right) {
    char *l = *((char**)left);
    char *r = *((char**)right);
    return strcmp(l, r);
}

static void
free_e(void* ptr) {
    if (ptr) {
        void *p = *((void **)ptr);
        if (p) {
            free(p);
        }
    }
}

static void print_e(int *ptr) {
    if (ptr) {
        printf("%d\n", *(int*)ptr);
    }
}

static void
test_with_int() {
    size_t size = 10;
    int i = 0;
    int *p_rv = (int *)0;
    int rv = 0;
    struct cstl_array* myArray = cstl_array_new(8, compare_e, NULL);
    assert(cstl_true == cstl_array_empty(myArray));

    for (i = 0; i < (int)size; i++) {
        cstl_array_push_back(myArray, &i, sizeof(int));
    }
    assert(cstl_false == cstl_array_empty(myArray));
    assert(size == cstl_array_size(myArray));
    for (i = 0; i < (int)size; i++) {
        p_rv = (int *) cstl_array_element_at(myArray, i);
        rv = *(int*)p_rv;
        assert(rv == i);
    }
    p_rv = (int *) cstl_array_front(myArray);
    rv = *(int*)p_rv;
    assert(rv == 0);

    p_rv = (int *) cstl_array_back(myArray);
    rv = *(int*)p_rv;
    assert(rv == (int)(size - 1));

    cstl_array_remove_from(myArray, 0);
    assert(size - 1 == cstl_array_size(myArray));
    p_rv = (int *) cstl_array_element_at(myArray, 0);
    rv = *(int*)p_rv;
    assert(rv == 1);

    size = cstl_array_size(myArray);
    cstl_array_remove_from(myArray, size / 2);
    assert(size - 1 == cstl_array_size(myArray));
    p_rv = (int *) cstl_array_element_at(myArray, size / 2);
    rv = *(int*)p_rv;
    assert((size_t)rv == size / 2 + 2);

    size = cstl_array_size(myArray);
    cstl_array_remove_from(myArray, size - 1);
    assert(size - 1 == cstl_array_size(myArray));
    size = cstl_array_size(myArray);
    p_rv = (int *) cstl_array_element_at(myArray, size - 1);
    rv = *(int*)p_rv;
    assert(rv == 8);

    i = 900;
    cstl_array_insert_at(myArray, 5, &i, sizeof(int));
    p_rv = (int *) cstl_array_element_at(myArray, 5);
    rv = *(int*)p_rv;
    assert(rv == i);

    p_rv = (int *) cstl_array_element_at(myArray, 6);
    rv = *(int*)p_rv;
    assert(rv == 7);

    for (i = 0; i < (int) cstl_array_size(myArray); i++) {
        p_rv = (int *) cstl_array_element_at(myArray, (size_t)i);
        print_e(p_rv);
    }

    cstl_array_delete(myArray);
}

static void
test_with_pointers() {
    size_t size = 10;
    int i = 0;
    int *rv;
    int **p_rv = (int **)0;
    struct cstl_array* myArray = cstl_array_new(8, compare_e_ptr, free_e);
    assert(cstl_true == cstl_array_empty(myArray));

    for (i = 0; i < (int)size; i++) {
        int *v = (int*)calloc(1, sizeof(int));
        *v = i;
        cstl_array_push_back(myArray, &v, sizeof(int*));
    }
    assert(cstl_false == cstl_array_empty(myArray));
    assert(size == cstl_array_size(myArray));
    for (i = 0; i < (int)size; i++) {
        p_rv = (int **) cstl_array_element_at(myArray, i);
        rv = *((int**)p_rv);
        assert(*rv == i);
    }
    p_rv = (int **) cstl_array_front(myArray);
    rv = *((int**)p_rv);
    assert(*rv == 0);

    p_rv = (int **) cstl_array_back(myArray);
    rv = *((int**)p_rv);
    assert(*rv == (int)size - 1);

    cstl_array_remove_from(myArray, 0);
    assert(size - 1 == cstl_array_size(myArray));

    p_rv = (int **) cstl_array_element_at(myArray, 0);
    rv = *((int**)p_rv);
    assert(*rv == 1);

    size = cstl_array_size(myArray);
    cstl_array_remove_from(myArray, size / 2);
    assert(size - 1 == cstl_array_size(myArray));
    p_rv = (int **) cstl_array_element_at(myArray, size / 2);
    rv = *((int**)p_rv);
    assert((size_t)*rv == size / 2 + 2);

    size = cstl_array_size(myArray);
    cstl_array_remove_from(myArray, size - 1);
    assert(size - 1 == cstl_array_size(myArray));

    size = cstl_array_size(myArray);

    p_rv = (int **) cstl_array_element_at(myArray, size - 1);
    rv = *((int**)p_rv);
    assert(*rv == 8);

    cstl_array_delete(myArray);
}

static void
test_with_strings() {
    size_t size = 10;
    char *input_array[11];
    int i = 0;
    char *rv;
    char **p_rv = (char **)0;
    struct cstl_array* myArray = cstl_array_new(8, compare_e_str, free_e);
    assert(cstl_true == cstl_array_empty(myArray));

    input_array[0] = "STRING_0";
    input_array[1] = "STRING_1";
    input_array[2] = "STRING_2";
    input_array[3] = "STRING_3";
    input_array[4] = "STRING_4";
    input_array[5] = "STRING_5";
    input_array[6] = "STRING_6";
    input_array[7] = "STRING_7";
    input_array[8] = "STRING_8";
    input_array[9] = "STRING_9";
    input_array[10] = "STRING_10";

    for (i = 0; i < (int)size; i++) {
        char *v = cstl_strdup(input_array[i]);
        cstl_array_push_back(myArray, &v, sizeof(char *));
    }
    assert(cstl_false == cstl_array_empty(myArray));
    assert(size == cstl_array_size(myArray));
    for (i = 0; i < (int)size; i++) {
        p_rv = (char **) cstl_array_element_at(myArray, i);
        rv = *((char**)p_rv);
        assert(strcmp(rv, input_array[i]) == 0);
    }
    p_rv = (char **) cstl_array_front(myArray);
    rv = *((char**)p_rv);
    assert(strcmp(rv, input_array[0]) == 0);

    p_rv = (char **) cstl_array_back(myArray);
    rv = *((char**)p_rv);
    assert(strcmp(rv, input_array[size - 1]) == 0);

    cstl_array_remove_from(myArray, 0);
    assert(size - 1 == cstl_array_size(myArray));

    p_rv = (char **) cstl_array_element_at(myArray, 0);
    rv = *((char**)p_rv);
    assert(strcmp(rv, input_array[1]) == 0);

    size = cstl_array_size(myArray);
    cstl_array_remove_from(myArray, size / 2);

    p_rv = (char **) cstl_array_element_at(myArray, size / 2);
    rv = *((char**)p_rv);
    assert(strcmp(rv, input_array[size / 2 + 2]) == 0);

    size = cstl_array_size(myArray);
    cstl_array_remove_from(myArray, size - 1);
    assert(size - 1 == cstl_array_size(myArray));
    size = cstl_array_size(myArray);

    p_rv = (char **) cstl_array_element_at(myArray, size - 1);
    rv = *((char**)p_rv);
    assert(strcmp(rv, input_array[8]) == 0);

    cstl_array_delete(myArray);
}

static void
print_using_iterators(struct cstl_array* myArray) {
    struct cstl_iterator *myItr;
    const void *pElement;
    printf("------------------------------------------------\n");
    myItr = cstl_array_new_iterator(myArray);
    while ((pElement = myItr->next(myItr)) != NULL) {
        int *value = (int *)myItr->current_value(myItr);
        printf("%d\n", *value);
    }
    cstl_array_delete_iterator(myItr);
}

static void
replace_values_using_iterators(struct cstl_array* myArray) {
    struct cstl_iterator *myItr;
    const void *pElement;
    printf("------------------------------------------------\n");
    myItr = cstl_array_new_iterator(myArray);
    while ((pElement = myItr->next(myItr)) != NULL) {
        int *old_value = (int *)myItr->current_value(myItr);
        int new_value = *old_value;
        new_value = new_value * 2;
        myItr->replace_current_value(myItr, &new_value, sizeof(new_value));
    }
    cstl_array_delete_iterator(myItr);
}

static struct cstl_array*
create_array() {
    int size = 10;
    int i = 0;
    int *p_rv = (int*)0;
    int rv = 0;

    struct cstl_array* myArray = cstl_array_new(8, compare_e, NULL);
    assert(cstl_true == cstl_array_empty(myArray));

    for (i = 0; i < size; i++) {
        cstl_array_push_back(myArray, &i, sizeof(int));
    }
    assert(cstl_false == cstl_array_empty(myArray));
    assert((size_t)size == cstl_array_size(myArray));
    for (i = 0; i < size; i++) {
        p_rv = (int*) cstl_array_element_at(myArray, i);
        rv = *(int*)p_rv;
        assert(rv == i);
    }
    return myArray;
}

void test_with_iterator_function() {
    struct cstl_array* myArray = create_array();
    print_using_iterators(myArray);
    replace_values_using_iterators(myArray);
    print_using_iterators(myArray);
    cstl_array_delete(myArray);
}

void
test_c_array() {
    test_with_int();
    test_with_pointers();
    test_with_strings();
    test_with_iterator_function();
}
