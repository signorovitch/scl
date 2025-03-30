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

struct builtin_data {
    char* name;
    AST* (*fn)(size_t argc, AST** argv);
};

static struct builtin_data BUILTIN_FNS[] = {
    { "sum", builtin_sum },
    { "sub", builtin_sub },
    { "mul", builtin_mul },
    { "div", builtin_div },
};
#define BUILTIN_FNS_LN (arrln(BUILTIN_FNS))

#endif
