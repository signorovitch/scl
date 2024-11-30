#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef enum {
    AST_TYPE_NUM,
    AST_TYPE_CALL,
    AST_TYPE_MAX = AST_TYPE_CALL
} ASTType;

typedef struct {
    ASTType type;
    void* data;
} AST;

AST* ast_init(ASTType type, void* data);
void ast_destroy(AST* ast);
void ast_print(AST* ast);

typedef struct {
    int val;
} ASTNumData;

ASTNumData* ast_type_num_init(int val);
void ast_type_num_destroy(ASTNumData* num);

typedef struct {
    char* to;       // What the call's to.
    size_t argc;    // Argument count.
    AST** argv;     // Argument vector.
} ASTCallData;

ASTCallData* ast_type_call_init(char* to, size_t argc, AST** argv);
void ast_type_call_destroy(ASTCallData* call);

#endif
