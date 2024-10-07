#include <stdio.h>

#include "include/util.h"
#include "include/lexer.h"

int main(int argc, char** argv) {
    char* text = malloc(5); 
    text = "a1b2";

    Lexer* lexer = lexer_init(text);
    lexer_print(lexer);
    lexer_lex(lexer);
    lexer_print(lexer);
}
