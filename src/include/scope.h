#ifndef SCOPE_H
#define SCOPE_H

#include "htab.h"

// Represents the reverse linked tree of scope.
typedef struct SCOPE_T {
    HTab* here;
    struct SCOPE_T* inherit;
} Scope;

// Create a new `Scope`.
Scope* scope_init(HTab* here, Scope* inherit);
// Destroy all linked `Scope`s this inherits from.
void scope_destroy(Scope* scope);
// Destroy the current `Scope` only.
void scope_destroy_psv(Scope *scope);

#endif
