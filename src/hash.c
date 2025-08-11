//
// Created by Diogo Silva on 2/7/25.
//

#include "dscl/hash.h"
#include <string.h>

#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U

// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
uint32_t dscl_fnv32a_hash(const char *s) {
    uint32_t hash = FNV_OFFSET_32;

    for (uint32_t i = 0; i < strlen(s); i++) {
        hash = hash ^ (s[i]);
        hash = hash * FNV_PRIME_32;
    }

    return hash;
}
