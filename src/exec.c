#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "include/ast.h"
#include "include/exec.h"
#include "include/util.h"

extern AST* root;

ASTNumData exec_expr(AST* ast) {
    log_dbg("Started execution.");
    switch (ast->type) {
        case AST_TYPE_CALL: return exec_call(ast);
        case AST_TYPE_NUM:  return *(ASTNumData*)ast->data;
        default:            printf("what\n");
    }
}

ASTNumData exec_call(AST* ast) {
    log_dbg("Started call execution.");
    fflush(stdout);
    ASTCallData* calldata = (ASTCallData*)ast->data;
    if (calldata->argc >= 1) {
    if (!strcmp(calldata->to, "sum")) {
        double total = exec_expr(calldata->argv[0]);

        for (
            size_t i = 1;
            i < calldata->argc;
            total += exec_expr(calldata->argv[i++])
        );

        return total;
    } else if (!strcmp(calldata->to, "sub")) {
        double total = exec_expr(calldata->argv[0]);

        for (
            size_t i = 1;
            i < calldata->argc;
            total -= exec_expr(calldata->argv[i++])
        );

        return total;
    } else if (!strcmp(calldata->to, "mul")) {
        double total = exec_expr(calldata->argv[0]);

        for (
            size_t i = 1;
            i < calldata->argc;
            total *= exec_expr(calldata->argv[i++])
        );

        return total;
    } else if (!strcmp(calldata->to, "div")) {
        double total = exec_expr(calldata->argv[0]);

        for (
            size_t i = 1;
            i < calldata->argc;
            total /= exec_expr(calldata->argv[i++])
        );

        return total;
    }}
    return -1000;
}

void exec_print(double n) { printf("= %lf\n", n); }
