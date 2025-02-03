#ifndef EXEC_H
#define EXEC_H

#include "ast.h"

ASTNumData exec_exp(AST* ast);
ASTNumData exec_call(AST* ast);
ASTNumData exec_vref(AST* ast);
ASTNumData exec_vdef(AST* ast);
void exec_print(double n);

#endif
