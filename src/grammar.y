%code requires {
    #include "../../src/include/ast.h"
}

%{
#include "../../src/include/ast.h"
%}

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
   NUM {}
   | exp PLUS exp { $$ = ast_type_call_init("+", 2, [ast_type_num_init($1), ast_type_num_init(int val)]}
   ;

%%
