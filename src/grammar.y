%{
    #include <string.h>
    #include <stdio.h>
    #include "../../src/include/ast.h"
    #include "../../src/include/lexer.h"
    #include "../../src/include/dlist.h"
    #include "../../src/include/builtin.h"

    int yylex(void);
    void yyerror(char const*);

    AST* root = NULL;
%}

%code requires {
    #include "../../src/include/ast.h"
    #include "../../src/include/dlist.h"
    #include "../../src/include/builtin.h"
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

%token BACKSLASH

%left ADD SUB
%left MUL DIV
%precedence NEG

%type<ast> exp;
%type<argarr> arg;
%type<argarr> argstart;
%type<exps> blockstart;
%type<exps> block;
%type<exps> inputstart
%type<exps> input

%start inputend // This makes no sense but w/e.

%%

inputstart:
    exp {
        DList* exps = dlist_init();
        dlist_append(exps, $1);
        $$ = exps;
    }
    ;

input:
    inputstart {
        $$ = $1;
    }
    | input EXPSEP exp {
        dlist_append($1, $3);
        $$ = $1;
    }
    ;

inputend:
    %empty
    | input {
        root = ast_init(AST_TYPE_BLOCK, ast_block_data_init((AST**) $1->buf, $1->ln));
        dlist_destroypsv($1);
    }
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
    // Number.
    NUM { $$ = ast_init(AST_TYPE_NUM, ast_num_data_init($1)); }

    // Variable reference.
    | WORD {
        $$ = ast_init(AST_TYPE_VREF, ast_vref_data_init($1));
    }

    // Call (general form).
    | exp GROUPS arg GROUPE {
        size_t argc = $3->ln;
        AST** argv = $3->buf;
        argarr_destroypsv($3);
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(
            argc,
            argv,
            $1
        ));
    }

    // Call (convenient form).
    | WORD GROUPS arg GROUPE {
        size_t argc = $3->ln;
        AST** argv = $3->buf;
        argarr_destroypsv($3);
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(
            argc,
            argv,
            ast_init(AST_TYPE_VREF, ast_vref_data_init($1))
        ));
    }

    // Function definitions. Convert to VDef of Lambda.
    | WORD GROUPS arg GROUPE exp {
        size_t parc = $3->ln;
        AST** parv = $3->buf;
        argarr_destroypsv($3);
        $$ = ast_init(AST_TYPE_VDEF, ast_vdef_data_init(
            $1,
            ast_init(AST_TYPE_LAMBDA, ast_lambda_data_init(
                parc, parv, $5
            ))
        ));
    }

    // Lambda definitions.
    | BACKSLASH GROUPS arg GROUPE exp {
        size_t parc = $3->ln;
        AST** parv = $3->buf;
        argarr_destroypsv($3);
        $$ = ast_init(AST_TYPE_LAMBDA, ast_lambda_data_init(parc, parv, $5));
    }

    // Block.
    | BLOCKS block BLOCKE {
        $$ = ast_init(AST_TYPE_BLOCK, ast_block_data_init((AST**) $2->buf, $2->ln));
    }

    // Negative.
    | SUB exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init(-1));
        argv[1] = $2;
        $$ = ast_init(AST_TYPE_CALL,
            ast_call_data_init(
                2,
                argv,
                ast_init(AST_TYPE_BIF,
                    ast_bif_data_init(builtin_mul)
                )
            )
        );
    }

    // Group.
    | GROUPS exp GROUPE { $$ = $2; }

    // Variable definition.
    | WORD EQ exp {
        $$ = ast_init(AST_TYPE_VDEF, ast_vdef_data_init($1, $3));
    }

    | exp ADD exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        $$ = ast_init(AST_TYPE_CALL,
            ast_call_data_init(
                2,
                argv,
                ast_init(AST_TYPE_BIF, ast_bif_data_init(builtin_sum))
            )
        );
    }

    | exp SUB exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        $$ = ast_init(AST_TYPE_CALL,
            ast_call_data_init(
                2,
                argv,
                ast_init(AST_TYPE_BIF, ast_bif_data_init(builtin_sub))
            )
        );
    }

    | exp MUL exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        $$ = ast_init(AST_TYPE_CALL,
            ast_call_data_init(
                2,
                argv,
                ast_init(AST_TYPE_BIF, ast_bif_data_init(builtin_mul))
            )
        );
    }

    | exp DIV exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;
        $$ = ast_init(AST_TYPE_CALL,
            ast_call_data_init(
                2,
                argv,
                ast_init(AST_TYPE_BIF, ast_bif_data_init(builtin_div))
            )
        );
    }
%%
