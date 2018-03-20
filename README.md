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

struct cstl_array* new_c_array ( int init_size, cstl_compare fn_c, cstl_destroy fn_d);
cstl_error push_back_c_array ( struct cstl_array* pArray, void* elem, size_t elem_size);
cstl_error element_at_c_array( struct cstl_array* pArray, int pos, void**e);
cstl_error insert_at_c_array ( struct cstl_array* pArray, int index, void* elem, size_t elem_size);
int size_c_array( struct cstl_array* pArray);
int capacity_c_array( struct cstl_array* pArray );
cstl_bool  empty_c_array( struct cstl_array* pArray);
cstl_error reserve_c_array( struct cstl_array* pArray, int pos);
cstl_error front_c_array( struct cstl_array* pArray,void* elem);
cstl_error back_c_array( struct cstl_array* pArray,void* elem);
cstl_error remove_from_c_array ( struct cstl_array*, int pos);
cstl_error delete_c_array( struct cstl_array* pArray);

struct cstl_iterator* new_iterator_c_array(struct cstl_array* pArray);
void delete_iterator_c_array ( struct cstl_iterator* pItr);
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

struct cstl_deque* new_c_deque( int deq_size , cstl_compare fn_c, cstl_destroy fn_d);
cstl_error     push_back_c_deque (struct cstl_deque* pDeq, void* elem, size_t elem_size);
cstl_error     push_front_c_deque(struct cstl_deque* pDeq, void* elem,size_t elem_size);

cstl_error     front_c_deque     (struct cstl_deque* pDeq,void*);
cstl_error     back_c_deque      (struct cstl_deque* pDeq,void*);
cstl_error     pop_back_c_deque  (struct cstl_deque* pDeq);
cstl_error     pop_front_c_deque (struct cstl_deque* pDeq);
cstl_bool      empty_c_deque     (struct cstl_deque* pDeq);
int            size_c_deque ( struct cstl_deque* pDeq);
cstl_error     delete_c_deque ( struct cstl_deque* pDeq);
cstl_error     element_at_c_deque (struct cstl_deque* pDeq, int index, void**elem);

struct cstl_iterator* new_iterator_c_deque(struct cstl_deque* pDeq);
void delete_iterator_c_deque ( struct cstl_iterator* pItr);
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
struct cstl_slist* new_c_slist(cstl_destroy fn_d, cstl_compare fn_c);
void           delete_c_slist   (struct cstl_slist* pSlist);
cstl_error     insert_c_slist   (struct cstl_slist* pSlist, int pos, void* elem, size_t elem_size);
cstl_error     push_back_c_slist(struct cstl_slist* pSlist, void* elem, size_t elem_size);
void           remove_c_slist   (struct cstl_slist* pSlist, int pos);
void           for_each_c_slist (struct cstl_slist* pSlist, void (*fn)(void* ));
cstl_bool      find_c_slist     (struct cstl_slist* pSlist, void* find_value, void**out_value);


struct cstl_iterator* new_iterator_c_slist(struct cstl_slist* pSlit);
void delete_iterator_c_slist ( struct cstl_iterator* pItr);
```
## set
```cpp
struct cstl_set {
    struct cstl_rb* root;
};
struct cstl_set* new_c_set( cstl_compare fn_c, cstl_destroy fn_d);
cstl_error   insert_c_set ( struct cstl_set* pSet, void* key, size_t key_size);
cstl_bool    exists_c_set ( struct cstl_set* pSet, void* key);
cstl_error   remove_c_set ( struct cstl_set* pSet, void* key);
cstl_bool    find_c_set   ( struct cstl_set* pSet, void* key, void* outKey);
cstl_error   delete_c_set ( struct cstl_set* pSet);

struct cstl_iterator* new_iterator_c_set(struct cstl_set* pSet);
void delete_iterator_c_set ( struct cstl_iterator* pItr);
```

## map
```cpp
struct cstl_map {
    struct cstl_rb* root;
};

struct cstl_map* new_c_map    ( cstl_compare fn_c_k, cstl_destroy fn_k_d, cstl_destroy fn_v_d);
cstl_error   insert_c_map ( struct cstl_map* pMap, void* key, size_t key_size, void* value,  size_t value_size);
cstl_bool    exists_c_map ( struct cstl_map* pMap, void* key);
cstl_error   remove_c_map ( struct cstl_map* pMap, void* key);
cstl_bool    find_c_map   ( struct cstl_map* pMap, void* key, void**value);
cstl_error   delete_c_map ( struct cstl_map* pMap);

struct cstl_iterator* new_iterator_c_map(struct cstl_map* pMap);
void delete_iterator_c_map ( struct cstl_iterator* pItr);
```


