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

#ifndef _C_ARRAY_H_
#define _C_ARRAY_H_

struct cstl_array {
    int no_max_elements; /* Number of maximum elements array can hold without reallocation */
    int no_of_elements;  /* Number of current elements in the array */
    struct cstl_object** pElements; /* actual storage area */
    cstl_compare compare_fn; /* Compare function pointer*/
    cstl_destroy destruct_fn; /* Destructor function pointer*/
};

extern struct cstl_array* cstl_array_new ( int init_size, cstl_compare fn_c, cstl_destroy fn_d);
extern cstl_error cstl_array_push_back ( struct cstl_array* pArray, void* elem, size_t elem_size);
extern cstl_error cstl_array_element_at( struct cstl_array* pArray, int pos, void**e);
extern cstl_error cstl_array_insert_at ( struct cstl_array* pArray, int index, void* elem, size_t elem_size);
extern int cstl_array_size( struct cstl_array* pArray);
extern int cstl_array_capacity( struct cstl_array* pArray );
extern cstl_bool  cstl_array_empty( struct cstl_array* pArray);
extern cstl_error cstl_array_reserve( struct cstl_array* pArray, int pos);
extern cstl_error cstl_array_front( struct cstl_array* pArray,void* elem);
extern cstl_error cstl_array_back( struct cstl_array* pArray,void* elem);
extern cstl_error cstl_array_remove_from ( struct cstl_array*, int pos);
extern cstl_error cstl_array_delete( struct cstl_array* pArray);

extern struct cstl_iterator* cstl_array_new_iterator(struct cstl_array* pArray);
extern void cstl_array_delete_iterator ( struct cstl_iterator* pItr);

#endif
