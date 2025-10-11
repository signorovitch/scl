#include "include/ast_print.h"
#include "include/ast.h"
#include "include/builtin.h"
#include "include/dstr.h"
#include "include/util.h"
#include <stdio.h>

static char* asttype_names[] = {
    [AST_TYPE_CALL] = "CALL",
    [AST_TYPE_LIT_NUM] = "LITERAL NUMBER",
    [AST_TYPE_LIT_BOOL] = "LITERAL BOOLEAN",
    [AST_TYPE_REF] = "REFERENCE",
    [AST_TYPE_DEF] = "DEFINITION",
    [AST_TYPE_BLOCK] = "BLOCK",
    [AST_TYPE_EXC] = "EXCEPTION",
    [AST_TYPE_ARG] = "DEFINITION ARGUMENT",
    [AST_TYPE_LAMBDA] = "LAMBDA",
    [AST_TYPE_BIF] = "BUILTIN FUNCTION",
    [AST_TYPE_FORCE] = "FORCE",
    [AST_TYPE_PRESERVE] = "PRESERVE"
};

void ast_print(AST* ast) {
    if (!ast) return;
    ast_print_i(ast, 0);
}

void ast_print_i(AST* ast, int i) {
    INDENT_BEGIN(i);

    INDENT_TITLE("AST", ast);
    INDENT_FIELD("type", "%s", asttype_names[ast->type]);
    INDENT_FIELD_EXT_NONL_START("data");
    switch (ast->type) {
        case AST_TYPE_LIT_NUM:
            printf("%s  %lf\n", INDENT_spacing->buf, *(ASTNumData*)ast->data);
            break;
        case AST_TYPE_LIT_BOOL:
            printf(
                "%s  %s\n", INDENT_spacing->buf,
                *(ASTBoolData*)ast->data ? "true" : "false"
            );
            break;
        case AST_TYPE_LIT_KIND:
            printf(
                "%s  %s\n", INDENT_spacing->buf,
                ast_lit_kind_names[*(ASTKindData*)ast->data]
            );
            break;
        case AST_TYPE_CALL:     ast_call_print(ast->data, i + 2); break;
        case AST_TYPE_EXC:      ast_exc_print(ast->data, i + 2); break;
        case AST_TYPE_REF:      ast_ref_print(ast->data, i + 2); break;
        case AST_TYPE_DEF:      ast_def_print(ast->data, i + 2); break;
        case AST_TYPE_BLOCK:    ast_block_print(ast->data, i + 2); break;
        case AST_TYPE_ARG:      ast_arg_print(ast->data, i + 2); break;
        case AST_TYPE_LAMBDA:   ast_lambda_print(ast->data, i + 2); break;
        case AST_TYPE_BIF:      ast_bif_print(ast->data, i + 2); break;
        case AST_TYPE_FORCE:    ast_force_print(ast->data, i + 2); break;
        case AST_TYPE_PRESERVE: ast_preserve_print(ast->data, i + 2); break;
        default:                exit(1);
    }
    INDENT_FIELD_NONL_END;
    INDENT_END;
}

void ast_num_print(ASTNumData* data, int i) {
    INDENT_BEGIN(i);

    INDENT_FIELD("data", "%lf", *data);

    INDENT_END;
}

void ast_bool_print(ASTBoolData* data, int i) {
    INDENT_BEGIN(i);

    INDENT_FIELD("data", "%s", *data ? "true" : "false");

    INDENT_END;
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

void ast_call_print(ASTCallData* data, int i) {
    INDENT_BEGIN(i);

    INDENT_TITLE("ASTCallData", data);
    INDENT_FIELD("argc", "%ld", data->argc);
    INDENT_FIELD_LIST("argv", data->argv, data->argc, ast_print_i);
    INDENT_FIELD_EXT_NONL_START("exp");
    ast_print_i(data->exp, i + 2);
    INDENT_FIELD_NONL_END;

    INDENT_END;
}
void ast_def_print(ASTDefData* def, int depth) {
    INDENT_BEGIN(depth);

    INDENT_TITLE("ASTDefData", def);
    INDENT_FIELD("name", "%s", def->name);
    if (def->kind) {
        INDENT_FIELD_EXT_NONL_START("kind");
        ast_print_i(def->kind, depth + 2);
        INDENT_FIELD_NONL_END;
    } else INDENT_FIELD("kind", "%s", "Any");

    INDENT_FIELD_EXT_NONL_START("exp");
    ast_print_i(def->exp, depth + 2); // 2 because already indented.
    INDENT_FIELD_NONL_END;

    INDENT_END;
}

void ast_ref_print(ASTRefData* data, int i) {
    INDENT_BEGIN(i);

    INDENT_TITLE("ASTRefData", data);
    INDENT_FIELD("to", "%s", data->to);

    INDENT_END;
}

void ast_block_print(ASTBlockData* data, int depth) {
    INDENT_BEGIN(depth);

    INDENT_TITLE("ASTBlockData", data);
    INDENT_FIELD("ln", "%ld", data->ln);
    INDENT_FIELD_LIST("inside", data->inside, data->ln, ast_print_i);

    INDENT_END;
}
void ast_arg_print(ASTArgData* arg, int i) {
    INDENT_BEGIN(i);
    INDENT_TITLE("ASTArgData", arg);
    INDENT_FIELD("name", "%s", arg->name);
    INDENT_END;
}

void ast_lambda_print(ASTLambdaData* lambda, int i) {
    INDENT_BEGIN(i)
    INDENT_TITLE("ASTLambdaData", lambda);
    INDENT_FIELD("parc", "%ld", lambda->parc);
    INDENT_FIELD_LIST("parv", lambda->parv, lambda->parc, ast_print_i);
    INDENT_FIELD_EXT_NONL_START("body");
    ast_print_i(lambda->body, i + 2);
    INDENT_FIELD_NONL_END;
    INDENT_END;
}

void ast_bif_print(ASTBIFData* bif, int i) {
    INDENT_BEGIN(i);
    INDENT_TITLE("ASTBIFData", bif);

    char* name = "unknown";

    for (int i = 0; i < BUILTIN_FNS_LN; i++)
        if ((void*)BUILTIN_FNS[i].fn == bif) {
            name = BUILTIN_FNS[i].name;
            break;
        }

    INDENT_FIELD("name", "%s", name);
    INDENT_END;
}

void ast_force_print(ASTForceData* force, int i) {
    INDENT_BEGIN(i);

    INDENT_TITLE("ASTForceData", force);
    INDENT_FIELD_EXT_NONL_START("body");
    ast_print_i(force->body, i + 2);
    INDENT_FIELD_NONL_END;

    INDENT_END;
}

void ast_preserve_print(ASTPreserveData* preserve, int i) {
    INDENT_BEGIN(i);

    INDENT_TITLE("ASTPreserveData", preserve);
    INDENT_FIELD_EXT_NONL_START("body");
    ast_print_i(preserve->body, i + 2);
    INDENT_FIELD_NONL_END;

    INDENT_END;
}
