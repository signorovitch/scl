#include <stdio.h>

#include "include/ast.h"
#include "include/dstr.h"
#include "include/util.h"

extern AST* root;

static char* asttype_names[] = {
    [AST_TYPE_CALL] = "FUNC CALL",     [AST_TYPE_NUM] = "NUMBER",
    [AST_TYPE_VREF] = "VAR REFERENCE", [AST_TYPE_VDEF] = "VAR DEFINITION",
    [AST_TYPE_BLOCK] = "BLOCK",        [AST_TYPE_EXC] = "EXCEPTION"
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
        case AST_TYPE_NUM:   ast_num_data_destroy(ast->data); break;
        case AST_TYPE_CALL:  ast_call_data_destroy(ast->data); break;
        case AST_TYPE_VREF:  ast_vref_data_destroy(ast->data); break;
        case AST_TYPE_VDEF:  ast_vdef_data_destroy(ast->data); break;
        case AST_TYPE_BLOCK: ast_block_data_destroy(ast->data); break;
        default:
            log_dbgf("Unknown ast type %d (max: %d)", ast->type, AST_TYPE_MAX);
    }

    free(ast);
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
        case AST_TYPE_CALL:  ast_call_print(ast->data, i + 2); break;
        case AST_TYPE_EXC:   ast_exc_print(ast->data, i + 2); break;
        case AST_TYPE_VREF:  ast_vref_print(ast->data, i + 2); break;
        case AST_TYPE_VDEF:  ast_vdef_print(ast->data, i + 2); break;
        case AST_TYPE_BLOCK: ast_block_print(ast->data, i + 2); break;
        default:             exit(1);
    }
    INDENT_FIELD_NONL_END;
    INDENT_END;
}

ASTNumData* ast_num_data_init(double val) {
    talloc(ASTNumData, num);

    *num = val;

    return num;
}

void ast_num_data_destroy(ASTNumData* num) { free(num); }

void ast_num_print(ASTNumData* data, int i) {
    INDENT_BEGIN(i);

    INDENT_FIELD("data", "%lf", *data);

    INDENT_END;
}

ASTExcData* ast_exc_data_init(char* msg, AST* trace) {
    ASTExcData* data = malloc(sizeof(ASTExcData));
    data->msg = msg;
    data->trace = trace;
    return data;
}

void ast_exc_print(ASTExcData* data, int i) {
    INDENT_BEGIN(i);

    INDENT_TITLE("ASTExcData", data);
    INDENT_FIELD("msg", "\"%s\"", data->msg);
    if (data->trace == NULL) {
        INDENT_FIELD("trace", "%p", NULL)
    } else {
        INDENT_FIELD_EXT_NONL_START("trace");
        ast_print_i(data->trace, i + 1);
        INDENT_FIELD_NONL_END;
    }
    INDENT_END;
}

ASTBIFData* ast_bif_data_init(AST* fn(size_t, AST**)) {
    return (ASTBIFData*)fn;
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
    if (!call) return;
    free(call->to);
    for (size_t i = 0; i < call->argc; i++) ast_destroy(call->argv[i]);
    free(call->argv);
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

ASTVDefData* ast_vdef_data_init(char* name, AST* val) {
    talloc(ASTVDefData, vdef);

    vdef->name = name;
    vdef->val = val;

    return vdef;
}

void ast_vdef_data_destroy(ASTVDefData* vdef) {
    ast_destroy(vdef->val);
    free(vdef->name);
    free(vdef);
}

void ast_vdef_print(ASTVDefData* vdef, int depth) {
    INDENT_BEGIN(depth);

    INDENT_TITLE("ASTVDefData", vdef);
    INDENT_FIELD("name", "%s", vdef->name);
    INDENT_FIELD_EXT_NONL_START("val");
    ast_print_i(vdef->val, depth + 2); // 2 because already indented.
    INDENT_FIELD_NONL_END;

    INDENT_END;
}

ASTVrefData* ast_vref_data_init(char* to) {
    talloc(ASTVrefData, vref);

    vref->to = to;

    return vref;
}

void ast_vref_data_destroy(ASTVrefData* vref) {
    free(vref->to);
    free(vref);
}

void ast_vref_print(ASTVrefData* data, int i) {
    INDENT_BEGIN(i);

    INDENT_TITLE("ASTVrefData", data);
    INDENT_FIELD("to", "%s", data->to);

    INDENT_END;
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

void ast_block_print(ASTBlockData* data, int depth) {
    INDENT_BEGIN(depth);

    INDENT_TITLE("ASTBlockData", data);
    INDENT_FIELD("ln", "%ld", data->ln);
    INDENT_FIELD_LIST("inside", data->inside, data->ln, ast_print_i);

    INDENT_END;
}
