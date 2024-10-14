#ifndef DSTR_H
#define DSTR_H

#include <stdlib.h>

#define DSTR_INITSZ 128

typedef struct {
    char* buf;    // The buffer containing the string.
    size_t bufsz; // The size of the buffer.
    size_t ln;    // The number of characters in the buffer.
} Dstr;

Dstr* dstr_init(void);
void dstr_destroy(Dstr* dstr);

// Append ln characters of src to dest.
void dstr_append(Dstr* dest, char* src, size_t ln);

// Append ch to dest.
void dstr_appendch(Dstr* dest, char ch);

#endif
