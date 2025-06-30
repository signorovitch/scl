#include <inttypes.h>
#include <stdio.h>

#include "include/ast.h"
#include "include/gc.h"
#include "include/scope.h"
#include "include/util.h"

extern AST* root;

AST* ast_init(ASTType type, void* data) {
    AST* ast = gc_alloc(sizeof(AST), GC_TYPE_AST);

    ast->type = type;
    ast->data = data;
    ast->scope = NULL;

    if (ast->type > AST_TYPE_MAX) {
        log_dbgf(
            "Attempted to create invalid AST (%i > %i) to GC: ast:%p",
            ast->type, AST_TYPE_MAX, ast
        );
    }

    return ast;
}

AST* ast_init_scope(ASTType type, void* data, Scope* scope) {
    AST* ast = malloc(sizeof(AST));

    ast->type = type;
    ast->data = data;
    ast->scope = scope;

    return ast;
}

void ast_destroy(AST* ast) {
    if (!ast) return;

    switch (ast->type) {
        case AST_TYPE_NUM:    ast_num_data_destroy(ast->data); break;
        case AST_TYPE_CALL:   ast_call_data_destroy(ast->data); break;
        case AST_TYPE_VREF:   ast_vref_data_destroy(ast->data); break;
        case AST_TYPE_VDEF:   ast_vdef_data_destroy(ast->data); break;
        case AST_TYPE_BLOCK:  ast_block_data_destroy_psv(ast->data); break;
        case AST_TYPE_FDEF:   ast_fdef_data_destroy_psv(ast->data); break;
        case AST_TYPE_ARG:    ast_arg_data_destroy(ast->data); break;
        case AST_TYPE_BIF:    ast_bif_data_destroy(ast->data); break;
        case AST_TYPE_EXC:    ast_exc_data_destroy(ast->data); break;
        case AST_TYPE_LAMBDA: ast_lambda_data_destroy(ast->data); break;
        default:
            log_dbgf("Unknown ast type %d (max: %d)", ast->type, AST_TYPE_MAX);
    }

    free(ast);
}

ASTNumData* ast_num_data_init(double val) {
    talloc(ASTNumData, num);

    *num = val;

    return num;
}

void ast_num_data_destroy(ASTNumData* num) { free(num); }

ASTExcData* ast_exc_data_init(const char* msg, AST* trace) {
    ASTExcData* data = malloc(sizeof(ASTExcData));
    data->msg = msg;
    data->trace = trace;
    return data;
}

void ast_exc_data_destroy(ASTExcData* exc) {
    // `msg` is static, and `trace` will get freed in GC.
    free(exc);
}

ASTBIFData* ast_bif_data_init(AST* fn(size_t, AST**, Scope*)) {
    return (ASTBIFData*)fn;
}

void ast_bif_data_destroy(ASTBIFData* bif) { return; }

// Lambda.

ASTLambdaData* ast_lambda_data_init(size_t parc, AST** parv, AST* body) {
    talloc(ASTLambdaData, lambda);

    lambda->parc = parc;
    lambda->parv = parv;
    lambda->body = body;

    return lambda;
}

void ast_lambda_data_destroy(ASTLambdaData* lambda) {
    free(lambda->parv);
    free(lambda);
}

// Call.

ASTCallData* ast_call_data_init(size_t argc, AST** argv, AST* to) {
    talloc(ASTCallData, call);

    call->to = to;
    call->argc = argc;
    call->argv = argv;

    return call;
}

void ast_call_data_destroy(ASTCallData* call) {
    if (!call) return;
    free(call->argv);
    free(call);
}

// VDef.

ASTVDefData* ast_vdef_data_init(char* name, AST* exp) {
    talloc(ASTVDefData, vdef);

    vdef->name = name;
    vdef->exp = exp;

    return vdef;
}

void ast_vdef_data_destroy(ASTVDefData* vdef) {
    free(vdef->name);
    free(vdef);
}

// VRef.

ASTVrefData* ast_vref_data_init(char* to) {
    talloc(ASTVrefData, vref);

    vref->to = to;

    return vref;
}

void ast_vref_data_destroy(ASTVrefData* vref) {
    free(vref->to);
    free(vref);
}

ASTBlockData* ast_block_data_init(AST** inside, size_t ln) {
    ASTBlockData* block = malloc(sizeof(ASTBlockData));

    block->inside = inside;
    block->ln = ln;

    return block;
}

void ast_block_data_destroy(ASTBlockData* block) {
    for (size_t i = 0; i < block->ln; i++) { ast_destroy(block->inside[i]); }

    free(block->inside);
    free(block);
}

void ast_block_data_destroy_psv(ASTBlockData* block) {
    free(block->inside);
    free(block);
}

ASTFDefData*
ast_fdef_data_init(char* name, size_t argc, AST** argv, AST* body) {
    ASTFDefData* fdef = malloc(sizeof(ASTFDefData));

    fdef->name = name;
    fdef->argc = argc;
    fdef->argv = argv;
    fdef->body = body;

    return fdef;
}

void ast_fdef_data_destroy(ASTFDefData* fdef) {
    free(fdef->name);
    for (int i = 0; i < fdef->argc; ast_destroy(fdef->argv[i++]));
    ast_destroy(fdef->body);
}

void ast_fdef_data_destroy_psv(ASTFDefData* fdef) {
    free(fdef->name);
    free(fdef->argv);
    free(fdef);
}

ASTArgData* ast_arg_data_init(char* name) {
    ASTArgData* arg = malloc(sizeof(ASTArgData));
    arg->name = name;
    return arg;
}

void ast_arg_data_destroy(ASTArgData* arg) { free(arg->name); }

AST* ast_find(Scope* scope, char* name) {
    while (scope) {
        AST* gotten = htab_get(scope->here, name);
        if (gotten) return gotten;
        else scope = scope->inherit;
    }

    return NULL;
}
