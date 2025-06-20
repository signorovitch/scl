#include <inttypes.h>
#include <stdio.h>

#include "include/ast.h"
#include "include/dstr.h"
#include "include/gc.h"
#include "include/scope.h"
#include "include/util.h"

extern AST* root;

static char* asttype_names[] = {
    [AST_TYPE_CALL] = "FUNC CALL",
    [AST_TYPE_NUM] = "NUMBER",
    [AST_TYPE_VREF] = "VAR REFERENCE",
    [AST_TYPE_VDEF] = "VAR DEFINITION",
    [AST_TYPE_BLOCK] = "BLOCK",
    [AST_TYPE_EXC] = "EXCEPTION",
    [AST_TYPE_FDEF] = "FUNCTION DEFINITION",
    [AST_TYPE_ARG] = "DEFINITION ARGUMENT"
};

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
        case AST_TYPE_NUM:   ast_num_data_destroy(ast->data); break;
        case AST_TYPE_CALL:  ast_call_data_destroy(ast->data); break;
        case AST_TYPE_VREF:  ast_vref_data_destroy(ast->data); break;
        case AST_TYPE_VDEF:  ast_vdef_data_destroy(ast->data); break;
        case AST_TYPE_BLOCK: ast_block_data_destroy(ast->data); break;
        case AST_TYPE_FDEF:  ast_fdef_data_destroy(ast->data); break;
        case AST_TYPE_ARG:   ast_arg_data_destroy(ast->data); break;
        default:
            log_dbgf("Unknown ast type %d (max: %d)", ast->type, AST_TYPE_MAX);
    }

    // If there're no more `AST`s linked to the scope, free.
    if (ast->scope && !--ast->scope->uses) scope_destroy_psv(ast->scope);

    free(ast);
}

void ast_destroy_psv(AST* ast) {
    if (!ast) return;

    switch (ast->type) {
        case AST_TYPE_NUM:   ast_num_data_destroy(ast->data); break;
        case AST_TYPE_CALL:  ast_call_data_destroy_psv(ast->data); break;
        case AST_TYPE_VREF:  ast_vref_data_destroy(ast->data); break;
        case AST_TYPE_VDEF:  ast_vdef_data_destroy_psv(ast->data); break;
        case AST_TYPE_BLOCK: ast_block_data_destroy_psv(ast->data); break;
        case AST_TYPE_FDEF:  ast_fdef_data_destroy_psv(ast->data); break;
        case AST_TYPE_ARG:   ast_arg_data_destroy(ast->data); break;
        case AST_TYPE_BIF:   ast_bif_data_destroy(ast->data); break;
        case AST_TYPE_EXC:   ast_exc_data_destroy(ast->data); break;
        default:
            log_dbgf("Unknown ast type %d (max: %d)", ast->type, AST_TYPE_MAX);
    }

    free(ast);
}

void ast_print(AST* ast) {
    if (!ast) return;
    ast_print_i(ast, 0);
}

void ast_print_i(AST* ast, int i) {
    INDENT_BEGIN(i);

    INDENT_TITLE("AST", ast);
    INDENT_FIELD("type", "%s", asttype_names[ast->type]);
    INDENT_FIELD("scope", "%p", ast->scope);
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
        case AST_TYPE_FDEF:  ast_fdef_print(ast->data, i + 2); break;
        case AST_TYPE_ARG:   ast_arg_print(ast->data, i + 2); break;
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

ASTBIFData* ast_bif_data_init(AST* fn(size_t, AST**, Scope*)) {
    return (ASTBIFData*)fn;
}

void ast_bif_data_destroy(ASTBIFData* bif) { return; }

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

void ast_call_data_destroy_psv(ASTCallData* call) {
    if (!call) return;
    free(call->to);
    call->to = NULL;
    free(call->argv);
    call->argv = NULL;
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

void ast_vdef_data_destroy_psv(ASTVDefData* vdef) {
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

void ast_block_data_destroy_psv(ASTBlockData* block) {
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

void ast_fdef_print(ASTFDefData* fdef, int i) {
    INDENT_BEGIN(i)
    INDENT_TITLE("ASTFDefData", fdef);
    INDENT_FIELD("name", "%s", fdef->name);
    INDENT_FIELD("argc", "%ld", fdef->argc);
    INDENT_FIELD_LIST("argv", fdef->argv, fdef->argc, ast_print_i);
    INDENT_FIELD_EXT_NONL_START("body");
    ast_print_i(fdef->body, i + 2);
    INDENT_FIELD_NONL_END;
    INDENT_END;
}

ASTArgData* ast_arg_data_init(char* name) {
    ASTArgData* arg = malloc(sizeof(ASTArgData));
    arg->name = name;
    return arg;
}

void ast_arg_data_destroy(ASTArgData* arg) { free(arg->name); }

void ast_arg_print(ASTArgData* arg, int i) {
    INDENT_BEGIN(i);
    INDENT_TITLE("ASTArgData", arg);
    INDENT_FIELD("name", "%s", arg->name);
    INDENT_END;
}

AST* ast_find(Scope* scope, char* name) {
    while (scope) {
        AST* gotten = htab_get(scope->here, name);
        if (gotten) return gotten;
        else scope = scope->inherit;
    }

    return NULL;
}
