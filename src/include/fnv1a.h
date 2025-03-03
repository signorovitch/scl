#ifndef FNV1A_H
#define FNV1A_H

// Implements the FNV-1a hash algorithm.

#include <stdint.h>
#include <stdlib.h>

// FNV prime.
#define FNV1A_PRIME_64 0x00000100000001b3u

// Offset basis.
#define FNV1A_BASIS_64 0xcbf29ce484222325u

// Hash a string `str` of length `ln`.
uint64_t fnv1a_hash(char* str, size_t ln);

#endif
