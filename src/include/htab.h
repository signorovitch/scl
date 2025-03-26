#ifndef HTAB_H
#define HTAB_H

#include <stddef.h>
#include <stdint.h>

#define HTAB_FN fnv1a_hash // Function used for hashing.
#define HTAB_SPACE                                                             \
    1024 // Number of entries possible in the hash table; must be
         // power of 2.

// Hash Table.
typedef void* HTab[HTAB_SPACE];

// Create a new hash table.
HTab* htab_init();
// Destroy a hash table, but not its elements.
void htab_destroy(HTab* htab);

// Get element at `hash`. Return its contents, or NULL if nothing found.
void* htab_get(HTab* htab, char* str);

// Insert `data` at index `hash`.
void htab_ins(HTab* htab, char* key, void* data);

// Gets the length of the hash table.
size_t htab_ln(HTab* htab);

#endif
