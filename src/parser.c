#include "include/util.h"
#include "include/parser.h"

Parser* parser_init(size_t tokenc, Token** tokenv) {
    talloc(Parser, parser);

    parser->tokenc = tokenc;
    parser->ctoken = *tokenv;
    parser->tokenv = tokenv;
    parser->ast = NULL;

    return parser;
}

void parser_destroy(Parser* parser) {
    if (!parser) return;

    // Also frees parser->ctoken.
    for (int i = 0; i < parser->tokenc; i++) token_destroy(parser->tokenv[i]);

    ast_destroy(parser->ast);
}


