#ifndef BUILTIN_H
#define BUILTIN_H

#include "ast.h"

// Sum some nums.
AST* builtin_sum(size_t argc, AST** argv);

// Subtract nums.
AST* builtin_sub(size_t argc, AST** argv);

// Multiply nums.
AST* builtin_mul(size_t argc, AST** argv);

// Divide nums.
AST* builtin_div(size_t argc, AST** argv);

#define NBUILTIN_FNS 4
// The list of built-in functions.
static AST* (*builtin_fns[NBUILTIN_FNS])(size_t argc, AST** argv) = {
    builtin_sum, builtin_sub, builtin_mul, builtin_div
};

static char* builtin_fns_names[NBUILTIN_FNS] = {
    "sum", "sub", "mul", "div"
};

#endif
