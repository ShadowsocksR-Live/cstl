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

extern struct cstl_array* new_cstl_array ( int init_size, cstl_compare fn_c, cstl_destroy fn_d);
extern cstl_error push_back_cstl_array ( struct cstl_array* pArray, void* elem, size_t elem_size);
extern cstl_error element_at_cstl_array( struct cstl_array* pArray, int pos, void**e);
extern cstl_error insert_at_cstl_array ( struct cstl_array* pArray, int index, void* elem, size_t elem_size);
extern int size_cstl_array( struct cstl_array* pArray);
extern int capacity_cstl_array( struct cstl_array* pArray );
extern cstl_bool  empty_cstl_array( struct cstl_array* pArray);
extern cstl_error reserve_cstl_array( struct cstl_array* pArray, int pos);
extern cstl_error front_cstl_array( struct cstl_array* pArray,void* elem);
extern cstl_error back_cstl_array( struct cstl_array* pArray,void* elem);
extern cstl_error remove_from_cstl_array ( struct cstl_array*, int pos);
extern cstl_error delete_cstl_array( struct cstl_array* pArray);

extern struct cstl_iterator* new_iterator_cstl_array(struct cstl_array* pArray);
extern void delete_iterator_cstl_array ( struct cstl_iterator* pItr);

#endif
