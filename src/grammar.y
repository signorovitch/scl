%{
    #include <stdio.h>
    #include "../../src/include/ast.h"
    #include "../../src/include/lexer.h"

    int yylex(void);
    void yyerror(char const*);

    AST* root = NULL;
%}

%code requires {
    #include "../../src/include/ast.h"
}

%union {
    double fval;
    char* strval;
    AST* ast;
}

%define parse.error verbose

%token<fval> NUM
%token<strval> CALL
%token PLUS
%token NL
%type<ast> exp

%%

input:
    %empty
    | exp { root = $1; }
    ;

exp:
    NUM { $$ = ast_init(AST_TYPE_NUM, ast_num_data_init($1)); }
    | NUM PLUS exp {
        AST* argv[2] = {
            ast_init(AST_TYPE_NUM, ast_num_data_init($1)),
            $3
        };
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("+", 2, argv));
    }/*
    | exp PLUS exp {
        AST* argv[2] = { $1, $3 };
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("+", 2, argv));
    };*/

%%
