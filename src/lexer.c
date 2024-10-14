#include <ctype.h>
#include <string.h>

#include "include/lexer.h"
#include "include/util.h"

Lexer* lexer_init(char* src) {
    Lexer* lexer = malloc(sizeof(Lexer));

    lexer->src = src;
    lexer->srcl = strlen(src);
    lexer->cchar = lexer->src;

    lexer->tokens = calloc(TOKENS_MAX, sizeof(Token*));
    lexer->ntokens = 0;
    lexer->state = LEXER_STATE_CONFUSED;

    log_dbgf("created new lexer @ %p", lexer);

    return lexer;
}

void lexer_destroy(Lexer* lexer) {
    free(lexer->src);

    for (int i = 0; i < lexer->ntokens; i++) token_destroy(lexer->tokens[i]);
}

void lexer_lex(Lexer* lexer) {
    while (*lexer->cchar) {
        switch (lexer->state) {
        case LEXER_STATE_CONFUSED: lexer_do_confused(lexer); break;
        case LEXER_STATE_NUM:      lexer_do_number(lexer); break;
        case LEXER_STATE_CALL:     lexer_do_call(lexer); break;
        default:                   break;
        }
    }
}

void lexer_do_confused(Lexer* lexer) {
    log_dbgf("lexer @ %p entered confused mode @ char '%c' (%d)", lexer, *lexer->cchar, (int)*lexer->cchar);

    lexer->state = LEXER_STATE_CONFUSED;
    if (isdigit(*lexer->cchar)) lexer_do_number(lexer);
    else lexer_do_call(lexer);
}

void lexer_do_number(Lexer* lexer) {
    log_dbgf("lexer @ %p entered number mode @ char '%c' (%d)", lexer, *lexer->cchar, (int)*lexer->cchar);

    lexer->state = LEXER_STATE_NUM;
    // Size of the number string.
    size_t numsz;

    // Where the number string starts.
    char* start = lexer->cchar;

    for (numsz = 0; *lexer->cchar && isdigit(*lexer->cchar); numsz++)
        lexer_inc(lexer);

    char* num = malloc(numsz + 1);
    memcpy(num, start, numsz);
    num[numsz] = '\0';

    lexer_add_token(lexer, token_init(TOKEN_TYPE_NUMBER, num, 1));
}

void lexer_do_call(Lexer* lexer) {
    log_dbgf("lexer @ %p entered call mode @ char '%c' (%d)", lexer, *lexer->cchar, (int)*lexer->cchar);

    lexer->state = LEXER_STATE_CALL;
    // Size of the call string.
    size_t callsz;

    // Where the call string starts.
    char* start = lexer->cchar;

    for (; *lexer->cchar && (isblank(lexer->cchar) || *lexer->cchar == '\n'); lexer_inc(lexer));

    for (callsz = 0; *lexer->cchar && isalpha(*lexer->cchar); callsz++)
        lexer_inc(lexer);

    char* call = malloc(callsz + 1);
    memcpy(call, start, callsz);
    call[callsz] = '\0';

    lexer_add_token(lexer, token_init(TOKEN_TYPE_CALL, call, 1));
}

void lexer_inc(Lexer* lexer) {
    lexer->cchar += sizeof(char);
}

void lexer_add_token(Lexer* lexer, Token* token) {
    assert(lexer->ntokens < TOKENS_MAX);

    if (lexer->ntokens < TOKENS_MAX - 1) {
        lexer->tokens[lexer->ntokens] = token;
        lexer->ntokens++;
    }
}

