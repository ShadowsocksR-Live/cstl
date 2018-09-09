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

#ifndef _C_ARRAY_H_
#define _C_ARRAY_H_

struct cstl_array;

extern struct cstl_array* cstl_array_new ( size_t init_size, cstl_compare fn_c, cstl_destroy fn_d);
extern cstl_error cstl_array_push_back ( struct cstl_array* pArray, void* elem, size_t elem_size);
extern const void * cstl_array_element_at(struct cstl_array* pArray, size_t index);
extern cstl_error cstl_array_insert_at ( struct cstl_array* pArray, size_t index, void* elem, size_t elem_size);
extern size_t cstl_array_size( struct cstl_array* pArray);
extern size_t cstl_array_capacity( struct cstl_array* pArray );
extern cstl_bool  cstl_array_empty( struct cstl_array* pArray);
extern cstl_error cstl_array_reserve( struct cstl_array* pArray, size_t pos);
extern const void * cstl_array_front(struct cstl_array* pArray);
extern const void * cstl_array_back(struct cstl_array* pArray);
extern cstl_error cstl_array_remove_from ( struct cstl_array*, size_t pos);
extern cstl_error cstl_array_delete( struct cstl_array* pArray);

extern struct cstl_iterator* cstl_array_new_iterator(struct cstl_array* pArray);
extern void cstl_array_delete_iterator ( struct cstl_iterator* pItr);

#endif
