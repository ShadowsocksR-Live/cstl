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

#ifndef _C_MAP_H_
#define _C_MAP_H_

struct cstl_map;

extern struct cstl_map* cstl_map_new( cstl_compare fn_c_k, cstl_destroy fn_k_d, cstl_destroy fn_v_d);
extern cstl_error   cstl_map_insert ( struct cstl_map* pMap, const void* key, size_t key_size, const void* value, size_t value_size);
extern cstl_bool    cstl_map_exists ( struct cstl_map* pMap, const void* key);
extern cstl_error   cstl_map_replace(struct cstl_map* pMap, const void* key, const void* value, size_t value_size);
extern cstl_error   cstl_map_remove ( struct cstl_map* pMap, const void* key);
extern const void * cstl_map_find(struct cstl_map* pMap, const void* key);
extern cstl_error   cstl_map_delete ( struct cstl_map* pMap);

extern struct cstl_iterator* cstl_map_new_iterator(struct cstl_map* pMap);
extern void cstl_map_delete_iterator ( struct cstl_iterator* pItr);

typedef void(*map_iter_callback)(struct cstl_map *map, const void *key, const void *value, cstl_bool *stop, void *p);
extern void cstl_map_traverse(struct cstl_map *map, map_iter_callback cb, void *p);

#endif
