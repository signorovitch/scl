#include <stdio.h>

#include "include/dstr.h"
#include "include/token.h"
#include "include/util.h"
#include "include/lexer.h"

int main(int argc, char** argv) {
    while(1) {
        Dstr* cline = dstr_init(); // The current line.
        printf("> ");
        fflush(stdout);
        for (char cch; (cch = getchar() != EOF);) {
            dstr_appendch(cline, fgetc(stdin));
        }

        if (cline->ln > 0) {
            Lexer* lexer = lexer_init(cline->buf);
            lexer_lex(lexer);
            printf("\n=%s\n", token_to_dstr(lexer->tokens[0])->buf);
        }

    }
}
