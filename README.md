# CSTL - STL like container support in C language

## Dynamic Array
```cpp
struct cstl_array {
    int no_max_elements; /* Number of maximum elements array can hold without reallocation */
    int no_of_elements;  /* Number of current elements in the array */
    struct cstl_object** pElements; /* actual storage area */
    cstl_compare compare_fn; /* Compare function pointer*/
    cstl_destroy destruct_fn; /* Destructor function pointer*/
};

struct cstl_array* cstl_array_new ( int init_size, cstl_compare fn_c, cstl_destroy fn_d);
cstl_error cstl_array_push_back ( struct cstl_array* pArray, void* elem, size_t elem_size);
const void * cstl_array_element_at(struct cstl_array* pArray, int index);
cstl_error cstl_array_insert_at ( struct cstl_array* pArray, int index, void* elem, size_t elem_size);
int cstl_array_size( struct cstl_array* pArray);
int cstl_array_capacity( struct cstl_array* pArray );
cstl_bool  cstl_array_empty( struct cstl_array* pArray);
cstl_error cstl_array_reserve( struct cstl_array* pArray, int pos);
const void * cstl_array_front(struct cstl_array* pArray);
const void * cstl_array_back(struct cstl_array* pArray);
cstl_error cstl_array_remove_from ( struct cstl_array*, int pos);
cstl_error cstl_array_delete( struct cstl_array* pArray);

struct cstl_iterator* cstl_array_new_iterator(struct cstl_array* pArray);
void cstl_array_delete_iterator ( struct cstl_iterator* pItr);
```

## deque
```cpp
struct cstl_deque {
    struct cstl_object**pElements;
    int no_max_elements;
    int head;
    int tail;
    int no_of_elements;
    cstl_compare compare_fn;
    cstl_destroy destruct_fn;
}c_deque;

struct cstl_deque* cstl_deque_new( int deq_size , cstl_compare fn_c, cstl_destroy fn_d);
cstl_error     cstl_deque_push_back (struct cstl_deque* pDeq, void* elem, size_t elem_size);
cstl_error     cstl_deque_push_front(struct cstl_deque* pDeq, void* elem,size_t elem_size);

cstl_error     cstl_deque_front(struct cstl_deque* pDeq, const void **elem);
cstl_error     cstl_deque_back(struct cstl_deque* pDeq, const void **elem);
cstl_error     cstl_deque_pop_back  (struct cstl_deque* pDeq);
cstl_error     cstl_deque_pop_front (struct cstl_deque* pDeq);
cstl_bool      cstl_deque_empty     (struct cstl_deque* pDeq);
int            cstl_deque_size ( struct cstl_deque* pDeq);
cstl_error     cstl_deque_delete ( struct cstl_deque* pDeq);
const void *   cstl_deque_element_at(struct cstl_deque* pDeq, int index);

struct cstl_iterator* cstl_deque_new_iterator(struct cstl_deque* pDeq);
void cstl_deque_delete_iterator ( struct cstl_iterator* pItr);
```

## list
```cpp
struct cstl_slist_node {
    struct cstl_object* elem;
    struct cstl_slist_node *next;
};
struct cstl_slist {
    struct cstl_slist_node* head;
    cstl_destroy destruct_fn;
    cstl_compare compare_key_fn;
    int size;
};
struct cstl_slist* cstl_slist_new(cstl_destroy fn_d, cstl_compare fn_c);
void           cstl_slist_delete   (struct cstl_slist* pSlist);
cstl_error     cstl_slist_insert   (struct cstl_slist* pSlist, int pos, void* elem, size_t elem_size);
cstl_error     cstl_slist_push_back(struct cstl_slist* pSlist, void* elem, size_t elem_size);
void           cstl_slist_remove   (struct cstl_slist* pSlist, int pos);
void cstl_slist_for_each(struct cstl_slist* pSlist, void(*fn)(const void *elem, void *p), void *p);
const void *   cstl_slist_find(struct cstl_slist* pSlist, void* find_value);


struct cstl_iterator* cstl_slist_new_iterator(struct cstl_slist* pSlit);
void cstl_slist_delete_iterator ( struct cstl_iterator* pItr);
```
## set
```cpp
struct cstl_set {
    struct cstl_rb* root;
};
struct cstl_set* cstl_set_new( cstl_compare fn_c, cstl_destroy fn_d);
cstl_error   cstl_set_insert ( struct cstl_set* pSet, void* key, size_t key_size);
cstl_bool    cstl_set_exists ( struct cstl_set* pSet, void* key);
cstl_error   cstl_set_remove ( struct cstl_set* pSet, void* key);
cstl_bool    cstl_set_find   ( struct cstl_set* pSet, void* key, const void **outKey);
cstl_error   cstl_set_delete ( struct cstl_set* pSet);

struct cstl_iterator* cstl_set_new_iterator(struct cstl_set* pSet);
void cstl_set_delete_iterator ( struct cstl_iterator* pItr);
```

## map
```cpp
struct cstl_map {
    struct cstl_rb* root;
};

struct cstl_map* cstl_map_new    ( cstl_compare fn_c_k, cstl_destroy fn_k_d, cstl_destroy fn_v_d);
cstl_error   cstl_map_insert ( struct cstl_map* pMap, void* key, size_t key_size, void* value,  size_t value_size);
cstl_bool    cstl_map_exists ( struct cstl_map* pMap, void* key);
cstl_error   cstl_map_remove ( struct cstl_map* pMap, void* key);
const void * cstl_map_find(struct cstl_map* pMap, const void* key);
cstl_error   cstl_map_delete ( struct cstl_map* pMap);

struct cstl_iterator* cstl_map_new_iterator(struct cstl_map* pMap);
void cstl_map_delete_iterator ( struct cstl_iterator* pItr);
```
