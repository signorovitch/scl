#include "include/gc.h"
#include "include/ast.h"
#include "include/scope.h"
#include "include/util.h"
#include <stdio.h>

#include <stddef.h>

GC* gclist = NULL;

void gc_destroy(GC* gc) { free(gc); }

void f() {}

void* gc_alloc(size_t sz, GCType type) {
    void* mem = malloc(sz);
    GC* gc = malloc(sizeof(GC));

    gc->p = mem;
    gc->type = type;
    gc->marked = false;
    gc->nxt = gclist;
    gclist = gc;

    return mem;
}

void gc_hack_free() {
    while (gclist) {
        GC* gc = gclist;
        gclist = gclist->nxt;
        switch (gc->type) {
            case GC_TYPE_AST:
                f();
                if (((AST*)gc->p)->type > AST_TYPE_MAX) {
                    log_dbgf(
                        "Attempted to free invalid AST (%i > %i) to GC: gc:%p "
                        "ast:%p",
                        ((AST*)gc->p)->type, AST_TYPE_MAX, gc, gc->p
                    );
                }
                ast_destroy_psv(gc->p);
                break;
            case GC_TYPE_SCOPE: scope_destroy_psv(gc->p); break;
        }
        gc_destroy(gc);
    }
}
