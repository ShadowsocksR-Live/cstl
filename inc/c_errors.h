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

#ifndef __C_STL_ERRORS_H__
#define __C_STL_ERRORS_H__

typedef enum {

    /* ------------------------------------------------------------------------*/
    /*                 C O M M O N    E R R O R    C O D E                     */
    /* ------------------------------------------------------------------------*/
    CSTL_ERROR_SUCCESS = 0,
    CSTL_ERROR_MEMORY = -1,
    CSTL_RBTREE_KEY_DUPLICATE = -2,
    CSTL_RBTREE_KEY_NOT_FOUND = -3,

    CSTL_ERROR_ERROR = -10,
    CSTL_ELEMENT_RETURN_ERROR = -11,

    /* ------------------------------------------------------------------------*/
    /*         D Y N A M I C    A R R A Y   E R R O R    C O D E S             */
    /* ------------------------------------------------------------------------*/
    CSTL_ARRAY_NOT_INITIALIZED = -101,
    CSTL_ARRAY_INDEX_OUT_OF_BOUND = -102,
    CSTL_ARRAY_INSERT_FAILED = -103,

    CSTL_DEQUE_NOT_INITIALIZED = -201,
    CSTL_DEQUE_INDEX_OUT_OF_BOUND = -202,

    CSTL_RBTREE_NOT_INITIALIZED = -401,

    CSTL_SET_NOT_INITIALIZED = -501,
    CSTL_SET_INVALID_INPUT = -502,

    CSTL_MAP_NOT_INITIALIZED = -501,
    CSTL_MAP_INVALID_INPUT = -502,

    CSTL_SLIST_INSERT_FAILED = -601
} cstl_error;

#endif /* __C_STL_ERRORS_H__ */
