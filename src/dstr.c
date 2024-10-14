#include "include/dstr.h"
#include "include/util.h"

#include <string.h>
#include <stdio.h>

Dstr* dstr_init(void) {
    Dstr* dstr = malloc(sizeof(Dstr));

    dstr->bufsz = DSTR_INITSZ;
    dstr->buf = malloc(DSTR_INITSZ);
    *dstr->buf = '\0';
    dstr->ln = 0;

    return dstr;
}

void dstr_destroy(Dstr* dstr) {
    free(dstr->buf);
    free(dstr);
}

void dstr_append(Dstr* dest, char* src, size_t ln) {
    while (dest->ln + ln + 1 > dest->bufsz) {
        // Double the buffer size when overflown.
        dest->bufsz *= 2;
        dest->buf = realloc(dest->buf, dest->bufsz);
        log_dbgf("dstr @ %p doubled from %ld to %ld", dest, dest->bufsz/2, dest->bufsz);
    }

    // Overwrites the \0 at the end of the string, keeps the null from the given
    // string.
    memcpy(dest->buf + dest->ln, src, ln + 1);
    dest->ln += ln;
}

void dstr_appendch(Dstr *dest, char ch) {
    if (dest->ln + 1 + 1 > dest->bufsz) {
        // Double the buffer size when overflown.
        dest->bufsz *= 2;
        dest->buf = realloc(dest->buf, dest->bufsz);
        log_dbgf("dstr @ %p doubled from %ld to %ld", dest, dest->bufsz/2, dest->bufsz);
    }

    // Overwrites the preexisting null terminator, and adds one of its own.
    dest->buf[dest->ln] = ch;
    dest->buf[dest->ln+1] = '\0';
    dest->ln += 1;
}
