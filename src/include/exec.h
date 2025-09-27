#ifndef EXEC_H
#define EXEC_H

#include "ast.h"
#include "scope.h"

// Start executing at the root of the AST. Initialize the `scope`.
AST* exec_start(AST* ast);
// Execute an expression. Delegates to the other executor functions.
AST* exec_exp(AST* ast, Scope* parent);
// Execute the expressions of a block.
AST* exec_block(AST* ast, Scope* parent);
// Execute a call.
AST* exec_call(AST* ast, Scope* parent);
// Execute a definition.
AST* exec_def(AST* ast, Scope* parent);
// Execute a reference.
AST* exec_ref(AST* ast, Scope* parent);
// Execute a function definition.
AST* exec_fdef(AST* ast, Scope* parent);
// Execute a lambda expression.
AST* exec_lambda(size_t argc, AST** argv, AST* exp, Scope* parent);
// Print the result of an execution.
void exec_print(double n);

// Create a new scope and mark it as linked. Also update inherited scope.
void exec_new_scope(AST* ast, Scope* inherit);

// Inherit from another scope and mark it as linked.
void exec_inherit_scope(AST* ast, Scope* inherit);

#endif
