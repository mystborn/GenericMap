#include "fnv1a.h"

#define FNV_PRIME_32 0x01000193
#define FNV_OFFSET_32 0x811c9dc5

uint32_t fnv32(const char* data) {
    uint32_t hash = FNV_OFFSET_32;
    while(*data != 0)
        hash = (*data++ ^ hash) * FNV_PRIME_32;

    return hash;
}