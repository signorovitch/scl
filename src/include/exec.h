#ifndef EXEC_H
#define EXEC_H

#include "ast.h"

ASTNumData exec_expr(AST* ast);
ASTNumData exec_call(AST* ast);
void exec_print(double n);

#endif
