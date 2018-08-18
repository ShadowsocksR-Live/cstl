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
#include <assert.h>
#include <stdio.h>

static int
compare_e(const void* left, const void* right) {
    const char *l = *(const char **)left;
    const char *r = *(const char **)right;
    return strcmp(l, r);
}

static void key_destroy(void*key) {
    free(*(char **)key);
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
        int value = int_value[i];
        printf("Inserting [%s -> %d]\n", key, value);
        cstl_map_insert(myMap, &key, sizeof(char*), &value, sizeof(int));
    }
}

static void
check_exists_all(struct cstl_map* myMap) {
    int size = sizeof(char_value) / sizeof(char_value[0]);
    int i = 0;
    for (i = 0; i < size; i++) {
        char *key = char_value[i];
        const void* value;
        assert(cstl_true == cstl_map_exists(myMap, &key));
        assert((value = cstl_map_find(myMap, &key)) != NULL);
        printf("-----> [%s == %d]\n", key, *(int*)value);
        assert(*(int*)value == int_value[i]);
    }
}

static void
remove_some_exist(struct cstl_map* myMap) {
    char *key = "A";
    assert(CSTL_ERROR_SUCCESS == cstl_map_remove(myMap, &key));
    assert(cstl_false == cstl_map_exists(myMap, &key));

    key = "X";
    assert(CSTL_ERROR_SUCCESS == cstl_map_remove(myMap, &key));
    assert(cstl_false == cstl_map_exists(myMap, &key));

    key = "Z";
    assert(CSTL_ERROR_SUCCESS == cstl_map_remove(myMap, &key));
    assert(cstl_false == cstl_map_exists(myMap, &key));

    key = "H";
    assert(CSTL_ERROR_SUCCESS == cstl_map_remove(myMap, &key));
    assert(cstl_false == cstl_map_exists(myMap, &key));
}

static void
add_removed_check_all(struct cstl_map* myMap) {

    char *key = cstl_strdup("A");
    cstl_map_insert(myMap, &key, sizeof(char *), &int_value[0], sizeof(int));

    key = cstl_strdup("X");
    cstl_map_insert(myMap, &key, sizeof(char *), &int_value[23], sizeof(int));

    key = cstl_strdup("Z");
    cstl_map_insert(myMap, &key, sizeof(char *), &int_value[25], sizeof(int));

    key = cstl_strdup("H");
    cstl_map_insert(myMap, &key, sizeof(char *), &int_value[7], sizeof(int));

    check_exists_all(myMap);
}

static void
print_using_iterator(struct cstl_map *myMap) {
    struct cstl_iterator *myItr;
    struct cstl_object *pElement;
    printf("------------------------------------------------\n");
    myItr = cstl_map_new_iterator(myMap);

    while ((pElement = myItr->get_next(myItr))) {
        struct cstl_rb_node *current = ((struct cstl_rb_node*)(myItr->current_element));
        struct cstl_object* key0 = current->key;
        struct cstl_object* value0 = current->value;
        int *value = (int *)0;
        char *key = (char *)0;

        assert(value0 == pElement);

#if 1
        key = *((char **) cstl_object_get_data(key0));
        value = (int *) cstl_object_get_data(value0);
        printf("%s : %d\n", key, *value);
#else
        key = *((char **) myItr->get_value(key0));
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
    while ((pElement = myItr->get_next(myItr)) != NULL) {
        const void* old_value = myItr->get_value(pElement);
        int new_value = *(int*)old_value;
        new_value = new_value * 2;
        myItr->replace_value(myItr, &new_value, sizeof(new_value));
    }
    cstl_map_delete_iterator(myItr);
}

static void
test_with_iterators() {
    struct cstl_map* myMap = cstl_map_new(compare_e, key_destroy, NULL);
    insert_all(myMap);
    print_using_iterator(myMap);
    replace_values_using_iterators(myMap);
    print_using_iterator(myMap);
    cstl_map_delete(myMap);
}

void
test_c_map() {
    struct cstl_map* myMap = cstl_map_new(compare_e, key_destroy, NULL);
    insert_all(myMap);
    check_exists_all(myMap);
    remove_some_exist(myMap);
    add_removed_check_all(myMap);
    cstl_map_delete(myMap);
    test_with_iterators();
}
