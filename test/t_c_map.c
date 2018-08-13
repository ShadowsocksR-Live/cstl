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
#include <assert.h>
#include <stdio.h>

static int
compare_e(void* left, void* right) {
    return strcmp((const char *)left, (const char *)right);
}

char *char_value[] = { "A","B","C","D","E","F","G","H","I","J","K","L","M",
                       "N","O","P","Q","R","S","T","U","V","W","X","Y","Z" };
int int_value[] = {  1, 2, 3, 4, 5, 6, 7, 8, 9,10,
                    11,12,13,14,15,16,17,18,19,20,
                    21,22,23,24,25,26 };

static void
insert_all(struct cstl_map* myMap) {
    int size = sizeof(char_value) / sizeof(char_value[0]);
    int i = 0;
    for (i = 0; i < size; i++) {
        char *key = cstl_strdup(char_value[i]);
        int key_length = (int)strlen(key) + 1;
        int value = int_value[i];
        printf("Inserting [%s -> %d]\n", key, value);
        cstl_map_insert(myMap, key, key_length, &value, sizeof(int));
        free(key);
    }
}

static void
check_exists_all(struct cstl_map* myMap) {
    int size = sizeof(char_value) / sizeof(char_value[0]);
    int i = 0;
    for (i = 0; i < size; i++) {
        const void* value;
        assert(cstl_true == cstl_map_exists(myMap, char_value[i]));
        assert((value = cstl_map_find(myMap, char_value[i])) != NULL);
        printf("-----> [%s == %d]\n", char_value[i], *(int*)value);
        assert(*(int*)value == int_value[i]);
    }
}

static void
remove_some_exist(struct cstl_map* myMap) {
    assert(CSTL_ERROR_SUCCESS == cstl_map_remove(myMap, "A"));
    assert(cstl_false == cstl_map_exists(myMap, "A"));

    assert(CSTL_ERROR_SUCCESS == cstl_map_remove(myMap, "X"));
    assert(cstl_false == cstl_map_exists(myMap, "X"));

    assert(CSTL_ERROR_SUCCESS == cstl_map_remove(myMap, "Z"));
    assert(cstl_false == cstl_map_exists(myMap, "Z"));

    assert(CSTL_ERROR_SUCCESS == cstl_map_remove(myMap, "H"));
    assert(cstl_false == cstl_map_exists(myMap, "H"));
}

static void
add_removed_check_all(struct cstl_map* myMap) {

    char *key = cstl_strdup("A");
    int  key_length = (int)strlen(key) + 1;
    cstl_map_insert(myMap, key, key_length, &int_value[0], sizeof(int));
    free(key);

    key = cstl_strdup("X");
    key_length = (int)strlen(key) + 1;
    cstl_map_insert(myMap, key, key_length, &int_value[23], sizeof(int));
    free(key);

    key = cstl_strdup("Z");
    key_length = (int)strlen(key) + 1;
    cstl_map_insert(myMap, key, key_length, &int_value[25], sizeof(int));
    free(key);

    key = cstl_strdup("H");
    key_length = (int)strlen(key) + 1;
    cstl_map_insert(myMap, key, key_length, &int_value[7], sizeof(int));
    free(key);

    check_exists_all(myMap);
}

static void
print_using_iterator(struct cstl_map *myMap) {
    struct cstl_iterator *myItr;
    struct cstl_object *pElement;
    printf("------------------------------------------------\n");
    myItr = cstl_map_new_iterator(myMap);

    while (pElement = myItr->get_next(myItr)) {
        struct cstl_rb_node *current = ((struct cstl_rb_node*)(myItr->pCurrentElement));
        struct cstl_object* key0 = current->key;
        struct cstl_object* value0 = current->value;
        int *value = (int *)0;
        char *key = (char *)0;

        assert(value0 == pElement);

#if 1
        key = (char *) cstl_object_get_data(key0);
        value = (int *) cstl_object_get_data(value0);
        printf("%s : %d\n", key, *value);
#else
        key = (char *) myItr->get_value(key0);
        value = (int *) myItr->get_value(pElement);
        printf("%s : %d\n", key, *value);
#endif
    }
    cstl_map_delete_iterator(myItr);
}

static void
replace_values_using_iterators(struct cstl_map* myMap) {
    struct cstl_iterator *myItr;
    struct cstl_object *pElement;
    printf("------------------------------------------------\n");
    myItr = cstl_map_new_iterator(myMap);
    pElement = myItr->get_next(myItr);
    while (pElement) {
        const void* old_value = myItr->get_value(pElement);
        int new_value = *(int*)old_value;
        new_value = new_value * 2;
        myItr->replace_value(myItr, &new_value, sizeof(new_value));

        pElement = myItr->get_next(myItr);
    }
    cstl_map_delete_iterator(myItr);
}

static void
test_with_iterators() {
    struct cstl_map* myMap = cstl_map_new(compare_e, NULL, NULL);
    insert_all(myMap);
    print_using_iterator(myMap);
    replace_values_using_iterators(myMap);
    print_using_iterator(myMap);
    cstl_map_delete(myMap);
}

void
test_c_map() {
    struct cstl_map* myMap = cstl_map_new(compare_e, NULL, NULL);
    insert_all(myMap);
    check_exists_all(myMap);
    remove_some_exist(myMap);
    add_removed_check_all(myMap);
    cstl_map_delete(myMap);
    test_with_iterators();
}
