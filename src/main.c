#include <stdio.h>

#include "include/ast.h"
#include "include/dstr.h"
#include "include/exec.h"
#include "include/lexer.h"
#include "include/util.h"

#include "../build/grammars/grammar.tab.h"

// Global Abstract Syntax Tree.
extern AST* root;

// Global input text.
char* inp = NULL;

extern int yyparse();

int main(int argc, char** argv) {

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
                default:   dstr_appendch(ln, c); log_dbgf("cchar: %c", c);
            }
        } while (1);

    lnend:

        log_dbgf("cline: %s", ln->buf);

        if (ln->ln > 0) {
            inp = ln->buf;
            if (yyparse() == 0) printf("Parsed successfully!\n");
            else printf("Parse error.\n");

            exec_print(exec_expr(root));
            ast_print(root);
        }

        dstr_destroy(ln);
    }
lnskip:;
}
