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

%token<strval> CALL
%token<fval> NUM

%token NEG
%token PLUS
%token MULT
%token DIV

%token NL
%type<ast> exp

%%

input:
    %empty
    | exp { root = $1; }
    ;

exp:
    NUM { $$ = ast_init(AST_TYPE_NUM, ast_num_data_init($1)); }
    | NEG NUM { $$ = ast_init(AST_TYPE_NUM, ast_num_data_init(-$2)); }
    | NUM PLUS NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init($1));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init($3));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("sum", 2, argv));
    }
    | NEG NUM PLUS NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init(-$2));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init($4));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("sum", 2, argv));
    }
    | NUM NEG NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init($1));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init($3));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("sub", 2, argv));
    }
    | NEG NUM NEG NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init(-$2));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init($4));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("sub", 2, argv));
    }

    | NUM MULT NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init($1));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init($3));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("mul", 2, argv));
    }
    | NEG NUM MULT NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init(-$2));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init($4));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("mul", 2, argv));
    }
    | NEG NUM MULT NEG NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init(-$2));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init(-$5));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("mul", 2, argv));
    }
    | NUM MULT NEG NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init($1));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init(-$4));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("mul", 2, argv));
    }

    | NUM DIV NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init($1));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init($3));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("div", 2, argv));
    }
    | NEG NUM DIV NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init(-$2));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init($4));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("div", 2, argv));
    }
    | NEG NUM DIV NEG NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init(-$2));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init(-$5));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("div", 2, argv));
    }
    | NUM DIV NEG NUM {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init($1));
        argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init(-$4));
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("div", 2, argv));
    }
%%
