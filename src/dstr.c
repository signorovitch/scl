#include "include/dstr.h"
#include <string.h>

Dstr* dstr_init(void) {
    Dstr* dstr = malloc(sizeof(Dstr));

    dstr->bufsz = DSTR_INITSZ;
    dstr->buf = malloc(DSTR_INITSZ);
    dstr->ln = 0;

    return dstr;
}

void dstr_destroy(Dstr* dstr) {
    free(dstr->buf);
    free(dstr);
}

void dstr_append(Dstr* dest, size_t ln, char* src) {
    if (dest->ln + ln > dest->bufsz) {
        // Double the buffer size when overflown.
        dest->bufsz *= dest->bufsz;
        dest->buf = realloc(dest->buf, dest->bufsz);
    }

    strcat(dest->buf + ln, src);
}
