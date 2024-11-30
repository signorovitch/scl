#include <stdio.h>
#include <string.h>

#include "include/ast.h"
#include "include/exec.h"
#include "include/util.h"

extern AST* root;

void exec_expr() {
    ast_print(root);
    log_dbg("Started execution.");
    switch (root->type) {
    case AST_TYPE_CALL: exec_call(); break;
    default:            printf("what\n");
    }
}

void exec_call() {
    log_dbg("Started call execution.");
    fflush(stdout);
    ASTCallData* calldata = (ASTCallData*)root->data;
    if (!strcmp(calldata->to, "+") && calldata->argc == 2) {

        ASTNumData* n1 = (ASTNumData*)calldata->argv[0]->data;
        ASTNumData* n2 = (ASTNumData*)calldata->argv[1]->data;

        exec_return(n1->val + n2->val);
    }
}

void exec_return(int n) { printf("= %d\n", n); }
