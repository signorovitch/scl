#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "dstr.h"

typedef enum {
    TOKEN_TYPE_CALL,
    TOKEN_TYPE_NUMBER,
} TokenType;

// Token.
typedef struct {
    TokenType type; // The type of the Token.
    size_t valn;    // The length of val.
    char* val;      // The text of the Token.
    size_t len;     // Length of the text of the Token.
} Token;

Token* token_init(TokenType type, char* val, size_t valn);
void token_destroy(Token* token);

// Returns a string representation of the Token.
Dstr* token_to_dstr(Token* token);

// Returns a string representation of the TokenType.
char* token_type_to_str(TokenType t);

#endif
