#ifndef BUILTIN_H
#define BUILTIN_H

#include "ast.h"

// Sum some nums.
AST* builtin_sum(size_t argc, AST** argv);

// Subtract nums.
AST* builtin_sub(size_t argc, AST** argv);

// The list of built-in functions.
static AST* (*builtin_fns[])(size_t argc, AST** argv) = {
    builtin_sum, builtin_sub
};

#endif
