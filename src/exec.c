#include <stdio.h>
#include <string.h>

#include "include/ast.h"
#include "include/exec.h"
#include "include/util.h"

void exec(AST* ast) {
    log_dbg("Started execution.");
    switch (ast->type) {
    case AST_TYPE_CALL: exec_call(ast); break;
    default:            printf("what\n");
    }
}

void exec_call(AST* ast) {
    log_dbg("Started call execution.");
    ASTTypeCall* calldata = (ASTTypeCall*)ast->data;
    if (!strcmp(calldata->to, "+") && calldata->argc == 2) {

        exec_return(1);
        /*
        ASTTypeNum* n1 = (ASTTypeNum*)calldata->argv[0]->data;
        ASTTypeNum* n2 = (ASTTypeNum*)calldata->argv[1]->data;

        exec_return(n1->val + n2->val);
        */
    }
}

void exec_return(int n) { printf("= %d\n", n); }
