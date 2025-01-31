#ifndef HTAB_H
#define HTAB_H

#include <stddef.h>
#define HTAB_SPACE 128

// Hash Table.
typedef struct {} HTab;

// Create a new hash table.
HTab* htab_init();
// Destroy a hash table.
void htab_destroy(HTab* htab);

// Get element at `hash`. Return its contents, or NULL if nothing found.
void* htab_get(HTab* htab, int hash);

// Insert `data` at index `hash`.
void htab_ins(HTab* htab, int key, void* data);

// Gets the length of the hash table.
size_t htab_ln(HTab* htab);



#endif
