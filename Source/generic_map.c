#include "generic_map.h"

// A detailed explanation of this can be found here: 
// https://probablydance.com/2018/06/16/fibonacci-hashing-the-optimization-that-the-world-forgot-or-a-better-alternative-to-integer-modulo/

// The specific constant was found here:
// http://book.huihoo.com/data-structures-and-algorithms-with-object-oriented-design-patterns-in-c++/html/page214.html

#define HASH_CONSTANT 2654435769

uint32_t fibHash(uint32_t hash, uint32_t shift) {
    return (hash * HASH_CONSTANT) >> shift;
}