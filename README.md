# CSTL - STL like container support in C language

## Dynamic Array
```cpp
struct cstl_array {
    size_t capacity; /* Number of maximum elements array can hold without reallocation */
    size_t count;  /* Number of current elements in the array */
    struct cstl_object** pElements; /* actual storage area */
    cstl_compare compare_fn; /* Compare function pointer*/
    cstl_destroy destruct_fn; /* Destructor function pointer*/
};

struct cstl_array* cstl_array_new ( size_t init_size, cstl_compare fn_c, cstl_destroy fn_d);
cstl_error cstl_array_push_back ( struct cstl_array* pArray, void* elem, size_t elem_size);
const void * cstl_array_element_at(struct cstl_array* pArray, size_t index);
cstl_error cstl_array_insert_at ( struct cstl_array* pArray, size_t index, void* elem, size_t elem_size);
size_t cstl_array_size( struct cstl_array* pArray);
size_t cstl_array_capacity( struct cstl_array* pArray );
cstl_bool  cstl_array_empty( struct cstl_array* pArray);
cstl_error cstl_array_reserve( struct cstl_array* pArray, size_t pos);
const void * cstl_array_front(struct cstl_array* pArray);
const void * cstl_array_back(struct cstl_array* pArray);
cstl_error cstl_array_remove_from ( struct cstl_array*, size_t pos);
cstl_error cstl_array_delete( struct cstl_array* pArray);

struct cstl_iterator* cstl_array_new_iterator(struct cstl_array* pArray);
void cstl_array_delete_iterator ( struct cstl_iterator* pItr);
```

## deque
```cpp
struct cstl_deque {
    struct cstl_object**pElements;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
    cstl_compare compare_fn;
    cstl_destroy destruct_fn;
};

struct cstl_deque* cstl_deque_new( size_t deq_size , cstl_compare fn_c, cstl_destroy fn_d);
cstl_error     cstl_deque_push_back (struct cstl_deque* pDeq, void* elem, size_t elem_size);
cstl_error     cstl_deque_push_front(struct cstl_deque* pDeq, void* elem,size_t elem_size);

const void * cstl_deque_front(struct cstl_deque* pDeq);
const void * cstl_deque_back(struct cstl_deque* pDeq);
cstl_error     cstl_deque_pop_back  (struct cstl_deque* pDeq);
cstl_error     cstl_deque_pop_front (struct cstl_deque* pDeq);
cstl_bool      cstl_deque_empty     (struct cstl_deque* pDeq);
size_t         cstl_deque_size ( struct cstl_deque* pDeq);
cstl_error     cstl_deque_delete ( struct cstl_deque* pDeq);
const void *   cstl_deque_element_at(struct cstl_deque* pDeq, size_t index);

struct cstl_iterator* cstl_deque_new_iterator(struct cstl_deque* pDeq);
void cstl_deque_delete_iterator ( struct cstl_iterator* pItr);
```

## list
```cpp
struct cstl_list_node {
    struct cstl_object* elem;
    struct cstl_list_node *next;
};
struct cstl_list {
    struct cstl_list_node* head;
    cstl_destroy destruct_fn;
    cstl_compare compare_key_fn;
    size_t size;
};
struct cstl_list* cstl_list_new(cstl_destroy fn_d, cstl_compare fn_c);
void cstl_list_destroy(struct cstl_list* pList);
void cstl_list_clear(struct cstl_list* pList);
cstl_error     cstl_list_insert   (struct cstl_list* pList, size_t pos, void* elem, size_t elem_size);
cstl_error     cstl_list_push_back(struct cstl_list* pList, void* elem, size_t elem_size);
void           cstl_list_remove   (struct cstl_list* pList, size_t pos);
void cstl_list_for_each(struct cstl_list* pList, void(*fn)(const void *elem, void *p), void *p);
const void *   cstl_list_find(struct cstl_list* pList, void* find_value);


struct cstl_iterator* cstl_list_new_iterator(struct cstl_list* pSlit);
void cstl_list_delete_iterator ( struct cstl_iterator* pItr);
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
const void * cstl_set_find(struct cstl_set* pSet, const void* key);
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
cstl_error   cstl_map_insert ( struct cstl_map* pMap, const void* key, size_t key_size, const void* value, size_t value_size);
cstl_bool    cstl_map_exists ( struct cstl_map* pMap, const void* key);
cstl_error   cstl_map_remove ( struct cstl_map* pMap, const void* key);
const void * cstl_map_find(struct cstl_map* pMap, const void* key);
cstl_error   cstl_map_delete ( struct cstl_map* pMap);

struct cstl_iterator* cstl_map_new_iterator(struct cstl_map* pMap);
void cstl_map_delete_iterator ( struct cstl_iterator* pItr);
```

## clang-format
```
find . -regex '.*\.\(c\|h\|cpp\|hpp\|cc\|cxx\)' -exec clang-format -style=file -i {} \;
```