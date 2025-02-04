#include "include/dlist.h"
#include "include/util.h"

#include <stddef.h>
#include <stdio.h>

DList* dlist_init(void) {
    DList* dlist = malloc(sizeof(DList));

    dlist->sz = DLIST_INITSZ;
    dlist->buf = malloc(DLIST_INITSZ);
    dlist->ln = 0;

    return dlist;
}

void dlist_destroy(DList* dlist) {
    free(dlist->buf);
    free(dlist);
}

void dlist_destroypsv(DList* dlist) { free(dlist); }

// Check whether the buffer is about to overflow and resize it if necessary.
void dlist_check_resz(DList* dlist) {
    while ((dlist->ln + 1) * sizeof(void*) >= dlist->sz) {
        // Double the buffer size when overflown.
        dlist->sz *= 2;
        dlist->buf = realloc(dlist->buf, dlist->sz);
        log_dbgf(
            "dlist @ %p doubled from %ld to %ld", dlist, dlist->sz / 2, dlist->sz
        );
    }
}

void dlist_append(DList* dest, void* src) {
    dlist_check_resz(dest);

    dest->buf[dest->ln] = src;
    dest->ln++;
}
