#ifndef EXEC_H
#define EXEC_H

#include "ast.h"
#include "stack.h"

// The `Stack` of `HTab` that makes up the scope of any given `AST`.
extern Stack* scope;

// Start executing at the root of the AST. Initialize the `scope`.
AST* exec_start(AST* ast);
AST* exec_exp(AST* ast);
AST* exec_block(AST* ast);
AST* exec_call(AST* ast);
AST* exec_vref(AST* ast);
AST* exec_vdef(AST* ast);
void exec_print(double n);

#endif
