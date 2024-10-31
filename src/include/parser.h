#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"
#include "stack.h"

typedef struct {
    size_t tokenc;      // Number of tokens in tokenv;
    Token* ctoken;      // The current token.
    Token** tokenv;     // Token vector.
    AST* ast;           // Abstract syntax tree.
    Stack* ops;
    Stack* nums;
} Parser;

Parser* parser_init(size_t tokenc, Token** tokenv);
void parser_destroy(Parser* parser);

// Increment `parser->ctoken`.
void parser_inc(Parser* parser);

// Step forward 1 token and add to stacks.
void parser_step(Parser* parser);

#endif
