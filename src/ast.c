#include <stdio.h>

#include "include/ast.h"
#include "include/dstr.h"
#include "include/util.h"

extern AST* root;

static char* asttype_names[] = {
    [AST_TYPE_CALL] = "FUNC CALL",
    [AST_TYPE_NUM] = "NUMBER",
    [AST_TYPE_VREF] = "VAR REFERENCE"
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
        default:
            log_dbgf("Unknown ast type %d (max: %d)", ast->type, AST_TYPE_MAX);
    }
}

void ast_print(AST* ast) { ast_print_i(ast, 0); }

void ast_print_i(AST* ast, int i) {
    INDENT_BEGIN(i);

    INDENT_TITLE("AST", ast);
    INDENT_FIELD("type", "%s", asttype_names[ast->type]);
    INDENT_FIELD_EXT_NONL_START("data");
    switch (ast->type) {
        case AST_TYPE_NUM:
            printf("%s  %lf\n", INDENT_spacing->buf, *(ASTNumData*)ast->data);
            break;
        case AST_TYPE_CALL: ast_call_print(ast->data, i + 2); break;
        case AST_TYPE_VREF: ast_vref_print(ast->data, i + 2); break;
        default:            exit(1);
    }
    INDENT_FIELD_NONL_END;
    INDENT_END;
}

ASTNumData* ast_num_data_init(double val) {
    talloc(ASTNumData, num);

    log_dbgf("val: %lf", val);

    *num = val;

    return num;
}

void ast_num_data_destroy(ASTNumData* num) {
    if (!num) return free(num);
}

void ast_num_print(ASTNumData* data, int i) {
    INDENT_BEGIN(i);

    INDENT_FIELD("data", "%lf", *data);

    INDENT_END;
}

ASTCallData* ast_call_data_init(char* to, size_t argc, AST** argv) {
    talloc(ASTCallData, call);

    log_dbgf("to: %s", to);

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

void ast_call_print(ASTCallData* data, int i) {
    INDENT_BEGIN(i);

    INDENT_TITLE("ASTCallData", data);
    INDENT_FIELD("to", "%s", data->to);
    INDENT_FIELD("argc", "%ld", data->argc);
    INDENT_FIELD_LIST("argv", data->argv, data->argc, ast_print_i);

    INDENT_END;
}

ASTVrefData* ast_vref_data_init(char* to) {
    
}

void ast_vref_data_destroy(ASTVrefData* vref) {}

void ast_vref_print(ASTVrefData* data, int i) {
    INDENT_BEGIN(i);

    INDENT_TITLE("ASTVrefData", data);
    INDENT_FIELD("to", "%s", data->to);

    INDENT_END;
}
