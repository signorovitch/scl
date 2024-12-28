#ifndef LEXER_H
#define LEXER_H

#include <assert.h>

#include "../../build/grammars/grammar.tab.h"

extern YYSTYPE yylval;
extern char* inp;

// Accumulate an integer.
int acc_int(int c);

// Accumulate a floating-point number.
double acc_float(int c);

// Called by `yyparse()` (in bison-generated files.)
int yylex();
void yyerror(char const* s);

#endif
