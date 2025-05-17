#include <stdio.h>
#include <string.h>

#include "include/ast.h"
#include "include/dstr.h"
#include "include/exec.h"

#include "include/lexer.h"
#include "include/util.h"

#include "../build/grammars/grammar.tab.h"

// Global Abstract Syntax Tree.
extern AST* root;
extern char* inp;
extern int yyparse();

int main(int argc, char** argv) {

    if (argc - 1 && strlen(argv[1]) > 0 && (inp = argv[1]) && !yyparse()) {
        log_dbg("Parsed successfully!\n");
        ast_print(root);
        AST* eval = exec_start(root);
        ast_print(eval);
        ast_destroy(eval);
        ast_destroy(root);
        exit(0);
    }

    while (1) {
        Dstr* ln = dstr_init();
        char c;

        printf("> ");
        fflush(stdout);

        // Accumulate line.
        do {
            c = getc(stdin);
            switch (c) {
                case EOF:  dstr_destroy(ln); goto lnskip;
                case '\n': goto lnend;
                default:   dstr_appendch(ln, c);
            }
        } while (1);

    lnend:

        log_dbgf("cline: %s", ln->buf);

        if (ln->ln > 0) {
            inp = ln->buf;
            if (yyparse() == 0) {
                log_dbg("Parsed successfully!\n");
            } else printf("Parse error.\n");

#ifdef DBG
            ast_print(root);
#endif

            AST* eval = exec_start(root);
            ast_print(eval);
            ast_destroy(eval);
            ast_destroy(root);
        }

        dstr_destroy(ln);
    }
lnskip:;

    return 0;
}
