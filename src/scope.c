#include "include/scope.h"
#include "include/htab.h"
#include <stdlib.h>

Scope* scope_init(HTab* here, Scope* inherit) {
    Scope* scope = malloc(sizeof(Scope));

    scope->here = here;
    scope->inherit = inherit;

    return scope;
}

void scope_destroy(Scope* scope) {
    htab_destroy(scope->here);
    if (scope->inherit != NULL) scope_destroy(scope->inherit);
    free(scope);
}

void scope_destroy_psv(Scope* scope) {
    htab_destroy(scope->here);
    scope->inherit = NULL;
    free(scope);
}
