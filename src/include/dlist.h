#ifndef DLIST_H
#define DLIST_H

#include <stdlib.h>

#define DLIST_INITSZ 128 * sizeof(void*)

typedef struct {
    void** buf; // The buffer containing the list.
    size_t sz;  // The size of the buffer.
    size_t ln;  // The number of elements in the list.
} DList;

// Create a new `DList`.
DList* dlist_init(void);
// Destroy a `DList`.
void dlist_destroy(DList* dstr);
// Destroy `DList` structure but preserve `->buf`.
void dlist_destroypsv(DList* dstr);

// Append `src` to `dest`.
void dlist_append(DList* dest, void* src);

#endif
