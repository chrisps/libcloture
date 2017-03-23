//
// Created by chris on 1/23/17.
//

#include "util/stl.hpp"
#if USE_VECTOR_T == 1
using namespace cloture::util;
using vector::voidvector_t;
using vector::Vector_t;

void vector::vector_t_reserve(voidvector_t* vvec, size_t r, size_t typesz) {
    if(r < vvec->alloc) {
        vvec->n = r;
        return;
    }
    else {
        auto temp = new char[r * typesz];
        if(vvec->array) {
            __builtin_memcpy(temp, vvec->array, vvec->n * typesz);
            delete vvec->array;
        }
        vvec->array = temp;
        vvec->n = vvec->alloc = r;
    }
}
#endif