#include <stdio.h>
#include <string.h>

#include "include/ast.h"
#include "include/exec.h"
#include "include/util.h"

extern AST* root;

ASTNumData exec_expr(AST* ast) {
    ast_print(ast);
    log_dbg("Started execution.");
    switch (ast->type) {
    case AST_TYPE_CALL: return exec_call(ast);
    case AST_TYPE_NUM:
        exec_print(*(ASTNumData*)ast->data);
        return *(ASTNumData*)ast->data;
    default: printf("what\n");
    }
}

ASTNumData exec_call(AST* ast) {
    log_dbg("Started call execution.");
    fflush(stdout);
    ASTCallData* calldata = (ASTCallData*)ast->data;
    if (!strcmp(calldata->to, "+") && calldata->argc == 2) {

        /*
        ASTNumData* n1 = (ASTNumData*)calldata->argv[0]->data;
        ASTNumData* n2 = (ASTNumData*)calldata->argv[1]->data;
        */

        ASTNumData n1 = exec_expr(calldata->argv[0]);
        ASTNumData n2 = exec_expr(calldata->argv[1]);

        return n1 + n2;
    }
    return -1000;
}

void exec_print(double n) { printf("= %lf\n", n); }
