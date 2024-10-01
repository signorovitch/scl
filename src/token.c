#include <stdlib.h>

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
