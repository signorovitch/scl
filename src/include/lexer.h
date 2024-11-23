#ifndef LEXER_H
#define LEXER_H

#include <assert.h>

#ifdef __has_include
    #if __has_include("../../build/grammars/grammar.tab.h")
        #include "../../build/grammars/grammar.tab.h"
    #else
        #warn "Build resources not present!"
    #endif
#else
    #warn "Not sure whether build-time resources are present."
    #include "../../build/grammars/grammar.tab.h"
#endif

extern YYSTYPE yylval;
extern char* inp;

// Called by `yyparse()` (in bison-generated files.)
int yylex();
void yyerror(char const* s);

#endif
