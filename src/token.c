#include "include/token.h"

Token* token_init(TokenType type, char* val) {
    Token* t = malloc(sizeof(Token));

    t->type = type;
    t->val = val;

    return t;
}

void token_destroy(Token* t) {
    free(t->val);
    free(t);
}

void token_print(Token* token) {
    printf("Token @%p:\n", token);
    printf("\ttype: %s\n", token_type_str(token));
    printf("\tval: %s\n", token->val);
    printf("\tlen: %ld\n", token->len);
}

static char* token_type_str(Token* token) {
    switch (token->type) {
    case TOKEN_TYPE_CALL:   return "CALL";
    case TOKEN_TYPE_NUMBER: return "NUMBER";
    default:                return "???";
    }
}
