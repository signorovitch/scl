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

%token BOOLT // Boolean true (TRUE or T).
%token BOOLF // Boolean false (FALSE or F).

%token IF // if or ?.
%token ELSE // else or :.

%token BLOCKS // Block start {.
%token BLOCKE // Block end }.

%token GROUPS // Group start (.
%token GROUPE // Group end ).
%token SEP // Seperator ,.

%token EQ // Equals =.
%token DEQ // Double equals ==.

%token RARROW // Right arrow ->.
%token LARROW // Left arrow <-.

%token EXPSEP // Expression seperator ;.

%token<strval> WORD // Word, i.e. keyword.
%token<fval> NUM // Number.

%token SUB // Subtract -.
%token ADD // Addition *.
%token MUL // Multiplication *.
%token DIV // Division /.

%token NL // Newline.

%token COLON // Colon :.
%token STOP // Stop sign $.

%token BACKSLASH

%left ADD SUB
%left MUL DIV
%right RARROW
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
    // Variable reference.
    WORD {
        $$ = ast_init(AST_TYPE_REF, ast_ref_data_init($1));
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

    // Call (right arrow general form).
    | GROUPS arg GROUPE RARROW exp {
        size_t argc = $2->ln;
        AST** argv = $2->buf;
        argarr_destroypsv($2);
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(
            argc,
            argv,
            $5
        ));
    }

    /* TODO: Call (right arrow single arg).
    | exp RARROW exp {
        size_t argc = 1;
        AST** argv = ;
        argarr_destroypsv($2);
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(
            argc,
            argv,
            $5
        ));
    }*/

    // Call (convenient form).
    | WORD GROUPS arg GROUPE {
        size_t argc = $3->ln;
        AST** argv = $3->buf;
        argarr_destroypsv($3);
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(
            argc,
            argv,
            ast_init(AST_TYPE_REF, ast_ref_data_init($1))
        ));
    }


    // Call (hacky convenient form).
    | WORD GROUPS GROUPE {
        size_t argc = 0;
        AST** argv = NULL;
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(
            argc,
            argv,
            ast_init(AST_TYPE_REF, ast_ref_data_init($1))
        ));
    }

    // Call (right arrow hacky convenient form).
    | GROUPS GROUPE RARROW WORD {
        size_t argc = 0;
        AST** argv = NULL;
        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(
            argc,
            argv,
            ast_init(AST_TYPE_REF, ast_ref_data_init($4))
        ));
    }

    // Number.
    | NUM { $$ = ast_init(AST_TYPE_LIT_NUM, ast_num_data_init($1)); }

    | BOOLT { $$ = ast_init(AST_TYPE_LIT_BOOL, ast_bool_data_init(1)); }
    | BOOLF { $$ = ast_init(AST_TYPE_LIT_BOOL, ast_bool_data_init(0)); }

    | exp DEQ exp {
        AST** argv = calloc(2, sizeof(AST*));
        argv[0] = $1;
        argv[1] = $3;

        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(
            2,
            argv,
            ast_init(AST_TYPE_BIF, ast_bif_data_init(builtin_eq))
        ));
    }

    | IF exp exp exp {
        AST** argv = calloc(3, sizeof(AST*));
        argv[0] = $2;
        argv[1] = $3;
        argv[2] = $4;

        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(
            3,
            argv,
            ast_init(AST_TYPE_BIF, ast_bif_data_init(builtin_if))
        ));
    }
    | IF exp exp ELSE exp {
        AST** argv = calloc(3, sizeof(AST*));
        argv[0] = $2;
        argv[1] = $3;
        argv[2] = $5;

        $$ = ast_init(AST_TYPE_CALL, ast_call_data_init(
            3,
            argv,
            ast_init(AST_TYPE_BIF, ast_bif_data_init(builtin_if))
        ));
    }


    // Function definitions. Convert to Def of Lambda.
    | WORD GROUPS arg GROUPE exp {
        size_t parc = $3->ln;
        AST** parv = $3->buf;
        argarr_destroypsv($3);
        $$ = ast_init(AST_TYPE_DEF, ast_def_data_init(
            $1,
            NULL,
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
        argv[0] = ast_init(AST_TYPE_LIT_NUM, ast_num_data_init(-1));
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

    // Stop sign.
    | exp STOP { $$ = $1; }

    // Variable definition.
    | WORD EQ exp {
        $$ = ast_init(AST_TYPE_DEF, ast_def_data_init($1, NULL, $3));
    }

    // Variable definition with type annotation.
    | WORD COLON exp EQ exp {
        $$ = ast_init(AST_TYPE_DEF, ast_def_data_init($1, $3, $5));
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
