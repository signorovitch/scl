#include <stdarg.h>
#include <stdio.h>

#include "include/ast.h"
#include "include/builtin.h"
#include "include/exec.h"

AST* builtin_sum(size_t argc, AST** argv, Scope* parent) {
    ASTNumData total = 0;

    for (int i = 0; i < argc; i++) {
        AST* arg = exec_exp(argv[i], parent);
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

AST* builtin_sub(size_t argc, AST** argv, Scope* parent) {
    if (argc <= 0) return ast_init(AST_TYPE_NUM, ast_num_data_init(0));

    AST* first = exec_exp(*argv, parent);
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
        AST* arg = exec_exp(argv[i], parent);
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

AST* builtin_mul(size_t argc, AST** argv, Scope* parent) {
    if (argc <= 0) return ast_init(AST_TYPE_NUM, ast_num_data_init(0));

    AST* first = exec_exp(*argv, parent);
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
        AST* arg = exec_exp(argv[i], parent);
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

AST* builtin_div(size_t argc, AST** argv, Scope* parent) {
    if (argc <= 0) return ast_init(AST_TYPE_NUM, ast_num_data_init(0));

    AST* first = exec_exp(*argv, parent);
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
        AST* arg = exec_exp(argv[i], parent);
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

AST* builtin_die(size_t argc, AST** argv, Scope* parent) {
    return ast_init(AST_TYPE_EXC, ast_exc_data_init("8", NULL));
}

AST* builtin_if(size_t argc, AST** argv, Scope* parent) {
    if (argc != 3)
        return ast_init(
            AST_TYPE_EXC,
            ast_exc_data_init("If invoked with too few args.", NULL)
        );

    AST* pred = exec_exp(argv[0], parent);
    AST* body = argv[1];
    AST* alt = argv[2];

    if (pred->type != AST_TYPE_BOOL) {
        if (pred->type == AST_TYPE_EXC) {
            return ast_init(
                AST_TYPE_EXC, ast_exc_data_init("if touched an error", pred)
            );
        } else {
            return ast_init(
                AST_TYPE_EXC,
                ast_exc_data_init("if works on booleans idiot", NULL)
            );
        }
    }

    if (*(ASTBoolData*)pred->data) return exec_exp(body, parent);
    else return exec_exp(alt, parent);
}

AST* builtin_eq(size_t argc, AST** argv, Scope* parent) {
    if (argc < 1) return ast_init(AST_TYPE_EXC, ast_exc_data_init("bad", NULL));
    else if (argc == 1) return ast_init(AST_TYPE_BOOL, ast_bool_data_init(1));

    AST* first = exec_exp(argv[0], parent);
    ASTType type = first->type;

    AST* second = exec_exp(argv[1], parent);
    if (first->type == AST_TYPE_EXC)
        return ast_init(
            AST_TYPE_EXC, ast_exc_data_init("first was bad", first)
        );
    if (second->type == AST_TYPE_EXC)
        return ast_init(
            AST_TYPE_EXC, ast_exc_data_init("second was bad", second)
        );

    if (second->type != type)
        return ast_init(
            AST_TYPE_EXC,
            ast_exc_data_init("apples and oranges or something idk", NULL)
        );

    // Later when I put together an anctual type system I'll have this
    // delegated to each type. For now this works.

    switch (type) {
        case AST_TYPE_NUM:
            if (*(ASTNumData*)first->data == *(ASTNumData*)second->data)
                return ast_init(AST_TYPE_BOOL, ast_bool_data_init(1));
            else return ast_init(AST_TYPE_BOOL, ast_bool_data_init(0));
        case AST_TYPE_BOOL:
            if (*(ASTNumData*)first->data == *(ASTNumData*)second->data)
                return ast_init(AST_TYPE_BOOL, ast_bool_data_init(1));
            else return ast_init(AST_TYPE_BOOL, ast_bool_data_init(0));
        default:
            return ast_init(
                AST_TYPE_BOOL, ast_bool_data_init(0)
            ); // Can't equate nonprimatives. I think. Maybe.
    }
}
