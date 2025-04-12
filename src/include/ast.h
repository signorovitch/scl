#ifndef AST_H
#define AST_H

#include <stdlib.h>

// The type of an `AST`.
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
    AST_TYPE_BIF,   // Built-in function.
    AST_TYPE_CALL,  // A function call.
    AST_TYPE_VDEF,  // A variable definition.
    AST_TYPE_VREF,  // A variable reference.
    AST_TYPE_BLOCK, // A block of code (scope).
    AST_TYPE_FDEF,  // A function definition.
    AST_TYPE_ARG,   // A definition argument.
    AST_TYPE_MAX = AST_TYPE_FDEF,
} ASTType;

// An Abstract Syntax Tree.
typedef struct {
    ASTType type; // The type of the `AST`.
    void* data;   // The data of the `AST`.
} AST;

// Create a new `AST`.
AST* ast_init(ASTType type, void* data);
// Destroy an `AST`, recursively.
void ast_destroy(AST* ast);
// Print an `AST`, recursively.
void ast_print(AST* ast);
// Helper function to `ast_print()`, where `i` is indentation level.
void ast_print_i(AST* ast, int i);

// A number.
typedef double ASTNumData;

// Create a new `ASTNumData`.
ASTNumData* ast_num_data_init(double val);
// Destroy an `ASTNumData`.
void ast_num_data_destroy(ASTNumData* num);
// Print an `ASTNumData`.
void ast_num_print(ASTNumData*, int i);

// An exception.
typedef struct ASTEXCDATA {
    char* msg;  // The exception message.
    AST* trace; // The previous exception.
} ASTExcData;
// Create a new `ASTExecData.
ASTExcData* ast_exc_data_init(char* msg, AST* trace);
// Destroy an `ASTExecData`.
void ast_exc_data_destroy(ASTExcData* exc);
// Print an `ASTExecData`.
void ast_exc_print(ASTExcData*, int i);

// A built-in function.
typedef AST* (*ASTBIFData)(size_t argc, AST** argv);

// Create a built-in function.
ASTBIFData* ast_bif_data_init(AST* fn(size_t, AST**));

// There is no `ASTBIFData` destroy function, as function pointers are immortal.

// A call (to a function).
typedef struct {
    char* to;    // What the call's to.
    size_t argc; // Argument count.
    AST** argv;  // Argument vector.
} ASTCallData;

// Create a new `ASTCallData`.
ASTCallData* ast_call_data_init(char* to, size_t argc, AST** argv);
// Destroy an `ASTCallData`.
void ast_call_data_destroy(ASTCallData* call);
// Print an `ASTCallData`.
void ast_call_print(ASTCallData*, int i);

// A variable definition's data.
typedef struct {
    char* name;
    AST* val;
} ASTVDefData;

// Create a new `ASTVDefData`.
ASTVDefData* ast_vdef_data_init(char* name, AST* val);
// Destroys the `ASTVDefData`, `ASTVDefData->name`, and `ASTVDefData->val`.
void ast_vdef_data_destroy(ASTVDefData* vdef);
// Print an `ASTVDefData`.
void ast_vdef_print(ASTVDefData*, int depth);

// A variable reference's data.
typedef struct {
    char* to; // What the reference's to.
} ASTVrefData;

// Create a new `ASTVRefData`.
ASTVrefData* ast_vref_data_init(char* to);
// Destroy an `ASTVRefData`.
void ast_vref_data_destroy(ASTVrefData* call);
// Print an `ASTVRefData`.
void ast_vref_print(ASTVrefData*, int i);

// A code block.
typedef struct {
    AST** inside; // What's inside the block.
    size_t ln;    // How many ASTs are in the block.
} ASTBlockData;

// Create a new `ASTBlockData`.
ASTBlockData* ast_block_data_init(AST** inside, size_t ln);
// Destroy an `ASTBlockData`, recursively.
void ast_block_data_destroy(ASTBlockData* block);
// Print an `ASTBlockData`.
void ast_block_print(ASTBlockData*, int i);

typedef struct {
    char* name;  // Function name.
    size_t argc; // Argument count.
    AST** argv;  // Argument vector.
    AST* body;   // Function body.
} ASTFDefData;

// Create a new `ASTFDefData`.
ASTFDefData* ast_fdef_data_init(char* name, size_t argc, AST** argv, AST* body);
// Destroy an `ASTFDefData`.
void ast_fdef_data_destroy(ASTFDefData* fdef);
// Print an `ASTFDefData`.
void ast_fdef_print(ASTFDefData* fdef, int i);

typedef struct {
    char* name; // Argument name.
} ASTArgData;

// Create a new `ASTArgData`.
ASTArgData* ast_arg_data_init(char* name);
// Destroy an `ASTArgData`.
void ast_arg_data_destroy(ASTArgData* arg);
// Print an `ASTArgData`.
void ast_arg_print(ASTArgData* arg, int i);

#endif
