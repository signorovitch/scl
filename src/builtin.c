#include "include/builtin.h"
#include "include/ast.h"
#include "include/exec.h"
#include "include/util.h"
#include <stdarg.h>
#include <stdio.h>

AST* builtin_sum(size_t argc, AST** argv) {
    ASTNumData total = 0;

    for (int i = 0; i < argc; i++) {
        AST* arg = exec_exp(argv[i]);
        if (arg->type == AST_TYPE_EXC)
            return ast_init(
                AST_TYPE_EXC,
                ast_exc_data_init("`sum` encountered an exception.", arg)
            );
        if (arg->type != AST_TYPE_NUM)
            return ast_init(
                AST_TYPE_EXC,
                ast_exc_data_init("Sum can't sum some non-num arguments.", NULL)
            );

        total += *(ASTNumData*)arg->data;
    }

    return ast_init(AST_TYPE_NUM, ast_num_data_init(total));
}

AST* builtin_sub(size_t argc, AST** argv) {
    log_dbg("Got here");
    AST* first = exec_exp(*argv);
    if (first->type == AST_TYPE_EXC)
        return ast_init(
            AST_TYPE_EXC,
            ast_exc_data_init("`sub` encountered an exception.", first)
        );
    if (first->type != AST_TYPE_NUM)
        return ast_init(
            AST_TYPE_EXC,
            ast_exc_data_init("Can't subtract non-num arguments.", NULL)
        );

    ASTNumData total = *(ASTNumData*)first->data;

    for (int i = 1; i < argc; i++) {
        AST* arg = exec_exp(argv[i]);
        if (arg->type == AST_TYPE_EXC)
            return ast_init(
                AST_TYPE_EXC,
                ast_exc_data_init("`sub` encountered an exception.", arg)
            );
        if (arg->type != AST_TYPE_NUM)
            return ast_init(
                AST_TYPE_EXC,
                ast_exc_data_init("Can't subtract non-num arguments.", NULL)
            );

        total -= *(ASTNumData*)arg->data;
    }

    return ast_init(AST_TYPE_NUM, ast_num_data_init(total));
}

AST* builtin_mul(size_t argc, AST** argv) {
    log_dbg("Got here");
    AST* first = exec_exp(*argv);
    if (first->type == AST_TYPE_EXC)
        return ast_init(
            AST_TYPE_EXC,
            ast_exc_data_init("`mul` encountered an expection.", first)
        );
    if (first->type != AST_TYPE_NUM)
        return ast_init(
            AST_TYPE_EXC,
            ast_exc_data_init("Can't multiply non-num arguments.", NULL)
        );

    ASTNumData total = *(ASTNumData*)first->data;

    for (int i = 1; i < argc; i++) {
        AST* arg = exec_exp(argv[i]);
        if (arg->type == AST_TYPE_EXC)
            return ast_init(
                AST_TYPE_EXC,
                ast_exc_data_init("`mul` encountered an execption.", arg)
            );
        if (arg->type != AST_TYPE_NUM)
            return ast_init(
                AST_TYPE_EXC,
                ast_exc_data_init("Can't multiply non-num arguments.", NULL)
            );

        total *= *(ASTNumData*)arg->data;
    }

    return ast_init(AST_TYPE_NUM, ast_num_data_init(total));
}

AST* builtin_div(size_t argc, AST** argv) {
    log_dbg("Got here");
    AST* first = exec_exp(*argv);
    if (first->type == AST_TYPE_EXC)
        return ast_init(
            AST_TYPE_EXC,
            ast_exc_data_init("`div` encountered an exception.", first)
        );
    if (first->type != AST_TYPE_NUM)
        return ast_init(
            AST_TYPE_EXC,
            ast_exc_data_init("Can't divide non-num arguments.", NULL)
        );

    ASTNumData total = *(ASTNumData*)first->data;

    for (int i = 1; i < argc; i++) {
        AST* arg = exec_exp(argv[i]);
        if (arg->type == AST_TYPE_EXC)
            return ast_init(
                AST_TYPE_EXC,
                ast_exc_data_init("`div` encountered an exception.", arg)
            );
        if (arg->type != AST_TYPE_NUM)
            return ast_init(
                AST_TYPE_EXC,
                ast_exc_data_init("Can't divide non-num arguments.", NULL)
            );

        total /= *(ASTNumData*)arg->data;
    }

    return ast_init(AST_TYPE_NUM, ast_num_data_init(total));
}
