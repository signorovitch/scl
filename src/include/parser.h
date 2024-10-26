#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"

typedef struct {
    size_t tokenc;      // Number of tokens in tokenv;
    Token* ctoken;      // The current token.
    Token** tokenv;     // Token vector.
    AST* ast;           // Abstract syntax tree.
} Parser;

Parser* parser_init(size_t tokenc, Token** tokenv);
void parser_destroy(Parser* parser);

void parser_inc(Parser* parser);

#endif
