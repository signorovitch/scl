%{
    #include <string.h>
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

%token EXPSEP

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
    | input EXPSEP exp { root = $3; }
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
        char* to = malloc(4);
        strcpy(to, "mul");
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(to, 2, argv));
    }

    | LGROUP exp RGROUP { $$ = $2; }

    // Variable reference.
    | WORD {
        $$ = ast_init(AST_TYPE_VREF, ast_vref_data_init($1));
    }

    | WORD LGROUP arg RGROUP {
        size_t argc = $3->ln;
        AST** argv = $3->buf;
        argarr_destroypsv($3);
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init($1, argc, argv));
    }

    | exp PLUS exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        char* to = malloc(4);
        strcpy(to, "sum");
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(to, 2, argv));
    }

    | exp SUB exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        char* to = malloc(4);
        strcpy(to, "sub");
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(to, 2, argv));
    }

    | exp MULT exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        char* to = malloc(4);
        strcpy(to, "mul");
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(to, 2, argv));
    }

    | exp DIV exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        char* to = malloc(4);
        strcpy(to, "div");
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(to, 2, argv));
    }
%%
