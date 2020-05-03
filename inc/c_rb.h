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

#ifndef _C_RB_H_
#define _C_RB_H_

struct cstl_object;

struct cstl_rb_node {
    struct cstl_rb_node *left;
    struct cstl_rb_node *right;
    struct cstl_rb_node *parent;
    int color;
    struct cstl_object* key;
    struct cstl_object* value;
};

struct cstl_rb {
    struct cstl_rb_node* root;
    struct cstl_rb_node sentinel;
    cstl_destroy destruct_k_fn;
    cstl_destroy destruct_v_fn;
    cstl_compare compare_fn;
};

extern struct cstl_rb* cstl_rb_new(cstl_compare fn_c,cstl_destroy fn_ed, cstl_destroy fn_vd );
extern cstl_error cstl_rb_insert(struct cstl_rb* pTree, const void* key, size_t key_size, const void* value, size_t value_size);
extern struct cstl_rb_node* cstl_rb_find (struct cstl_rb* pTree, const void* key);
extern struct cstl_rb_node* cstl_rb_remove (struct cstl_rb* pTree, const void* key);
extern cstl_error cstl_rb_delete (struct cstl_rb* pTree);
extern cstl_bool cstl_rb_empty  (struct cstl_rb* pTree);

extern struct cstl_rb_node *cstl_rb_minimum( struct cstl_rb* pTree, struct cstl_rb_node* x );
extern struct cstl_rb_node* cstl_rb_tree_successor(struct cstl_rb* pTree, struct cstl_rb_node* x);

#endif
