#include "include/lexer.h"

Lexer* lexer_init(char* src) {
    Lexer* lexer = malloc(sizeof(Lexer));

    lexer->src = src;
    lexer->srcl = strlen(src);
    lexer->cchar = lexer->src;
    
    lexer->tokens = NULL;
    lexer->ntokens = 0;
    lexer->state = LEXER_STATE_CONFUSED;

    return lexer;
}

void lexer_destroy(Lexer *lexer) {
    free(lexer->src);

    for (
        int i = 0;
        i < lexer->ntokens;
        token_destroy(lexer->tokens[i++])
    );
}

void lexer_do_confused(Lexer *lexer) {

}

void lexer_lex(Lexer* lexer) {
    while (*lexer->cchar) {
        switch (lexer->state) {
        case LEXER_STATE_CONFUSED:
            lexer_do_confused(lexer);
        break; case LEXER_STATE_NUM:
            lexer_do_number(lexer);
        break; case LEXER_STATE_CALL:
            lexer_do_call(lexer);
        break;
        default: break;
        }
    }
}
