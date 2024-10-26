#include <stdio.h>

#include "include/dstr.h"
#include "include/util.h"
#include "include/lexer.h"

int main(int argc, char** argv) {
    while(1) {
        Dstr* cline = dstr_init(); // The current line.
        printf("> ");
        fflush(stdout);
        for (char cch; (cch = getc(stdin)) != '\n';) {
            log_dbgf("cchar: %c", cch);
            dstr_appendch(cline, cch);
        }

        log_dbgf("cline: %s", cline->buf);

        if (cline->ln > 0) {
            Lexer* lexer = lexer_init(cline->buf);
            lexer_lex(lexer);
            lexer_print(lexer);
        }
    }
}
