#ifndef DSTR_H
#define DSTR_H

#include <stdlib.h>

#define DSTR_INITSZ 128

typedef struct {
    char* buf; // The buffer containing the string.
    size_t sz; // The size of the buffer.
    size_t ln; // The number of characters in the buffer.
} Dstr;

// Initialize a `DStr`.
Dstr* dstr_init(void);
// Destroy a `DStr`.
void dstr_destroy(Dstr* dstr);
// Destroy `DStr` structure but preserve `DStr->buf`.
void dstr_destroypsv(Dstr* dstr);

// Append `ln` characters of `src` to `dest`.
void dstr_append(Dstr* dest, char* src, size_t ln);

// Append `ch` to `dest`.
void dstr_appendch(Dstr* dest, char ch);

#endif
