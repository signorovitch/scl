#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "include/ast.h"
#include "include/builtin.h"
#include "include/dlist.h"
#include "include/exec.h"
#include "include/htab.h"
#include "include/scope.h"
#include "include/util.h"

AST* exec_start(AST* ast) {
    log_dbg("Started execution.");

    if (!ast) return ast;

    Scope* global = scope_init(NULL);
    global->uses = 1;

    // Keep track of built-in function ASTs, as they arent part of the main
    // tree.
    // DList* builtins = dlist_init();

    for (int i = 0; i < BUILTIN_FNS_LN; i++) {

        // AST* builtin_ast =
        // ast_init(AST_TYPE_BIF, ast_bif_data_init(BUILTIN_FNS[i].fn));

        // dlist_append(builtins, builtin_ast);

        htab_ins(
            global->here, BUILTIN_FNS[i].name,
            ast_init(AST_TYPE_BIF, ast_bif_data_init(BUILTIN_FNS[i].fn))
        );
    }

    log_dbg("Completed startup sequence.");

    AST* res = exec_exp(ast, global);

    // Clean up built-in function ASTs.
    /*for (int i = 0; i < builtins->ln; i++) ast_destroy(builtins->buf[i]);
    dlist_destroy(builtins);
    */

    // scope_destroy_psv(global);
    return res;
}

AST* exec_exp(AST* ast, Scope* parent) {
    switch (ast->type) {
        case AST_TYPE_BLOCK: return exec_block(ast, parent);
        case AST_TYPE_CALL:  return exec_call(ast, parent);
        case AST_TYPE_NUM:
            return ast_init(
                AST_TYPE_NUM, ast_num_data_init(*(ASTNumData*)ast->data)
            );
        case AST_TYPE_VREF: return exec_vref(ast, parent);
        case AST_TYPE_VDEF: return exec_vdef(ast, parent);
        case AST_TYPE_FDEF: return exec_fdef(ast, parent);
        default:            printf("what\n"); exit(1);
    }
}

AST* exec_block(AST* ast, Scope* parent) {
    ASTBlockData* block = (ASTBlockData*)ast->data;

    // Blocks create their own scope, shared among their expressions.
    // ast->scope = scope_init(parent);

    // HERE
    exec_new_scope(ast, parent);

    // Loop through all but last ast.
    for (int i = 0; i < block->ln - 1; i++)
        exec_exp(block->inside[i], ast->scope);
    AST* last = exec_exp(block->inside[block->ln - 1], ast->scope);

    return last;
}

AST* exec_call(AST* ast, Scope* parent) {
    log_dbg("Started call execution.");
    ASTCallData* data = (ASTCallData*)ast->data;
    size_t argc = data->argc;
    AST** argv = data->argv;
    char* fname = data->to;

    ast->scope = parent;

    AST* fdef = ast_find(ast->scope, fname);

    if (fdef == NULL)
        return ast_init(
            AST_TYPE_EXC, ast_exc_data_init("No such function found.", NULL)
        );

    switch (fdef->type) {
        case AST_TYPE_BIF:
            ASTBIFData bifdata = fdef->data;
            return bifdata(argc, argv, parent);
        case AST_TYPE_FDEF: return exec_cf(fdef, argc, argv);
        default:
            return ast_init(AST_TYPE_EXC, ast_exc_data_init("Good job!", NULL));
    }
}

AST* exec_cf(AST* ast, size_t argc, AST** argv) {
    Scope* callscope = scope_init(ast->scope);
    ASTFDefData* fdef = (ASTFDefData*)ast->data;
    for (int i = 0; i < argc; i++) {
        char* key = ((ASTArgData*)fdef->argv[i]->data)->name;
        AST* val = argv[i];
        scope_add(callscope, key, val);
    }

    return exec_exp(fdef->body, callscope);
}

AST* exec_vdef(AST* ast, Scope* parent) {
    // Use parent's scope.
    // ast->scope = parent;

    // HERE
    exec_inherit_scope(ast, parent);

    ASTVDefData* data = (ASTVDefData*)ast->data;
    AST* val = data->val;
    char* key = data->name;
    scope_add(parent, key, val); // Add variable definition to parent scope.
    return exec_exp(val, parent);
}

AST* exec_vref(AST* ast, Scope* parent) {
    // Use parent's scope.
    // ast->scope = parent;

    // HERE
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

    return exec_exp(found, ast->scope);
}

AST* exec_fdef(AST* ast, Scope* parent) {
    ast->scope = scope_init(parent);
    ASTFDefData* fdef = (ASTFDefData*)ast->data;
    AST* val = fdef->body;
    char* key = fdef->name;
    scope_add(parent, key, val);
    return val; // Function definitions return function body.
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
