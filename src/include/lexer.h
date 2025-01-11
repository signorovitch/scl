#ifndef LEXER_H
#define LEXER_H

#include <assert.h>
#include <stddef.h>

#include "ast.h"

#define ARLN 8

extern char* inp;

typedef struct {
    size_t sz;
    size_t ln;
    AST** buf;
} ArgArr;

ArgArr* argarr_init();
void argarr_destroy(ArgArr* argarr);
void argarr_add(ArgArr* argarr, AST* arg);

#include "../../build/grammars/grammar.tab.h"

extern YYSTYPE yylval;

// Accumulate an integer.
int acc_int(int c);

// Accumulate a floating-point number.
double acc_float(int c);

// Called by `yyparse()` (in bison-generated files.)
int yylex();
void yyerror(char const* s);

#endif
