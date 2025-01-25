#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "include/ast.h"
#include "include/exec.h"
#include "include/util.h"

extern AST* root;

ASTNumData exec_exp(AST* ast) {
    log_dbg("Started execution.");
    switch (ast->type) {
        case AST_TYPE_CALL: return exec_call(ast);
        case AST_TYPE_NUM:  return *(ASTNumData*)ast->data;
        case AST_TYPE_VREF: return exec_vref(ast);
        default:            printf("what\n");
                            exit(1);
    }
}

ASTNumData exec_call(AST* ast) {
    log_dbg("Started call execution.");
    fflush(stdout);
    ASTCallData* calldata = (ASTCallData*)ast->data;
    if (calldata->argc >= 1) {
    if (!strcmp(calldata->to, "sum")) {
        double total = exec_exp(calldata->argv[0]);

        for (
            size_t i = 1;
            i < calldata->argc;
            total += exec_exp(calldata->argv[i++])
        );

        return total;
    } else if (!strcmp(calldata->to, "sub")) {
        double total = exec_exp(calldata->argv[0]);

        for (
            size_t i = 1;
            i < calldata->argc;
            total -= exec_exp(calldata->argv[i++])
        );

        return total;
    } else if (!strcmp(calldata->to, "mul")) {
        double total = exec_exp(calldata->argv[0]);

        for (
            size_t i = 1;
            i < calldata->argc;
            total *= exec_exp(calldata->argv[i++])
        );

        return total;
    } else if (!strcmp(calldata->to, "div")) {
        double total = exec_exp(calldata->argv[0]);

        for (
            size_t i = 1;
            i < calldata->argc;
            total /= exec_exp(calldata->argv[i++])
        );

        return total;
    }}
    return -1000;
}

ASTNumData exec_vref(AST* ast) {
    return *ast_num_data_init(42.42);
}

void exec_print(double n) { printf("= %lf\n", n); }
