#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>

typedef enum {
    TOKEN_TYPE_CALL,
    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_MAX = TOKEN_TYPE_NUMBER,
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

// Prints out a representation of the Token.
void token_print(Token* token);

// Prints out a representation of the Token, with the specified indent level.
void token_print_i(Token* token, int ilevel);

// Prints out a representation of the TokenType.
void tokentype_print(TokenType t);

// Prints out a representation of the TokenType, with the specified indent
// level.
void tokentype_print_i(TokenType t, int ilevel);

// Prints a token's type. That's it.
void tokentype_print_raw(TokenType t);

#endif
