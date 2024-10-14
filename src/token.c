#include "include/token.h"
#include "include/dstr.h"
#include <stdlib.h>

Token* token_init(TokenType type, char* val, size_t valn) {
    Token* t = malloc(sizeof(Token));

    t->type = type;
    t->val = val;
    t->valn = valn;

    return t;
}

void token_destroy(Token* t) {
    free(t->val);
    free(t);
}

Dstr* token_to_dstr(Token* token) {
    Dstr* str = dstr_init();

    size_t titlesz = sizeof("Token @ 0x00000000");
    char title[titlesz];
    sprintf(title, "Token @ %p", token);
    dstr_append(str, title, titlesz - 1);
    dstr_append(str, "\n", 1);

    size_t typesz = sizeof("type: 1");
    char type[typesz];
    // If token_to_dstr starts breaking, it might be because there're more than
    // 10 types. FIXME.
    sprintf(type, "type: %d", token->type);
    dstr_append(str, type, typesz - 1);
    dstr_append(str, "\n", 1);

    dstr_append(str, "val: ", 5);
    dstr_append(str, token->val, token->valn);

    return str;
}
