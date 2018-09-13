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

#ifndef _C_LIB_H_
#define _C_LIB_H_

#include <stdlib.h>

#include "c_errors.h"

/* ------------------------------------------------------------------------*/
/*       C O M M O N       D E F I N I T O N S                             */
/* ------------------------------------------------------------------------*/

typedef void (*cstl_destroy)(void*);
typedef int  (*cstl_compare)(const void*,const void*);
typedef void (*cstl_traversal)( void*);

typedef int  cstl_error;
typedef int  cstl_bool;

#define cstl_black           0
#define cstl_red             1
#define cstl_true            1
#define cstl_false           0

/* ------------------------------------------------------------------------*/
/*                            P  A  I   R                                  */
/* ------------------------------------------------------------------------*/

struct cstl_iterator {
    const void * (*next)(struct cstl_iterator *pIterator);
    void (*replace_current_value)(struct cstl_iterator *pIterator, void *new_value, size_t size);
    const void* (*current_key)(struct cstl_iterator *pIterator);
    const void* (*current_value)(struct cstl_iterator *pIterator);
    void* pContainer;
    size_t current_index;
    void* current_element;
};

#include "c_array.h"
#include "c_deque.h"
#include "c_set.h"
#include "c_map.h"
#include "c_list.h"
#include "c_map.h"
#include "c_algorithms.h"

/* ------------------------------------------------------------------------*/
/*            H E L P E R       F U N C T I O N S                          */
/* ------------------------------------------------------------------------*/

extern void  cstl_copy ( void* destination, void* source, size_t size );
extern void  cstl_get  ( void* destination, void* source, size_t size);
extern char* cstl_strdup (const char *ptr);

struct cstl_object;

extern struct cstl_object* cstl_object_new (const void* inObject, size_t obj_size);
extern const void * cstl_object_get_data(struct cstl_object *inObject);
extern void  cstl_object_delete  (struct cstl_object* inObject );
extern void cstl_object_replace_raw(struct cstl_object* current_object,const void* elem, size_t elem_size);

#endif
