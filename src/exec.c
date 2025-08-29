#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "include/ast.h"
#include "include/builtin.h"
#include "include/exec.h"
#include "include/htab.h"
#include "include/scope.h"
#include "include/util.h"

AST* exec_start(AST* ast) {
    log_dbg("Started execution.");

    if (!ast) return ast;

    Scope* global = scope_init(NULL);
    global->uses = 1;

    for (int i = 0; i < BUILTIN_FNS_LN; i++)
        htab_ins(
            global->here, BUILTIN_FNS[i].name,
            ast_init(AST_TYPE_BIF, ast_bif_data_init(BUILTIN_FNS[i].fn))
        );

    log_dbg("Completed startup sequence.");

    AST* res = exec_exp(ast, global);

    return res;
}

AST* exec_exp(AST* ast, Scope* parent) {
    switch (ast->type) {
        case AST_TYPE_BLOCK: return exec_block(ast, parent);
        case AST_TYPE_CALL:  return exec_call(ast, parent);
        case AST_TYPE_LIT_NUM:
            return ast_init(
                AST_TYPE_LIT_NUM, ast_num_data_init(*(ASTNumData*)ast->data)
            );
        case AST_TYPE_LIT_BOOL:
            return ast_init(
                AST_TYPE_LIT_BOOL, ast_bool_data_init(*(ASTBoolData*)ast->data)
            );
        case AST_TYPE_VREF:   return exec_vref(ast, parent);
        case AST_TYPE_VDEF:   return exec_vdef(ast, parent);
        case AST_TYPE_FDEF:   return exec_fdef(ast, parent);
        case AST_TYPE_BIF:
        case AST_TYPE_LAMBDA: return ast;
        default:              printf("what\n"); exit(1);
    }
}

AST* exec_block(AST* ast, Scope* parent) {
    ASTBlockData* block = (ASTBlockData*)ast->data;

    exec_new_scope(ast, parent);

    // Loop through all but last ast.
    for (int i = 0; i < block->ln - 1; i++)
        exec_exp(block->inside[i], ast->scope);
    AST* last = exec_exp(block->inside[block->ln - 1], ast->scope);

    return last;
}

AST* exec_call(AST* ast, Scope* parent) {
    ASTCallData* calldata = (ASTCallData*)ast->data;

    AST* exp = exec_exp(calldata->exp, parent);

    switch (exp->type) {
        case AST_TYPE_BIF:
            ASTBIFData bifdata = exp->data;
            return bifdata(calldata->argc, calldata->argv, parent);
        case AST_TYPE_LAMBDA:
            return exec_lambda(calldata->argc, calldata->argv, exp, parent);
        default:
            return ast_init(
                AST_TYPE_EXC, ast_exc_data_init("Uncallable.", NULL)
            );
    }
}

AST* exec_vdef(AST* ast, Scope* parent) {
    // Use parent's scope.
    exec_inherit_scope(ast, parent);

    ASTVDefData* data = (ASTVDefData*)ast->data;
    AST* val = data->exp;
    char* key = data->name;
    scope_add(parent, key, val); // Add variable definition to parent scope.
    return exec_exp(val, parent);
}

AST* exec_vref(AST* ast, Scope* parent) {
    // Use parent's scope.
    exec_inherit_scope(ast, parent);
    log_dbg("attempting to reference var");
    ASTVrefData* vref = (ASTVrefData*)ast->data;

    AST* found = ast_find(parent, vref->to);

    if (found == NULL) {
        // TODO: Better memory management here.
        static char msg[256];
        snprintf(
            msg, sizeof(msg), "Could not find value in scope for `%s`.",
            vref->to
        );
        return ast_init(AST_TYPE_EXC, ast_exc_data_init(msg, NULL));
    }

    // return exec_exp(found, ast->scope);
    return found;
}

AST* exec_fdef(AST* ast, Scope* parent) {
    ast->scope = scope_init(parent);
    ASTFDefData* fdef = (ASTFDefData*)ast->data;
    AST* val = ast;
    char* key = fdef->name;
    scope_add(parent, key, val);
    return fdef->body; // Function definitions return function body.
}

AST* exec_lambda(size_t argc, AST** argv, AST* exp, Scope* parent) {
    Scope* callscope = scope_init(parent);
    ASTLambdaData* lambda = (ASTLambdaData*)exp->data;
    for (int i = 0; i < argc; i++) {
        char* key = ((ASTArgData*)lambda->parv[i]->data)->name;
        AST* val = exec_exp(argv[i], parent);
        scope_add(callscope, key, val);
    }

    return exec_exp(lambda->body, callscope);
}

void exec_print(double n) { printf("= %lf\n", n); }

inline void exec_new_scope(AST* ast, Scope* inherit) {
    Scope* scope = scope_init(inherit);
    ast->scope = scope;

    // Update linked status.
    scope->uses++;
}

inline void exec_inherit_scope(AST* ast, Scope* inherit) {
    ast->scope = inherit;

    // Update uses.
    inherit->uses++;
}
