#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "include/ast.h"
#include "include/builtin.h"
#include "include/exec.h"
#include "include/htab.h"
#include "include/stack.h"
#include "include/util.h"

AST* exec_find(char* name);

AST* exec_start(AST* ast) {
    log_dbg("Started execution.");

    Stack* scope = stack_init();

    HTab* global = htab_init();

    for (int i = 0; i < BUILTIN_FNS_LN; i++)
        htab_ins(
            global, BUILTIN_FNS[i].name,
            ast_init(AST_TYPE_BIF, ast_bif_data_init(BUILTIN_FNS[i].fn))
        );

    // Push global namespace to `scope`.
    stack_push(scope, global);

    return exec_exp(ast);
}

AST* exec_exp(AST* ast) {
    switch (ast->type) {
        case AST_TYPE_BLOCK: return exec_block(ast);
        case AST_TYPE_CALL:  return exec_call(ast);
        case AST_TYPE_NUM:   return ast;
        case AST_TYPE_VREF:  return exec_vref(ast);
        case AST_TYPE_VDEF:  return exec_vdef(ast);
        case AST_TYPE_FDEF:  return exec_fdef(ast);
        default:             printf("what\n"); exit(1);
    }
}

AST* exec_block(AST* ast) {
    ASTBlockData* block = (ASTBlockData*)ast->data;

    HTab* local = htab_init();
    // stack_push(scope, local);

    // Loop through all but last ast.
    for (int i = 0; i < block->ln - 1; i++) exec_exp(block->inside[i]);
    AST* last = exec_exp(block->inside[block->ln - 1]);

    // stack_pop(scope);
    htab_destroy(local);

    return last;
}

AST* exec_call(AST* ast) {
    log_dbg("Started call execution.");
    ASTCallData* data = (ASTCallData*)ast->data;
    size_t argc = data->argc;
    AST** argv = data->argv;
    char* fname = data->to;

    AST* fdef = exec_find(fname);

    if (fdef == NULL)
        return ast_init(
            AST_TYPE_EXC, ast_exc_data_init("No such function found.", NULL)
        );

    switch (fdef->type) {
        case AST_TYPE_BIF:
            ASTBIFData bifdata = fdef->data;
            return bifdata(argc, argv);
        case AST_TYPE_FDEF: return exec_cf(fdef, argc, argv);
        default:
            return ast_init(AST_TYPE_EXC, ast_exc_data_init("Good job!", NULL));
    }
}

AST* exec_cf(AST* ast, size_t argc, AST** argv) {
    ASTFDefData* fdef = (ASTFDefData*)ast->data;
    for (int i = 0; i < argc; i++) {
        char* key = ((ASTArgData*)fdef->argv[i]->data)->name;
        AST* val = argv[i];
        // htab_ins(scope->buf[scope->ln - 1], key, val);
    }

    return exec_exp(fdef->body);
}

AST* exec_find(char* name) {
    AST* val = NULL;

    /*
    for (int i = scope->ln - 1; i >= 0; i--) {
        HTab* lvl = scope->buf[i];
        val = htab_get(lvl, name);
        if (val != NULL) return val;
        }*/

    return NULL;
}

AST* exec_vdef(AST* ast) {
    ASTVDefData* data = (ASTVDefData*)ast->data;
    AST* val = data->val;
    char* key = data->name;
    // htab_ins(scope->buf[scope->ln - 1], key, val);
    return exec_exp(val);
}

AST* exec_vref(AST* ast) {
    log_dbg("attempting to reference var");
    ASTVrefData* vref = (ASTVrefData*)ast->data;

    AST* found = exec_find(vref->to);

    if (found == NULL) {
        // TODO: Better memory management here.
        static char msg[256];
        snprintf(
            msg, sizeof(msg), "Could not find value in scope for `%s`.",
            vref->to
        );
        return ast_init(AST_TYPE_EXC, ast_exc_data_init(msg, NULL));
    }

    return exec_exp(found);
}

AST* exec_fdef(AST* ast) {
    ASTFDefData* fdef = (ASTFDefData*)ast->data;
    AST* val = fdef->body;
    char* key = fdef->name;
    // htab_ins(scope->buf[scope->ln - 1], key, val);
    return val; // Function definitions return function body.
}

void exec_print(double n) { printf("= %lf\n", n); }
