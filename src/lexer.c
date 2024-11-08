#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "include/lexer.h"
#include "include/dstr.h"
#include "include/token.h"
#include "include/util.h"

Lexer* thelexer = NULL;

void lexer_init(char* src) {
    thelexer = malloc(sizeof(Lexer));

    thelexer->src = src;
    thelexer->srcln = strlen(src);
    thelexer->cchar = thelexer->src;

    thelexer->tokens = calloc(TOKENS_MAX, sizeof(Token*));
    thelexer->ntokens = 0;
    thelexer->state = LEXER_STATE_CONFUSED;

    log_dbgf("created thelexer @ %p", thelexer);
}

void lexer_destroy() {
    // Does not free lexer->src.
    for (int i = 0; i < thelexer->ntokens; i++) token_destroy(thelexer->tokens[i]);
}

void lexer_lex() {
    while (*thelexer->cchar) {
        switch (thelexer->state) {
        case LEXER_STATE_CONFUSED: lexer_do_confused(); break;
        case LEXER_STATE_NUM:      lexer_do_number(); break;
        case LEXER_STATE_CALL:     lexer_do_call(); break;
        default:                   break;
        }
    }
}

void lexer_do_confused() {
    log_dbgf("lexer @ %p entered confused mode @ char '%c' (%d)", thelexer,
             *thelexer->cchar, (int)*thelexer->cchar);

    if (isspace(*thelexer->cchar)) lexer_inc();

    if (isdigit(*thelexer->cchar)) {
        thelexer->state = LEXER_STATE_NUM;
        lexer_do_number();
    } else {
        thelexer->state = LEXER_STATE_CALL;
        lexer_do_call();
    }
}

void lexer_do_number() {
    log_dbgf("lexer @ %p entered number mode @ char '%c' (%d)", thelexer,
             *thelexer->cchar, (int)*thelexer->cchar);

    // Length of the number string.
    size_t numln;

    // Where the number string starts.
    char* start = thelexer->cchar;

    for (numln = 0; *thelexer->cchar && isdigit(*thelexer->cchar); numln++)
        lexer_inc();

    char* num = malloc(numln + 1);
    memcpy(num, start, numln);
    num[numln] = '\0';

    lexer_add_token(token_init(TOKEN_TYPE_NUMBER, num, numln));
    thelexer->state = LEXER_STATE_CONFUSED;
}

void lexer_do_call() {
    log_dbgf("lexer @ %p entered call mode @ char '%c' (%d)", thelexer,
             *thelexer->cchar, (int)*thelexer->cchar);

    // Size of the call string.
    size_t callln;

    // Where the call string starts.
    char* start = thelexer->cchar;

    for (callln = 0;
         *thelexer->cchar && (!isdigit(*thelexer->cchar) && !isspace(*thelexer->cchar));
         callln++)
        lexer_inc();

    char* call = malloc(callln + 1);
    memcpy(call, start, callln);
    call[callln] = '\0';

    lexer_add_token(token_init(TOKEN_TYPE_CALL, call, callln));

    thelexer->state = LEXER_STATE_CONFUSED;
}

void lexer_inc() {
    thelexer->cchar += sizeof(char);
}

void lexer_add_token(Token* token) {
    assert(thelexer->ntokens < TOKENS_MAX);

    if (thelexer->ntokens < TOKENS_MAX - 1) {
        thelexer->tokens[thelexer->ntokens] = token;
        thelexer->ntokens++;

        log_dbgf("added token (total: %ld)", thelexer->ntokens);
    }
}

void lexer_print() { lexer_print_i(0); }

void lexer_print_i(int ilvl) {
    INDENT_BEGIN(ilvl);
    INDENT_TITLE("Lexer", thelexer);
    INDENT_FIELD_NONL_START("state")
        lexerstate_print_raw();
    INDENT_FIELD_NONL_END
    INDENT_FIELD("srcln", "%ld", thelexer->srcln);
    INDENT_FIELD_NL("src", "\"%s\"", thelexer->src);
    INDENT_FIELD("cchar", "'%c'", *thelexer->cchar);
    INDENT_FIELD("ntokens", "%ld", thelexer->ntokens);
    INDENT_FIELD_LIST("tokens", thelexer->tokens, thelexer->ntokens, token_print_i);
}

void lexerstate_print_raw() {
    LexerState s = thelexer->state;
    if (s > LEXER_STATE_MAX) {
        printf("Unknown (%d)", s);
        log_dbgf("%d is not a valid LexerState (max: %d)", s, TOKEN_TYPE_MAX);
    } else printf("%s", lexerstate_names[s]);
}

#include "../build/grammars/grammar.tab.h"

int yylex(void) {
    if (*thelexer->cchar == '\0') return YYEOF;

    switch (*thelexer->cchar) {
        case ' ':
        case '\t':
            thelexer->cchar++;
    }

    // Assign & consume current character.
    int c = *thelexer->cchar++;

    switch (c) {
        case '+':
            return PLUS;
    }

    if (isdigit(c)) {
        int value = c - '0';  // Start with the first digit
        while (isdigit(*thelexer->cchar)) {
            value = value * 10 + (*thelexer->cchar - '0');  // Accumulate value
            thelexer++;
        }
        yylval.intval = value;  // Set the token value
        return NUM; // Return the INTEGER token type
    }

    fprintf(stderr, "Unexpected character: %c\n", c);

    return 0;
}
