#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "include/ast.h"
#include "include/exec.h"
#include "include/htab.h"
#include "include/stack.h"
#include "include/util.h"

extern AST* root;

ASTNumData exec_start(AST* ast) {
    scope = stack_init();

    // Global scope. Just dummy values for testing.
    HTab* global = htab_init();

    // n = 42.02
    char* name = malloc(2); // TODO: Write a macro for this pattern.
    strcpy(name, "n");
    AST* n = ast_init(
        AST_TYPE_VDEF,
        ast_vdef_data_init(
            name, ast_init(AST_TYPE_NUM, ast_num_data_init(42.02))
        )
    );

    htab_ins(
        global, ((ASTVDefData*)n->data)->name, ((ASTVDefData*)n->data)->val
    );

    // Push global namespace to `scope`.
    stack_push(scope, global);

    return exec_exp(ast);
}

ASTNumData exec_exp(AST* ast) {
    log_dbg("Started execution.");
    switch (ast->type) {
        case AST_TYPE_BLOCK: return exec_block(ast);
        case AST_TYPE_CALL:  return exec_call(ast);
        case AST_TYPE_NUM:   return *(ASTNumData*)ast->data;
        case AST_TYPE_VREF:  return exec_vref(ast);
        case AST_TYPE_VDEF:  return exec_vdef(ast);
        default:             printf("what\n"); exit(1);
    }
}

ASTNumData exec_block(AST* ast) {
    ASTBlockData* block = (ASTBlockData*)ast->data;

    HTab* local = htab_init();
    stack_push(scope, local);

    // Loop through all but last ast.
    for (int i = 0; i < block->ln - 1; i++) exec_exp(block->inside[i]);
    ASTNumData last = exec_exp(block->inside[block->ln - 1]);

    stack_pop(scope);
    htab_destroy(local);

    return last;
}

ASTNumData exec_call(AST* ast) {
    log_dbg("Started call execution.");
    fflush(stdout);
    ASTCallData* calldata = (ASTCallData*)ast->data;
    if (calldata->argc >= 1) {
        if (!strcmp(calldata->to, "sum")) {
            double total = exec_exp(calldata->argv[0]);

            for (size_t i = 1; i < calldata->argc;
                 total += exec_exp(calldata->argv[i++]));

            return total;
        } else if (!strcmp(calldata->to, "sub")) {
            double total = exec_exp(calldata->argv[0]);

            for (size_t i = 1; i < calldata->argc;
                 total -= exec_exp(calldata->argv[i++]));

            return total;
        } else if (!strcmp(calldata->to, "mul")) {
            double total = exec_exp(calldata->argv[0]);

            for (size_t i = 1; i < calldata->argc;
                 total *= exec_exp(calldata->argv[i++]));

            return total;
        } else if (!strcmp(calldata->to, "div")) {
            double total = exec_exp(calldata->argv[0]);

            for (size_t i = 1; i < calldata->argc;
                 total /= exec_exp(calldata->argv[i++]));

            return total;
        }
    }
    return -1000;
}

AST* exec_find(char* name) {
    AST* val = NULL;

    for (int i = scope->ln - 1; i >= 0; i--) {
        HTab* lvl = scope->buf[i];
        val = htab_get(lvl, name);
        if (val != NULL) return val;
    }

    log_dbgf("Could not find var %s", name);
    exit(1);
}

ASTNumData exec_vdef(AST* ast) {
    ASTVDefData* data = (ASTVDefData*)ast->data;
    AST* val = data->val;
    char* key = data->name;
    htab_ins(scope->buf[scope->ln - 1], key, val);
    return exec_exp(val);
}

ASTNumData exec_vref(AST* ast) {
    log_dbg("attempting to reference var");
    ASTVrefData* vref = (ASTVrefData*)ast->data;

    return exec_exp(exec_find(vref->to));
}

void exec_print(double n) { printf("= %lf\n", n); }
