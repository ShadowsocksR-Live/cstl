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

#ifndef __C_STL_SET_H__
#define __C_STL_SET_H__

struct cstl_set;

extern struct cstl_set *cstl_set_new(cstl_compare fn_c, cstl_destroy fn_d);
extern cstl_error cstl_set_insert(struct cstl_set *pSet, void *key,
                                  size_t key_size);
extern cstl_bool cstl_set_exists(struct cstl_set *pSet, void *key);
extern cstl_error cstl_set_remove(struct cstl_set *pSet, void *key);
extern const void *cstl_set_find(struct cstl_set *pSet, const void *key);
extern cstl_error cstl_set_delete(struct cstl_set *pSet);

extern struct cstl_iterator *cstl_set_new_iterator(struct cstl_set *pSet);
extern void cstl_set_delete_iterator(struct cstl_iterator *pItr);

typedef void(*fn_cstl_set_iter)(struct cstl_set *set, const void *obj, cstl_bool *stop, void *p);
extern void cstl_set_container_traverse(struct cstl_set *set, fn_cstl_set_iter fn, void *p);
extern void cstl_set_container_add(struct cstl_set *set, void *obj);
extern void cstl_set_container_remove(struct cstl_set *set, void *obj);

#endif /* __C_STL_SET_H__ */
