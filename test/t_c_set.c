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
delete_e(void* ptr) {
    if (ptr) {
        char *str = *((char **)ptr);
        if (str) {
            free(str);
        }
    }
}
static int
compare_e(const void* left, const void* right) {
    char *l = *((char**)left);
    char *r = *((char**)right);
    return strcmp((const char *)l, (const char *)r);
}
static int
compare_int(const void* left, const void* right) {
    int *l = (int*)left;
    int *r = (int*)right;

    return (*l - *r);
}

static void
test_with_iterators() {
    int test[] = { 13,8,17,1,11,15,25,6,22,27 };
    int	index = 0;
    int size = sizeof(test) / sizeof(test[0]);
    struct cstl_iterator *myItr;
    const void *pElement;

    struct cstl_set* pSet = cstl_set_new(compare_int, NULL);

    for (index = 0; index < size; index++) {
        int v = test[index];
        cstl_set_insert(pSet, &v, sizeof(int));
    }
    for (index = 0; index < size; index++) {
        int v = test[index];
        assert(cstl_true == cstl_set_exists(pSet, &v));
    }
    printf("------------------------------------------------\n");
    myItr = cstl_set_new_iterator(pSet);
    while ((pElement = myItr->next(myItr)) != NULL) {
        const void* value = myItr->current_value(myItr);
        printf("%d\n", *(int*)value);
    }
    cstl_set_delete_iterator(myItr);
    cstl_set_delete(pSet);
}

void
test_c_set() {
    {
        int test[] = { 0,1,2,3,4,5,6,7,8,9 };
        int index = 0;
        int size = sizeof(test) / sizeof(test[0]);
        const void* outKey = NULL;

        struct cstl_set* pSet = cstl_set_new(compare_int, NULL);

        for (index = 0; index < size; index++) {
            int v = test[index];
            cstl_set_insert(pSet, &v, sizeof(int));
        }
        for (index = 0; index < size; index++) {
            int v = test[index];
            assert(cstl_true == cstl_set_exists(pSet, &v));
        }

        index = 9;
        outKey = cstl_set_find(pSet, &index);
        assert(9 == *(int*)outKey);
        cstl_set_delete(pSet);
    }
    {
        typedef struct test {
            char *string;
        } TEST_INPUT;

        int index = 0;
        int size = 0;
        char *v;

        TEST_INPUT ti[] = {
            { "A for APPLE" },{ "B for BALL" },{ "C for CAT" },{ "D for DOG" },
            { "E for ELEPHANT" },{ "F for FISH" },{ "G for GOAT" },
            { "H for HORSE" },{ "I for ICECREAM" },{ "J for JOKER" },
            { "K for KITE" },{ "L for LAMB" },{ "M for MONKEY" },
            { "N for NEST" },{ "O for ORANGE" },{ "P for POT" },
            { "Q for QUEEN" },{ "R for RAT" },{ "S for SHEEP" },
            { "T for TABLE" },{ "U for UMBRELLA" },{ "V for VIOLIN" },{ "W for WAX" },
            { "X for XEROX" },{ "Y for YUMMY" },{ "Z for ZEBRA" }
        };
        struct cstl_set* pSet = cstl_set_new(compare_e, delete_e);
        size = sizeof(ti) / sizeof(ti[0]);

        //*
        for (index = 0; index < size; index++) {
            char *temp = cstl_strdup(ti[index].string);
            cstl_set_insert(pSet, &temp, sizeof(char *));
        }
        for (index = 0; index < size; index++) {
            v = ti[index].string;
            assert(cstl_true == cstl_set_exists(pSet, &v));
            // cstl_set_remove(pSet, &v);
        }
        // */
        {
        const void *pElement;
        struct cstl_iterator *myItr = cstl_set_new_iterator(pSet);
        while ((pElement = myItr->next(myItr))) {
            const void* value = myItr->current_value(myItr);
            printf("%s\n", *((char **)value));
        }
        cstl_set_delete_iterator(myItr);
        }
        cstl_set_delete(pSet);
    }
    test_with_iterators();
}
