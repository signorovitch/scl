#ifndef AST_H
#define AST_H

typedef enum {
    AST_TYPE_NUM,
    AST_TYPE,CALL
} ASTType;

typedef struct {
    ASTType type;
    void* data;
} AST;

typedef struct {
    int val;
} ASTTypeNum;

typedef struct {
    char* to;
    char** args;
} ASTTypeCall;

#endif
