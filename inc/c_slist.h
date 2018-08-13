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

#ifndef _C_SLIST_H_
#define _C_SLIST_H_

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

extern struct cstl_slist * cstl_slist_new (cstl_destroy fn_d, cstl_compare fn_c);
extern void           cstl_slist_delete   (struct cstl_slist* pSlist);
extern cstl_error     cstl_slist_insert   (struct cstl_slist* pSlist, int pos, void* elem, size_t elem_size);
extern cstl_error     cstl_slist_push_back(struct cstl_slist* pSlist, void* elem, size_t elem_size);
extern void           cstl_slist_remove   (struct cstl_slist* pSlist, int pos);
extern void           cstl_slist_for_each (struct cstl_slist* pSlist, void (*fn)(void* ));
extern cstl_bool      cstl_slist_find     (struct cstl_slist* pSlist, void* find_value, const void**out_value);

extern struct cstl_iterator* cstl_slist_new_iterator(struct cstl_slist* pSlit);
extern void cstl_slist_delete_iterator ( struct cstl_iterator* pItr);

#endif
