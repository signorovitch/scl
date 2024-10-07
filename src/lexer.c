#include "include/lexer.h"
#include "include/token.h"
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

Lexer* lexer_init(char* src) {
    Lexer* lexer = malloc(sizeof(Lexer));

    lexer->src = src;
    lexer->srcl = strlen(src);
    lexer->cchar = lexer->src;

    lexer->tokens = calloc(TOKENS_MAX, sizeof(Token));
    lexer->ntokens = 0;
    lexer->state = LEXER_STATE_CONFUSED;

    return lexer;
}

void lexer_destroy(Lexer* lexer) {
    free(lexer->src);

    for (int i = 0; i < lexer->ntokens; token_destroy(lexer->tokens[i++]));
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
    if (isdigit(*lexer->cchar)) lexer_do_number(lexer);
    else lexer_do_call(lexer);
}

void lexer_do_number(Lexer* lexer) {
    // Size of the number string.
    size_t numsz;

    // Where the number string starts.
    char* start = lexer->cchar;

    for (numsz = 0; *lexer->cchar && isdigit(*lexer->cchar); numsz++)
        lexer_inc(lexer);

    char* num = malloc(numsz + 1);
    memcpy(num, start, numsz);
    num[numsz] = '\0';

    lexer_add_token(lexer, token_init(TOKEN_TYPE_NUMBER, num));
}

void lexer_do_call(Lexer* lexer) {
    // Size of the call string.
    size_t callsz;

    // Where the call string starts.
    char* start = lexer->cchar;

    for(callsz = 0; *lexer->cchar && isalpha(*lexer->cchar); callsz++)
        lexer_inc(lexer);

    char* call = malloc(callsz + 1);
    memcpy(call, start, callsz);
    call[callsz] = '\0';
}

void lexer_inc(Lexer* lexer) {
    lexer->cchar += sizeof(char);
}

void lexer_add_token(Lexer* lexer, Token* token) {
    assert(lexer->ntokens < TOKENS_MAX);

    if (lexer->ntokens < TOKENS_MAX - 1) {
        lexer->tokens[lexer->ntokens - 1] = token;
        lexer->ntokens++;
    }
}

void lexer_print(Lexer* lexer) {
    printf("Lexer @%p:\n", lexer);
    printf("\tsrc: \"%s\"\n", lexer->src);
    printf("\tsrcl: \"%ld\"\n", lexer->srcl);
    printf("\tcchar: \"%s\"\n", lexer->cchar);
    printf("\tntokens: %ld\n", lexer->ntokens);
    printf("\ttokens: [START]\n");
    for (int i = 0; i < lexer->ntokens; i++) token_print(lexer->tokens[i]);
    printf("[END]\n");
    printf("\tstate: %s\n", lexer_state_str(lexer));

}

static char* lexer_state_str(Lexer* lexer) {
    switch (lexer->state) {
    case LEXER_STATE_NUM:      return "NUM";
    case LEXER_STATE_CALL:     return "CALL";
    case LEXER_STATE_CONFUSED: return "CONFUSED";
    default:                   return "???";
    }
}
