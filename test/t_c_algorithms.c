#include "c_lib.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void print_integers(const void *value, const void *key, void *p) {
    (void)key; (void)p;
    if (value) {
        printf("%d\n", *(int*)value);
    }
}

void print_integers_ptr(const void *value, const void *key, void *p) {
    (void)key; (void)p;
    if (value) {
        printf("%d\n", **((int**)value));
    }
}

static void
free_element(void* ptr) {
    if (ptr) {
        void *p = *((void **)ptr);
        if (p) {
            free(p);
        }
    }
}

static int
compare_strings(const void* left, const void* right) {
    return strcmp((const char *)left, (const char *)right);
}

static int
compare_integers(const void* left, const void* right) {
    int *l = (int*)left;
    int *r = (int*)right;

    return (*l - *r);
}

static int
compare_integers_ptr(const void* left, const void* right) {
    int *l = *((int**)left);
    int *r = *((int**)right);
    return (*l - *r);
}

static struct cstl_array*
create_c_array() {
    int size = 10;
    int i = 0;
    void* p_rv = (void*)0;
    int rv = 0;

    struct cstl_array* myArray = cstl_array_new(8, compare_integers, NULL);
    assert(cstl_true == cstl_array_empty(myArray));

    for (i = 0; i < size; i++) {
        cstl_array_push_back(myArray, &i, sizeof(int));
    }
    assert(cstl_false == cstl_array_empty(myArray));
    assert((size_t)size == cstl_array_size(myArray));
    for (i = 0; i < size; i++) {
        p_rv = (void*) cstl_array_element_at(myArray, i);
        rv = *(int*)p_rv;
        assert(rv == i);
    }
    return myArray;
}

static struct cstl_deque*
create_deque() {
    int flip = 1;
    int i = 0;
    int limit = 20;
    struct cstl_deque* myDeq = cstl_deque_new(10, compare_integers, NULL);
    assert((struct cstl_deque*)0 != myDeq);

    for (i = 0; i <= limit; i++) {
        if (flip) {
            cstl_deque_push_back(myDeq, &i, sizeof(int));
            flip = 0;
        } else {
            cstl_deque_push_front(myDeq, &i, sizeof(int));
            flip = 1;
        }
    }
    return myDeq;
}

static struct cstl_set*
create_set() {
    int test[] = { 13,8,17,1,11,15,25,6,22,27 };
    int	index = 0;
    int size = sizeof(test) / sizeof(test[0]);
    struct cstl_set* pSet = cstl_set_new(compare_integers, NULL);

    for (index = 0; index < size; index++) {
        int v = test[index];
        cstl_set_insert(pSet, &v, sizeof(int));
    }
    return pSet;
}

static struct cstl_map*
create_map() {
    char *char_value[] = { "A","B","C","D","E","F","G","H","I","J","K","L","M",
                           "N","O","P","Q","R","S","T","U","V","W","X","Y","Z" };
    int int_value[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,
                       11,12,13,14,15,16,17,18,19,20,
                       21,22,23,24,25,26 };
    int size = sizeof(char_value) / sizeof(char_value[0]);
    int i = 0;
    struct cstl_map *pMap = cstl_map_new(compare_strings, NULL, NULL);
    for (i = 0; i < size; i++) {
        char *key = char_value[i];
        int key_length = (int)strlen(key) + 1;
        int value = int_value[i];
        cstl_map_insert(pMap, key, key_length, &value, sizeof(int));
    }
    return pMap;
}

static struct cstl_list*
create_slist() {
    struct cstl_list* pList = cstl_list_new(free_element, compare_integers_ptr);
    int i = 0;
    for (i = 0; i <= 10; i++) {
        int *v = (int *)calloc(1, sizeof(int));
        *v = i; // memcpy ( v, &i, sizeof ( int ));
        cstl_list_push_back(pList, &v, sizeof(int *));
    }
    return pList;
}

static void
t_cstl_for_each(void) {
    struct cstl_array *pArray;
    struct cstl_deque *pDeq;
    struct cstl_set   *pSet;
    struct cstl_map   *pMap;
    struct cstl_list *pList;
    struct cstl_iterator *pArrayIterator;
    struct cstl_iterator *pDequeIterator;
    struct cstl_iterator *pSetIterator;
    struct cstl_iterator *pMapIterator;
    struct cstl_iterator *pListIterator;

    printf("Performing for_each for array\n");
    pArray = create_c_array();
    pArrayIterator = cstl_array_new_iterator(pArray);
    cstl_for_each(pArrayIterator, print_integers, NULL);
    cstl_array_delete(pArray);
    cstl_array_delete_iterator(pArrayIterator);

    printf("Performing for_each for deque\n");
    pDeq = create_deque();
    pDequeIterator = cstl_deque_new_iterator(pDeq);
    cstl_for_each(pDequeIterator, print_integers, NULL);
    cstl_deque_delete(pDeq);
    cstl_deque_delete_iterator(pDequeIterator);

    printf("Performing for_each for set\n");
    pSet = create_set();
    pSetIterator = cstl_set_new_iterator(pSet);
    cstl_for_each(pSetIterator, print_integers, NULL);
    cstl_set_delete(pSet);
    cstl_set_delete_iterator(pSetIterator);

    printf("Performing for_each for map\n");
    pMap = create_map();
    pMapIterator = cstl_map_new_iterator(pMap);
    cstl_for_each(pMapIterator, print_integers, NULL);
    cstl_map_delete(pMap);
    cstl_map_delete_iterator(pMapIterator);

    printf("Performing for_each for list\n");
    pList = create_slist();
    pListIterator = cstl_list_new_iterator(pList);
    cstl_for_each(pListIterator, print_integers_ptr, NULL);
    cstl_list_destroy(pList);
    cstl_list_delete_iterator(pListIterator);
}

void
test_c_algorithms() {
    t_cstl_for_each();
}
