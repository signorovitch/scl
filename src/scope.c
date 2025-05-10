#include "include/scope.h"
#include "include/htab.h"
#include "include/util.h"
#include <stdio.h>
#include <stdlib.h>

Scope* scope_init(Scope* inherit) {
    Scope* scope = malloc(sizeof(Scope));

    scope->here = htab_init();
    scope->inherit = inherit;
    scope->uses = 0;

    log_dbgf("%p: new scope, inherits from %p", scope, inherit);

    return scope;
}

void scope_destroy(Scope* scope) {
    if (!scope) return;
    htab_destroy(scope->here);
    if (scope->inherit != NULL) scope_destroy(scope->inherit);
    free(scope);
}

void scope_destroy_psv(Scope* scope) {
    log_dbgf("%p got here", scope);
    if (!scope) return;
    htab_destroy(scope->here);
    scope->inherit = NULL;
    free(scope);
    log_dbgf("%p got here 2", scope);
}

inline void scope_add(Scope* scope, char* key, void* val) {
    htab_ins(scope->here, key, val);
}
