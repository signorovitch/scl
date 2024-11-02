#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <assert.h>

#include "token.h"

#define TOKENS_MAX 32
#define ZERO_CHAR 30

// What the lexer is currently looking at.
typedef enum {
    LEXER_STATE_CONFUSED, // Can't decide what it's looking at (also initial
                          // state).
    LEXER_STATE_NUM,      // Looking at a number.
    LEXER_STATE_CALL,     // Looking at a call.
    LEXER_STATE_MAX = LEXER_STATE_CALL,
} LexerState;

static char* lexerstate_names[] = {
    [LEXER_STATE_CONFUSED] = "CONFUSED",
    [LEXER_STATE_NUM] = "NUM",
    [LEXER_STATE_CALL] = "CALL",
};

// Lexer: converts text to tokens.
typedef struct {
    LexerState state; // What the lexer is looking at.
    size_t srcln;     // The number of source chars.
    char* src;        // The source text.
    char* cchar;      // The current character.
    size_t ntokens;   // The number of tokens.
    Token** tokens;   // The tokens produced.
} Lexer;

// Create a lexer.
Lexer* lexer_init(char* src);

// Destroy a lexer.
// Does not destroy `lexer->src`!
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

// Print a representation of a Lexer.
void lexer_print(Lexer* lexer);

// Print a representation of a Lexer at specified indentation level.
void lexer_print_i(Lexer* lexer, int ilvl);

// Print a representation of a LexerState.
void lexerstate_print_raw(LexerState s);

// Interface with bison.
int yylex();

#endif
