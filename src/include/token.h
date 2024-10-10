#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    TOKEN_TYPE_CALL,
    TOKEN_TYPE_NUMBER,
} TokenType;

// Token.
typedef struct {
    TokenType type; // The type of the Token.
    char* val;      // The text of the Token.
    size_t len;     // Length of the text of the Token.
} Token;

Token* token_init(TokenType type, char* val);
void token_destroy(Token* token);

// Returns a string representation of the Token.
char* token_to_str(Token* token, unsigned int indent);

// Returns a string representation of the TokenType.
char* token_type_to_str(TokenType t);

#endif
