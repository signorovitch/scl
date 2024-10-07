#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "token.h"

#define TOKENS_MAX 32
#define ZERO_CHAR 30

// What the lexer is currently looking at.
typedef enum {
    LEXER_STATE_CONFUSED, // Can't decide what it's looking at (also initial
                          // state).
    LEXER_STATE_NUM,      // Looking at a number.
    LEXER_STATE_CALL,     // Looking at a call.
} LexerState;

// Lexer: converts text to tokens.
typedef struct {
    char* src;        // The source text.
    size_t srcl;      // The number of source chars.
    char* cchar;      // The current character.
    Token** tokens;   // The tokens produced.
    size_t ntokens;   // The number of tokens.
    LexerState state; // What the lexer is looking at.
} Lexer;

// Create a lexer.
Lexer* lexer_init(char* src);

// Destroy a lexer.
void lexer_destroy(Lexer* lexer);

// Convert text to tokens.
void lexer_lex(Lexer* lexer);

// Lex in confused mode.
void lexer_do_confused(Lexer* lexer);

// Lex in number mode.
void lexer_do_number(Lexer* lexer);

// Lex in call mode.
void lexer_do_call(Lexer* lexer);

// Increment the lexer's current character pointer.
void lexer_inc(Lexer* lexer);

// Add a token to the lexer.
void lexer_add_token(Lexer* lexer, Token* token);

// Print the contents of a lexer.
void lexer_print(Lexer* lexer);

static char* lexer_state_str(Lexer* lexer);

#endif
