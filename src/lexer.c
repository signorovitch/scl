#include <complex.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "include/dstr.h"
#include "include/lexer.h"
#include "include/util.h"

ArgArr* argarr_init() {
    ArgArr* argarr = malloc(sizeof(ArgArr));

    argarr->sz = ARLN * sizeof(AST*);
    argarr->ln = 0;
    argarr->buf = malloc(argarr->sz);

    return argarr;
}

void argarr_destroy(ArgArr* argarr) {
    free(argarr->buf);
    free(argarr);
}

void argarr_destroypsv(ArgArr* argarr) { free(argarr); }

void argarr_add(ArgArr* argarr, AST* arg) {
    if ((argarr->ln + 1) * sizeof(AST*) > argarr->sz) {
        argarr->sz *= 2;
        argarr->buf = realloc(argarr->buf, argarr->sz);
        log_dbgf(
            "ArgArr @ %p doubled from %ld to %ld", argarr, argarr->sz / 2,
            argarr->sz
        );
    }

    argarr->buf[argarr->ln++] = arg;
}

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
    while (isalpha(*inp) || *inp == '_') {
        dstr_appendch(val, *(inp - 1));
        inp++;
    }
    dstr_appendch(val, *(inp - 1));

    char* ret = val->buf;
    dstr_destroypsv(val);
    return ret;
}

char* acc_deq(int c) {
    Dstr* val = dstr_init();
    while (*inp == '=') {
        dstr_appendch(val, *(inp - 1));
        inp++;
    }
    dstr_appendch(val, *(inp - 1));

    char* ret = val->buf;
    dstr_destroypsv(val);
    return ret;
}

int yylex() {
    if (*inp == '\0') return YYEOF;

    // Skip all whitespace.
    while (*inp == ' ' || *inp == '\t') inp++;

    // Assign & consume current character.
    int c = *inp++;

    // Check for NUM.
    if (isdigit(c)) {
        yylval.fval = acc_float(c); // Set the token value.
        return NUM;
    }

    if (c == '=') {
        yylval.strval = acc_deq(c);
        if (!strcmp(yylval.strval, "=")) return EQ;
        if (!strcmp(yylval.strval, "==")) return DEQ;
    }

    if (isalpha(c) || c == '_') {
        yylval.strval = acc_word(c);

        if (!strcmp(yylval.strval, "TRUE") || !strcmp(yylval.strval, "T"))
            return BOOLT;
        if (!strcmp(yylval.strval, "FALSE") || !strcmp(yylval.strval, "F"))
            return BOOLF;
        if (!strcmp(yylval.strval, "if")) return IF;
        if (!strcmp(yylval.strval, "else")) return ELSE;

        return WORD;
    }

    switch (c) {
        case '+':  return ADD;
        case '\n': return NL;
        case '-':  return SUB;
        case '*':  return MUL;
        case '/':  return DIV;
        case '(':  return GROUPS;
        case ')':  return GROUPE;
        case ',':  return SEP;
        case ';':  return EXPSEP;
        case '{':  return BLOCKS;
        case '}':  return BLOCKE;
        // case '=':  return EQ;
        case '\\': return BACKSLASH;
        case '?':  return IF;
        case ':':  return ELSE;
        default:   fprintf(stderr, "Unexpected character: %c\n", c);
    }

    return 0;
}
void yyerror(char const* s) { fprintf(stderr, "Parse error: %s\n", s); }
