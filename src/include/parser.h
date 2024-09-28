#ifndef PARSER_H
#define PARSER_H

// Expression one of:
// - Operation
// - Number

// Operation contains:
// - Type
// - Expression 1
// - Expression 2

typedef enum OpType {
    OPTYPE_PLUS,
    OPTYPE_MINUS
} optype_t;

typedef union Exp {
    typedef struct Op {
        optype_t type;
        Exp* exp1;
        Exp* exp2;
    } op_t;

    int n;
} exp_t;



#endif
