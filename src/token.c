#include "include/token.h"
#include <stdio.h>

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

char* token_to_str(Token* token, unsigned int indent) {
    char* title = malloc(sizeof("Token @ 0x000000000000\n"));
    sprintf(title, "Token @%p\n", token);
    return title;
}
