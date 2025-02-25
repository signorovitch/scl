#include "include/builtin.h"
#include "include/ast.h"
#include "include/util.h"
#include <stdarg.h>
#include <stdio.h>

AST* builtin_sum(size_t argc, AST** argv) {
    log_dbg("Got here");
    ASTNumData total = 0;

    for (int i = 0; i < argc; i++) {
        AST* arg = argv[i];
        if (arg->type != AST_TYPE_NUM)
            return ast_init(
                AST_TYPE_EXC,
                ast_exc_data_init("Sum can't sum some non-num arguments.")
            );

        total += *(ASTNumData*)arg->data;
    }

    return ast_init(AST_TYPE_NUM, ast_num_data_init(total));
}

AST* builtin_sub(size_t argc, AST** argv) {
    log_dbg("Got here");
    AST* first = *argv;
    if (first->type != AST_TYPE_NUM)
        return ast_init(
            AST_TYPE_EXC, ast_exc_data_init("Can't subtract non-num arguments.")
        );

    ASTNumData total = *(ASTNumData*)first->data;

    for (int i = 1; i < argc; i++) {
        AST* arg = argv[i];
        if (arg->type != AST_TYPE_NUM)
            return ast_init(
                AST_TYPE_EXC,
                ast_exc_data_init("Can't subtract non-num arguments.")
            );

        total -= *(ASTNumData*)arg->data;
    }

    return ast_init(AST_TYPE_NUM, ast_num_data_init(total));
}
