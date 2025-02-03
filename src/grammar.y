%{
    #include <string.h>
    #include <stdio.h>
    #include "../../src/include/ast.h"
    #include "../../src/include/lexer.h"
    #include "../../src/include/dlist.h"

    int yylex(void);
    void yyerror(char const*);

    AST* root = NULL;
%}

%code requires {
    #include "../../src/include/ast.h"
    #include "../../src/include/dlist.h"
}

%union {
    double fval;
    char* strval;
    AST* ast;
    ArgArr* argarr;
    DList* exps;
}

%define parse.error verbose

%token BLOCKS // Block start {.
%token BLOCKE // Block end }.

%token GROUPS // Group start (.
%token GROUPE // Group end ).
%token SEP // Seperator ,.

%token EQ // Equals =.

%token EXPSEP // Expression seperator ;.

%token<strval> WORD // Word, i.e. keyword.
%token<fval> NUM // Number.

%token SUB // Subtract -.
%token ADD // Addition *. 
%token MUL // Multiplication *.
%token DIV // Division /.

%token NL // Newline.

%left ADD SUB
%left MUL DIV
%precedence NEG

%type<ast> exp;
%type<argarr> arg;
%type<argarr> argstart;
%type<exps> blockstart;
%type<exps> block;

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

blockstart:
    exp {
        DList* exps = dlist_init(); // List of expressions.
        dlist_append(exps, $1);
        $$ = exps;
    }
    ;

block:
     blockstart { $$ = $1; }
     | block EXPSEP exp {
        dlist_append($1, $3);
        $$ = $1;
    }
    ;

exp:
    NUM { $$ = ast_init(AST_TYPE_NUM, ast_num_data_init($1)); }

    | BLOCKS exp BLOCKE { $$ = $2; }

    | BLOCKS block BLOCKE {
        size_t i = $2->ln - 1;
        $$ = $2->buf[i];
    }

    | SUB exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init(-1));
        argv[1] = $2;
        char* to = malloc(4);
        strcpy(to, "mul");
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(to, 2, argv));
    }

    | GROUPS exp GROUPE { $$ = $2; }

    // Variable definition.
    | WORD EQ exp {
        $$ = ast_init(AST_TYPE_VDEF, ast_vdef_data_init($1, $3));
    }

    // Variable reference.
    //| WORD {
    //    $$ = ast_init(AST_TYPE_VREF, ast_vref_data_init($1));
    //}

    | WORD GROUPS arg GROUPE {
        size_t argc = $3->ln;
        AST** argv = $3->buf;
        argarr_destroypsv($3);
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init($1, argc, argv));
    }

    | exp ADD exp {
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

    | exp MUL exp {
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
