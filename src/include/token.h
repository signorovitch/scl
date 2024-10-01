#ifndef TOKEN_H
#define TOKEN_H

typedef enum TokenType {
    TOKEN_TYPE_CALL,
    TOKEN_TYPE_NUMBER,
} TokenType;

// Token.
typedef struct Token {
    TokenType type; // The type of the Token.
    char* val;      // The text of the Token.        
} Token;

Token* token_init(TokenType type, char* val);
void token_destroy(Token* token);

#endif
