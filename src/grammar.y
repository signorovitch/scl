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
    int intval;
    char* strval;
    AST* ast;
}

%define parse.error verbose

%token<intval> NUM
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
    NUM { $$ = ast_init(AST_TYPE_NUM, ast_type_num_init($1)); }
    | NUM PLUS NUM {
        AST* argv[2] = {
            ast_init(AST_TYPE_NUM, ast_type_num_init($1)),
            ast_init(AST_TYPE_NUM, ast_type_num_init($3))
        };
        $$ = ast_init(AST_TYPE_CALL, ast_type_call_init("+", 2, argv));
    };

%%
