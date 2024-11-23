#ifndef EXEC_H
#define EXEC_H

#include "ast.h"

void exec(AST* ast);
void exec_call(AST* ast);
void exec_return(int n);

#endif
