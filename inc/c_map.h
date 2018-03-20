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

#ifndef _C_MAP_H_
#define _C_MAP_H_

struct cstl_map {
    struct cstl_rb* root;
};


extern struct cstl_map* new_cstl_map( cstl_compare fn_c_k, cstl_destroy fn_k_d, cstl_destroy fn_v_d);
extern cstl_error   insert_cstl_map ( struct cstl_map* pMap, void* key, size_t key_size, void* value,  size_t value_size);
extern cstl_bool    exists_cstl_map ( struct cstl_map* pMap, void* key);
extern cstl_error   remove_cstl_map ( struct cstl_map* pMap, void* key);
extern cstl_bool    find_cstl_map   ( struct cstl_map* pMap, void* key, void**value);
extern cstl_error   delete_cstl_map ( struct cstl_map* pMap);

extern struct cstl_iterator* new_iterator_cstl_map(struct cstl_map* pMap);
extern void delete_iterator_cstl_map ( struct cstl_iterator* pItr);


#endif