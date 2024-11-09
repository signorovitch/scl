%{
#include "../../src/include/ast.h"
#include "../../src/include/lexer.h"
int yylex (void);
void yyerror (char const *);
%}

%code requires {
    #include "../../src/include/ast.h"
}

%union {
    int intval;
    char* strval;
    AST* ast;
}

%token <intval> NUM
%token <strval> CALL
%token PLUS
%type <ast> exp

%%

exp:
   NUM { $$ = ast_type_num_init($1); }
   | NUM PLUS NUM { AST* argv[2] = {ast_type_num_init($1), ast_type_num_init($1)}; $$ = ast_type_call_init("+", 2, argv);}
   ;

%%
