#ifndef AST_H
#define AST_H

#include <stdlib.h>

typedef enum {
    // Primitive types.
    AST_TYPE_NUM, // A number (float).
    AST_TYPE_STR, // A string
    AST_TYPE_INT, // An integer.
    AST_TYPE_SYM, // A symbol.
    AST_TYPE_EXC, // Exception.

    // Collection types:
    AST_TYPE_VEC,  // A vector (fixed size, fixed type).
    AST_TYPE_LIST, // A list (variable size, variable type).

    // Misc. types.
    AST_TYPE_CALL,  // A function call.
    AST_TYPE_VDEF,  // A variable definition.
    AST_TYPE_VREF,  // A variable reference.
    AST_TYPE_BLOCK, // A block of code (scope).
    AST_TYPE_MAX = AST_TYPE_BLOCK,
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

// An exception.
typedef char* ASTExcData;
ASTExcData* ast_exc_data_init(char* msg);
void ast_exc_data_destroy(ASTExcData* exc);

typedef struct {
    char* to;    // What the call's to.
    size_t argc; // Argument count.
    AST** argv;  // Argument vector.
} ASTCallData;

ASTCallData* ast_call_data_init(char* to, size_t argc, AST** argv);
void ast_call_data_destroy(ASTCallData* call);
void ast_call_print(ASTCallData*, int i);

// A variable definition's data.
typedef struct {
    char* name;
    AST* val;
} ASTVDefData;

ASTVDefData* ast_vdef_data_init(char* name, AST* val);
// Destroys the vdef, its name, and its ->val.
void ast_vdef_data_destroy(ASTVDefData* vdef);
void ast_vdef_print(ASTVDefData*, int depth);

// A variable reference's data.
typedef struct {
    char* to; // What the reference's to.
} ASTVrefData;

ASTVrefData* ast_vref_data_init(char* to);
void ast_vref_data_destroy(ASTVrefData* call);
void ast_vref_print(ASTVrefData*, int i);

typedef struct {
    AST** inside; // What's inside the block.
    size_t ln;    // How many ASTs are in the block.
} ASTBlockData;

ASTBlockData* ast_block_data_init(AST** inside, size_t ln);
// Destroy a block. Also destroy all ASTs inside.
void ast_block_data_destroy(ASTBlockData* block);
void ast_block_print(ASTBlockData*, int i);

#endif
