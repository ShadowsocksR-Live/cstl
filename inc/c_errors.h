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

#ifndef _C_ERRORS_H_
#define _C_ERRORS_H_

/* ------------------------------------------------------------------------*/
/*                 C O M M O N    E R R O R    C O D E                     */
/* ------------------------------------------------------------------------*/
#define CSTL_ERROR_SUCCESS  0
#define CSTL_ERROR_ERROR    1
#define CSTL_ERROR_MEMORY   2
#define CSTL_ELEMENT_RETURN_ERROR 3
/* ------------------------------------------------------------------------*/
/*         D Y N A M I C    A R R A Y   E R R O R    C O D E S             */
/* ------------------------------------------------------------------------*/
#define CSTL_ARRAY_NOT_INITIALIZED    101
#define CSTL_ARRAY_INDEX_OUT_OF_BOUND 102
#define CSTL_ARRAY_INSERT_FAILED      103

#define CSTL_DEQUE_NOT_INITIALIZED    201
#define CSTL_DEQUE_INDEX_OUT_OF_BOUND 202

#define CSTL_RBTREE_NOT_INITIALIZED   401
#define CSTL_RBTREE_KEY_DUPLICATE     402
#define CSTL_RBTREE_KEY_NOT_FOUND     403

#define CSTL_SET_NOT_INITIALIZED      501
#define CSTL_SET_INVALID_INPUT        502

#define CSTL_MAP_NOT_INITIALIZED      501
#define CSTL_MAP_INVALID_INPUT        502

#define CSTL_SLIST_INSERT_FAILED      601

#endif
