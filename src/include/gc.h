#ifndef GC_H
#define GC_H

#include <stdlib.h>

// The type a GC can refer to.
typedef enum {
    GC_TYPE_AST,
    GC_TYPE_SCOPE,
    GC_TYPE_MAX = GC_TYPE_SCOPE
} GCType;

// Added to each AST and Scope; keep track of what's actually still accessible.
typedef struct GC_STRUCT {
    void* p;               // Pointer to the data.
    struct GC_STRUCT* nxt; // The next GC in the linked list.
    GCType type;           // What type of data.
    bool marked;           // Whether the data is still accessible.
} GC;

GC* gc_init(void* p, GCType type, GC*);
// Does not free ->p or ->nxt.
void gc_destroy(GC* gc);

// Allocate for an object in the heap, and keep track of it in the GC.
void* gc_alloc(size_t sz, GCType type);

// Free everything, immediately.
void gc_hack_free();

#endif
