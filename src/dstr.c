#include "include/dstr.h"
#include "include/util.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

Dstr* dstr_init(void) {
    Dstr* dstr = malloc(sizeof(Dstr));

    dstr->sz = DSTR_INITSZ;
    dstr->buf = malloc(DSTR_INITSZ);
    *dstr->buf = '\0';
    dstr->ln = 0;

    return dstr;
}

void dstr_destroy(Dstr* dstr) {
    free(dstr->buf);
    free(dstr);
}

// Check whether the buffer is overflowing and resize it if necessary.
void check_resz(Dstr* dstr, size_t ln) {
    while (dstr->ln + ln + 1 > dstr->sz) {
        // Double the buffer size when overflown.
        dstr->sz *= 2;
        dstr->buf = realloc(dstr->buf, dstr->sz);
        log_dbgf(
            "dstr @ %p doubled from %ld to %ld", dstr, dstr->sz / 2,
            dstr->sz
        );
    }
}

void dstr_append(Dstr* dest, char* src, size_t ln) {
    check_resz(dest, ln);

    // Overwrites the \0 at the end of the string, keeps the null from the given
    // string.
    memcpy(dest->buf + dest->ln, src, ln + 1);
    dest->ln += ln;
}

void dstr_appendch(Dstr* dest, char ch) {
    check_resz(dest, 1);

    // Overwrites the preexisting null terminator, and adds one of its own.
    dest->buf[dest->ln] = ch;
    dest->buf[dest->ln + 1] = '\0';
    dest->ln += 1;
}
