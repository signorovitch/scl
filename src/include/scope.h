#ifndef SCOPE_H
#define SCOPE_H

#include "htab.h"

// Represents the reverse linked tree of scope.
typedef struct SCOPE_T {
    HTab* here; // This scope's hash table.
    struct SCOPE_T* inherit; // The scope to inherit from.
    int uses; // How many `AST`s are linked to this scope.
} Scope;

// Create a new `Scope`. Creates new empty `HTab` for current scope.
Scope* scope_init(Scope* inherit);
// Destroy all linked `Scope`s this inherits from.
void scope_destroy(Scope* scope);
// Destroy the current `Scope` only.
void scope_destroy_psv(Scope *scope);
// Insert a key/val pair into the `HTab` of a `Scope`.
void scope_add(Scope* scope, char* key, void* val);

#endif
