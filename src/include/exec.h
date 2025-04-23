#ifndef EXEC_H
#define EXEC_H

#include "ast.h"
#include "stack.h"

// Start executing at the root of the AST. Initialize the `scope`.
AST* exec_start(AST* ast);
// Execute an expression. Delegates to the other executor functions.
AST* exec_exp(AST* ast);
// Execute the expressions of a block.
AST* exec_block(AST* ast);
// Execute a call.
AST* exec_call(AST* ast);
// Execute a custom function call.
AST* exec_cf(AST* ast, size_t argc, AST** argv);
// Execute a variable reference.
AST* exec_vref(AST* ast);
// Execute a variable definition.
AST* exec_vdef(AST* ast);
// Execute a function definition.
AST* exec_fdef(AST* ast);
// Print the result of an execution.
void exec_print(double n);

#endif
