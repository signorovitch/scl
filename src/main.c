#include <stdio.h>

#include "include/token.h"
#include "include/util.h"
#include "include/lexer.h"

int main(int argc, char** argv) {
    char* text = malloc(5); 
    text = "aa11";

    Lexer* lexer = lexer_init(text);
    lexer_lex(lexer);

    printf("%s\n", token_to_str(lexer->tokens[0], 0));

    lexer_destroy(lexer);
}
