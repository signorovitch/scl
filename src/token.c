#include <stdio.h>

#include "include/token.h"
#include "include/dstr.h"
#include "include/util.h"

static char* tokentype_names[] = {
    [TOKEN_TYPE_CALL] = "CALL",
    [TOKEN_TYPE_NUMBER] = "NUMBER",
};

Token* token_init(TokenType type, char* val, size_t valn) {
    Token* t = malloc(sizeof(Token));

    t->type = type;
    t->valn = valn;
    t->val = val;

    return t;
}

void token_destroy(Token* t) {
    free(t->val);
    free(t);
}

void token_print(Token* token) { token_print_i(token, 0); }

#if 0
void token_print_i(Token *token, int ilvl) {
    Dstr* spacing = dstr_init();
    for (int j = 0; j < ilvl; j++) dstr_appendch(spacing, ' ');

    printf("%sToken @ %p\n", spacing->buf, token);
    printf("%s type:\n", spacing->buf);
    tokentype_print_i(token->type, ilvl+1);
    printf("%s valn:\n", spacing->buf);
    printf("%s  %ld\n", spacing->buf, token->valn);
    printf("%s val:\n", spacing->buf);
    printf("%s  \"%s\"\n", spacing->buf, token->val);

    // <spacing> <field>:<value>
}
#endif

void token_print_i(Token *token, int ilvl) {
    INDENT_BEGIN(ilvl);

    INDENT_TITLE("Token", token);
    INDENT_FIELD_NONL("type");
    tokentype_print_raw(token->type);
}

void tokentype_print_raw(TokenType t) {
    if (t > TOKEN_TYPE_MAX) {
        printf("Unknown (%d)", t);
        log_dbgf("%d is not a valid TokenType (max: %d)", t, TOKEN_TYPE_MAX);
        return;
    }

    printf("%s", tokentype_names[t]);
}

void tokentype_print(TokenType t) { tokentype_print_i(t, 0); }

void tokentype_print_i(TokenType t, int i) {
    INDENT_BEGIN(i);

    if (t > TOKEN_TYPE_MAX) {
        INDENT_FIELD("val", "Unknown (%d)", t);
        log_dbgf("%d is not a valid TokenType (max: %d)", t, TOKEN_TYPE_MAX);
        return;
    }

    INDENT_FIELD("val", "%s", tokentype_names[t]);
}
