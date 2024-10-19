#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "include/lexer.h"
#include "include/dstr.h"
#include "include/util.h"

Lexer* lexer_init(char* src) {
    Lexer* lexer = malloc(sizeof(Lexer));

    lexer->src = src;
    lexer->srcln = strlen(src);
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

    if (isdigit(*lexer->cchar)) {
        lexer->state = LEXER_STATE_NUM;
        lexer_do_number(lexer);
    } else {
        lexer->state = LEXER_STATE_CALL;
        lexer_do_call(lexer);
    }
}

void lexer_do_number(Lexer* lexer) {
    log_dbgf("lexer @ %p entered number mode @ char '%c' (%d)", lexer, *lexer->cchar, (int)*lexer->cchar);

    // Length of the number string.
    size_t numln;

    // Where the number string starts.
    char* start = lexer->cchar;

    for (numln = 0; *lexer->cchar && isdigit(*lexer->cchar); numln++)
        lexer_inc(lexer);

    char* num = malloc(numln + 1);
    memcpy(num, start, numln);
    num[numln] = '\0';

    lexer_add_token(lexer, token_init(TOKEN_TYPE_NUMBER, num, numln));
    lexer->state = LEXER_STATE_CONFUSED;
}

void lexer_do_call(Lexer* lexer) {
    log_dbgf("lexer @ %p entered call mode @ char '%c' (%d)", lexer, *lexer->cchar, (int)*lexer->cchar);

    // Size of the call string.
    size_t callln;

    // Where the call string starts.
    char* start = lexer->cchar;

    for (callln = 0; *lexer->cchar && (!isdigit(*lexer->cchar)); callln++)
        lexer_inc(lexer);

    char* call = malloc(callln + 1);
    memcpy(call, start, callln);
    call[callln] = '\0';

    lexer_add_token(lexer, token_init(TOKEN_TYPE_CALL, call, callln));

    lexer->state = LEXER_STATE_CONFUSED;
}

void lexer_inc(Lexer* lexer) {
    lexer->cchar += sizeof(char);
}

void lexer_add_token(Lexer* lexer, Token* token) {
    assert(lexer->ntokens < TOKENS_MAX);

    if (lexer->ntokens < TOKENS_MAX - 1) {
        lexer->tokens[lexer->ntokens] = token;
        lexer->ntokens++;

        log_dbgf("added token (total: %ld)", lexer->ntokens);
    }
}

void lexer_print(Lexer* lexer) { lexer_print_i(lexer, 0); }

void lexer_print_i(Lexer* lexer, int ilvl) {
    Dstr* spacing = dstr_init();
    char* sp = spacing->buf;
    for (int i = 0; i < ilvl; i++) dstr_appendch(spacing, ' ');

    printf("%sLexer @ %p\n", sp, lexer);
    printf("%s state:\n", sp);
    lexerstate_print_i(lexer->state, ilvl + 2);
    printf("%s srcln:\n", sp);
    printf("%s %ld\n", sp, lexer->srcln);
    printf("%s src:\n", sp);
    printf("%s  \"%s\"\n", sp, lexer->src);
    printf("%s cchar: \'%c\'\n", sp, *lexer->cchar);
    printf("%s ntokens: %ld\n", sp, lexer->ntokens);
    printf("%s tokens: [\n", sp);

    for (int i = 0; i < lexer->ntokens; i++) {
        token_print_i(lexer->tokens[i], ilvl + 2);
        printf(",\n\n");
    }
}

void lexerstate_print(LexerState s) { lexerstate_print_i(s, 0); }

void lexerstate_print_i(LexerState s, int ilvl) {
    Dstr* spacing = dstr_init();

    for (int j = 0; j < ilvl; j++) dstr_appendch(spacing, ' ');

    if (s > LEXER_STATE_MAX) {
        printf("%sUnknown (%d)\n", spacing->buf, s);
        log_dbgf("%d is not a valid LexerSate (max: %d)", s, LEXER_STATE_MAX);
        return;
    }

    printf("%s%s\n", spacing->buf, lexerstate_names[s]);

    dstr_destroy(spacing);
}
