#include <stdio.h>

#include "include/ast.h"
#include "include/dstr.h"
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
        Dstr* cline = dstr_init(); // The current line.
        printf("> ");
        fflush(stdout);
        for (char cch; (cch = getc(stdin)) != '\n';) {
            log_dbgf("cchar: %c", cch);
            dstr_appendch(cline, cch);
        }
        dstr_appendch(cline, '\n');

        log_dbgf("cline: %s", cline->buf);

        if (cline->ln > 0) {
            // I hope it's null-terminated.
            inp = cline->buf;
            if (yyparse() == 0) {
                printf("Parsed successfully!\n");
            } else {
                printf("Parse error.\n");
            }
        }

        dstr_destroy(cline);
    }
}
