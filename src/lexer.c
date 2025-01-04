#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>

#include "include/dstr.h"
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

    if (*inp == '.') {
        inp++;

        while (isdigit(*inp)) {
            value = value * 10 + (*inp - '0'); // Accumulate value.
            dplaces++;
            inp++;
        }
        value = value / pow(10, dplaces);
    }
    return value;
}

char* acc_word(int c) {
    Dstr* val = dstr_init();
    do {
        dstr_appendch(val, *(inp - 1));
        inp++;
    } while (isalpha(*inp));
    dstr_appendch(val, *(inp - 1));

    return val->buf;
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

    if (isalpha(c)) {
        yylval.strval = acc_word(c);
        return CALL;
    }

    switch (c) {
        case '+':  return PLUS;
        case '\n': return NL;
        case '-':  return SUB;
        case '*':  return MULT;
        case '/':  return DIV;
        case '(':  return LGROUP;
        case ')':  return RGROUP;
        case ',':  return SEP;
        default:   fprintf(stderr, "Unexpected character: %c\n", c);
    }

    return 0;
}
void yyerror(char const* s) { fprintf(stderr, "Parse error: %s\n", s); }
