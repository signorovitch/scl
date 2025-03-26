#include "include/fnv1a.h"

#include <stdlib.h>

uint64_t fnv1a_hash(char* key, size_t ln) {
    uint64_t hash = FNV1A_BASIS_64;

    for (size_t i = 0; i < ln; i++) {
        hash ^= (unsigned char)key[i];
        hash *= FNV1A_PRIME_64;
    }

    return hash;
}
