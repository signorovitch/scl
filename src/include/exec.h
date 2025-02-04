#ifndef EXEC_H
#define EXEC_H

#include "ast.h"
#include "stack.h"


// Start executing at the root of the AST. Initialize the `scope`.
ASTNumData exec_start(AST* ast);
ASTNumData exec_exp(AST* ast, Stack* scope);
ASTNumData exec_block(AST* ast, Stack* scope);
ASTNumData exec_call(AST* ast, Stack* scope);
ASTNumData exec_vref(AST* ast, Stack* scope);
ASTNumData exec_vdef(AST* ast, Stack* scope);
void exec_print(double n);

#endif
