#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef enum {
    AST_TYPE_NUM, // A number.
    AST_TYPE_CALL, // A function call.
    AST_TYPE_VREF, // A variable reference.
    AST_TYPE_MAX = AST_TYPE_CALL
} ASTType;

typedef struct {
    ASTType type;
    void* data;
} AST;

AST* ast_init(ASTType type, void* data);
void ast_destroy(AST* ast);
void ast_print(AST* ast);
void ast_print_i(AST* ast, int i);

typedef double ASTNumData;

ASTNumData* ast_num_data_init(double val);
void ast_num_data_destroy(ASTNumData* num);
void ast_num_print(ASTNumData*, int i);

typedef struct {
    char* to;       // What the call's to.
    size_t argc;    // Argument count.
    AST** argv;     // Argument vector.
} ASTCallData;

ASTCallData* ast_call_data_init(char* to, size_t argc, AST** argv);
void ast_call_data_destroy(ASTCallData* call);
void ast_call_print(ASTCallData*, int i);

typedef struct {
    char* to;       // What the reference's to.
} ASTVrefData;

ASTCallData* ast_vref_data_init(char* to);
void ast_vref_data_destroy(ASTCallData* call);
void ast_vref_print(ASTVrefData*, int i);

#endif
