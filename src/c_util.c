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

#include "c_lib.h"
#include <string.h>
#include <stdlib.h>

void
cstl_copy(void* destination, void* source, size_t size) {
    memcpy((char*)destination, source, size);
}

void
cstl_get(void* destination, void* source, size_t size) {
    memcpy(destination, (char*)source, size);
}

char * cstl_strdup(const char *ptr) {
#ifdef WIN32
    return _strdup(ptr);
#else
    return strdup(ptr);
#endif
}

struct cstl_object {
    void* raw_data;
    size_t size;
};

struct cstl_object*
cstl_object_new(const void* inObject, size_t obj_size) {
    struct cstl_object* tmp = (struct cstl_object*)calloc(1, sizeof(struct cstl_object));
    void *raw_data = (void *)0;
    if (!tmp) {
        return (struct cstl_object*)0;
    }
    tmp->size = obj_size;
    raw_data = (void*)calloc(obj_size, sizeof(char));
    if (!raw_data) {
        free(tmp);
        return (struct cstl_object*)0;
    }
    memcpy(raw_data, inObject, obj_size);
    tmp->raw_data = raw_data;
    return tmp;
}

const void * cstl_object_get_data(struct cstl_object *inObject) {
    return inObject->raw_data;
}

void
cstl_object_replace_raw(struct cstl_object* current_object, const void* elem, size_t elem_size) {
    free(current_object->raw_data);
    current_object->raw_data = (void*)calloc(elem_size, sizeof(char));
    memcpy(current_object->raw_data, elem, elem_size);
}

void
cstl_object_delete(struct cstl_object* inObject) {
    if (inObject) {
        free(inObject->raw_data);
        free(inObject);
    }
}
