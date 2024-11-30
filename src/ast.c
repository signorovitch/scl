#include <stdio.h>

#include "include/ast.h"
#include "include/util.h"

extern AST* root;

static char* asttype_names[] = {
    [AST_TYPE_CALL] = "CALL",
    [AST_TYPE_NUM] = "NUMBER",
};

AST* ast_init(ASTType type, void* data) {
    AST* ast = malloc(sizeof(AST));

    ast->type = type;
    ast->data = data;

    return ast;
}

void ast_destroy(AST* ast) {
    if (!ast) return;

    switch (ast->type) {
    case AST_TYPE_NUM:  ast_num_data_destroy(ast->data); break;
    case AST_TYPE_CALL: ast_call_data_destroy(ast->data); break;
    default:            log_dbgf("Unknown ast type %d (max: %d)", ast->type, AST_TYPE_MAX);
    }
}

void ast_print(AST* ast) {
    log_dbgf("Tree type: %s", asttype_names[ast->type]);
    fflush(stdout);
}


ASTNumData* ast_num_data_init(double val) {
    talloc(ASTNumData, num);

    *num = val;

    return num;
}

void ast_num_data_destroy(ASTNumData* num) {
    if (!num) return free(num);
}

ASTCallData* ast_call_data_init(char* to, size_t argc, AST** argv) {
    talloc(ASTCallData, call);

    call->to = to;
    call->argc = argc;
    call->argv = argv;

    return call;
}

void ast_call_data_destroy(ASTCallData* call) {
    if (!call) return free(call->to);
    for (size_t i = 0; i < call->argc; i++) free(call->argv[i]);
    free(call);
}
