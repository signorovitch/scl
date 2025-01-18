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
    ArgArr* argarr;
}

%define parse.error verbose

%token LGROUP
%token RGROUP
%token SEP

%token<strval> WORD
%token<fval> NUM

%token SUB
%token PLUS
%token MULT
%token DIV

%token NL

%left PLUS SUB
%left MULT DIV
%precedence NEG

%type<ast> exp;
%type<argarr> arg;
%type<argarr> argstart;

%%

input:
    %empty
    | exp { root = $1; }
    ;


argstart:
    exp {
        ArgArr* argarr = argarr_init();
        argarr_add(argarr, $1);
        $$ = argarr;
    }
    ;

arg:
    argstart { $$ = $1; }
    | arg SEP exp {
        argarr_add($1, $3);
        $$ = $1;
    }
    ;

exp:
    NUM { $$ = ast_init(AST_TYPE_NUM, ast_num_data_init($1)); }

    | SUB exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init(-1));
        argv[1] = $2;
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("mul", 2, argv));
    }

    | LGROUP exp RGROUP { $$ = $2; }

    // Variable reference.
    //| WORD

    | WORD LGROUP arg RGROUP {
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init($1, $3->ln, $3->buf));
    }

    | exp PLUS exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("sum", 2, argv));
    }

    | exp SUB exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("sub", 2, argv));
    }

    | exp MULT exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("mul", 2, argv));
    }

    | exp DIV exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init("div", 2, argv));
    }
%%
