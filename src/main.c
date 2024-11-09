#include <stdio.h>

#include "include/ast.h"
#include "include/dstr.h"
#include "include/lexer.h"
#include "include/util.h"

#include "../build/grammars/grammar.tab.h"

// Global Abstract Syntax Tree.
AST* root = NULL;

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

        log_dbgf("cline: %s", cline->buf);

        if (cline->ln > 0) {
            lexer_init(cline->buf);
            lexer_lex();
            lexer_print();
            if (yyparse() == 0) {
                printf("Parsed successfully!\n");
            } else {
                printf("Parse error.\n");
            }
            lexer_destroy();
        }

        dstr_destroy(cline);
    }
}
