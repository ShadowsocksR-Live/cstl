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

#ifndef _C_DEQUE_H_
#define _C_DEQUE_H_

struct cstl_deque;

extern struct cstl_deque* cstl_deque_new( size_t deq_size , cstl_compare fn_c, cstl_destroy fn_d);
extern size_t cstl_deque_count(struct cstl_deque *deque);
extern cstl_error     cstl_deque_push_back (struct cstl_deque* pDeq, void* elem, size_t elem_size);
extern cstl_error     cstl_deque_push_front(struct cstl_deque* pDeq, void* elem, size_t elem_size);

extern const void *   cstl_deque_front(struct cstl_deque* pDeq);
extern const void *   cstl_deque_back(struct cstl_deque* pDeq);
extern cstl_error     cstl_deque_pop_back  (struct cstl_deque* pDeq);
extern cstl_error     cstl_deque_pop_front (struct cstl_deque* pDeq);
extern cstl_bool      cstl_deque_empty     (struct cstl_deque* pDeq);
extern size_t         cstl_deque_size ( struct cstl_deque* pDeq);
extern cstl_error     cstl_deque_delete ( struct cstl_deque* pDeq);
extern const void *   cstl_deque_element_at(struct cstl_deque* pDeq, size_t index);

extern struct cstl_iterator* cstl_deque_new_iterator(struct cstl_deque* pDeq);
extern void cstl_deque_delete_iterator ( struct cstl_iterator* pItr);

#endif
