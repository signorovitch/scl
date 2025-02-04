#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "include/ast.h"
#include "include/exec.h"
#include "include/stack.h"
#include "include/util.h"
#include "include/htab.h"

extern AST* root;

ASTNumData exec_start(AST* ast) {
    // The `Stack` of `HTab` that makes up the scope of any given `AST`.
    Stack* scope = stack_init();

    // Global scope. Just dummy values for testing.
    HTab* global = htab_init();

    // n = 42.02
    char* name = malloc(2); // TODO: Write a macro for this pattern.
    strcpy(name, "n");
    AST* n = ast_init(AST_TYPE_VDEF, ast_vdef_data_init(name,
        ast_init(AST_TYPE_NUM, ast_num_data_init(42.02))
    ));

    htab_ins(global, ((ASTVDefData*)n->data)->name, ((ASTVDefData*)n->data)->val);

    // Push global namespace to `scope`.
    stack_push(scope, global);

    return exec_exp(ast, scope);
}

ASTNumData exec_exp(AST* ast, Stack* scope) {
    log_dbg("Started execution.");
    switch (ast->type) {
        case AST_TYPE_CALL: return exec_call(ast, scope);
        case AST_TYPE_NUM:  return *(ASTNumData*)ast->data;
        case AST_TYPE_VREF: return exec_vref(ast, scope);
        case AST_TYPE_VDEF: return exec_vdef(ast, scope);
        default:            printf("what\n");
                            exit(1);
    }
}

ASTNumData exec_call(AST* ast, Stack* scope) {
    log_dbg("Started call execution.");
    fflush(stdout);
    ASTCallData* calldata = (ASTCallData*)ast->data;
    if (calldata->argc >= 1) {
    if (!strcmp(calldata->to, "sum")) {
        double total = exec_exp(calldata->argv[0], scope);

        for (
            size_t i = 1;
            i < calldata->argc;
            total += exec_exp(calldata->argv[i++], scope)
        );

        return total;
    } else if (!strcmp(calldata->to, "sub")) {
        double total = exec_exp(calldata->argv[0], scope);

        for (
            size_t i = 1;
            i < calldata->argc;
            total -= exec_exp(calldata->argv[i++], scope)
        );

        return total;
    } else if (!strcmp(calldata->to, "mul")) {
        double total = exec_exp(calldata->argv[0], scope);

        for (
            size_t i = 1;
            i < calldata->argc;
            total *= exec_exp(calldata->argv[i++], scope)
        );

        return total;
    } else if (!strcmp(calldata->to, "div")) {
        double total = exec_exp(calldata->argv[0], scope);

        for (
            size_t i = 1;
            i < calldata->argc;
            total /= exec_exp(calldata->argv[i++], scope)
        );

        return total;
    }}
    return -1000;
}

ASTNumData exec_vdef(AST* ast, Stack* scope) {
    ASTVDefData* data = (ASTVDefData*) ast->data;
    AST* val = data->val;
    char* key = data->name;
    htab_ins(scope->val[0], key, val);
    return exec_exp(val, scope);
}

ASTNumData exec_vref(AST* ast, Stack* scope) {
    log_dbg("attempting to reference var");
    ASTVrefData* vref = (ASTVrefData*) ast->data;

    char* key = vref->to;
    AST* val;

    val = htab_get(scope->val[0], key);
    if (val != NULL) return exec_exp(val, scope); else log_dbg("didn't find def");

    return *ast_num_data_init(101.0);
}

void exec_print(double n) { printf("= %lf\n", n); }
