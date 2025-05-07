#include "include/scope.h"
#include "include/htab.h"
#include <stdlib.h>

Scope* scope_init(Scope* inherit) {
    Scope* scope = malloc(sizeof(Scope));

    scope->here = htab_init();
    scope->inherit = inherit;

    return scope;
}

void scope_destroy(Scope* scope) {
    if (!scope) return;
    htab_destroy(scope->here);
    if (scope->inherit != NULL) scope_destroy(scope->inherit);
    free(scope);
}

void scope_destroy_psv(Scope* scope) {
    if (!scope) return;
    htab_destroy(scope->here);
    scope->inherit = NULL;
    free(scope);
}

inline void scope_add(Scope* scope, char* key, void* val) {
    htab_ins(scope->here, key, val);
}
