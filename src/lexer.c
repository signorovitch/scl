#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <math.h>

#include "include/lexer.h"

int acc_int(int c) {
    int value = c - '0';
    while (isdigit(*inp)) {
        value = value * 10 + (*inp - '0'); // Accumulate value.
        inp++;
    }
    return value;
}

double acc_float(int c) {
    int dplaces = 0;
    double value = (double)(c - '0');

    // Grab everything prior to '.'.
    while (isdigit(*inp)) {
        value = value * 10 + (*inp - '0'); // Accumulate value.
        inp++;
    }

    if (*inp ==  '.') {
        inp++;

        while (isdigit(*inp)) {
            // TODO:
            // Accumulate as int, divide once at end.
            // value = value + (((double)(*inp - '0'))/pow(10.0l, (double)(inp-oinp))); // Accumulate value.
            value = value * 10 + (*inp - '0'); // Accumulate value.
            dplaces++;
            inp++;
        }
        value = value / pow(10, dplaces);
    }

    // > 1.20000
    // = 1.0 + 2/10

    // > 1.23
    // = 1.2 + 3/100
    return value;
}

int yylex() {
    if (*inp == '\0') return YYEOF;

    // Skip all whitespace.
    while (*inp == ' ' || *inp == '\t') { inp++; }

    // Assign & consume current character.
    int c = *inp++;

    // Check for NUM.
    if (isdigit(c)) {
        yylval.fval = acc_float(c); // Set the token value.
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
void yyerror(char const* s) { fprintf(stderr, "Parse error: %s\n", s); }
