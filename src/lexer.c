#include <ctype.h>
#include <limits.h>
#include <stdio.h>

#include "include/lexer.h"

int yylex() {
    if (*inp == '\0') return YYEOF;

    // Skip all whitespace.
    while (*inp == ' ' || *inp == '\t') { inp++; }

    // Assign & consume current character.
    int c = *inp++;

    // Check for NUM.
    if (isdigit(c)) {
        int value = c - '0';
        while (isdigit(*inp)) {
            value = value * 10 + (*inp - '0'); // Accumulate value.
            inp++;
        }
        yylval.intval = value; // Set the token value.
        return NUM;
    }

    switch (c) {
    case '+': return PLUS;
    case '\n': return NL;
    default:  return CALL;
    }

    fprintf(stderr, "Unexpected character: %c\n", c);

    return 0;
}
void yyerror(char const* s) { fprintf(stderr, "Syntax error:\n%s\n", s); }
