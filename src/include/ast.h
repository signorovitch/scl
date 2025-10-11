#ifndef AST_H
#define AST_H

#include "scope.h"
#include <stdlib.h>

// The type of an `AST`.
typedef enum {
    // Primitive type literals.
    AST_TYPE_LIT_NUM,  // A number (float) literal.
    AST_TYPE_LIT_BOOL, // A boolean literal.
    AST_TYPE_LIT_VEC,  // A vector literal.
    AST_TYPE_LIT_KIND, // A kind literal.

    AST_TYPE_EXC_CON,  // Exception constructor `Exc`.
    AST_TYPE_VEC_CON,  // Vectpr constructor `Vec()`.

    // Collection types:
    AST_TYPE_VEC,  // A vector (fixed size, fixed type).
    AST_TYPE_LIST, // A list (variable size, variable type).

    // Syntactic types:
    AST_TYPE_FORCE,
    AST_TYPE_PRESERVE,
    AST_TYPE_BLOCK,  // A block of code (scope).
    AST_TYPE_ARG,    // A definition argument.

    // Misc. types.
    AST_TYPE_BIF,    // Built-in function.
    AST_TYPE_CALL,   // A function call.
    AST_TYPE_DEF,   // A definition.
    AST_TYPE_REF,   // A variable reference.
    AST_TYPE_LAMBDA, // An anonymous function definition.
    AST_TYPE_EXC,   // An exception.
    AST_TYPE_MAX = AST_TYPE_EXC,
} ASTType;

// An Abstract Syntax Tree.
typedef struct {
    ASTType type; // The type of the `AST`.
    void* data;   // The data of the `AST`.
    Scope* scope; // The scope of the `AST`.
} AST;

// Create a new `AST`.
AST* ast_init(ASTType type, void* data);
// Create a new `AST` with a specified scope.
AST* ast_init_scope(ASTType type, void* data, Scope* scope);
// Destroy an `AST`.
void ast_destroy(AST* ast);

// A number.
typedef double ASTNumData;

// Create a new `ASTNumData`.
ASTNumData* ast_num_data_init(double val);
// Destroy an `ASTNumData`.
void ast_num_data_destroy(ASTNumData* num);

// A boolean.
typedef int ASTBoolData;

// Create a new `ASTBoolData`.
ASTBoolData* ast_bool_data_init(int val);
// Destroy an `ASTBoolData`.
void ast_bool_data_destroy(ASTBoolData* bol);

// A literal kind.
typedef enum {
    AST_LIT_KIND_BOOL,
    AST_LIT_KIND_NUM,
    AST_LIT_KIND_KIND,
    AST_LIT_KIND_MAX = AST_LIT_KIND_KIND
} ASTKindData;

extern const char* ast_lit_kind_names[AST_LIT_KIND_MAX + 2];

// Create a new `ASTKindData`.
ASTKindData* ast_kind_data_init(ASTKindData);

// Destroy an `ASTKindData`.
void ast_kind_data_destroy(ASTKindData*);

// An exception.
typedef struct ASTEXCDATA {
    const char* msg; // The exception message.
    AST* trace;      // The previous exception.
} ASTExcData;
// Create a new `ASTExecData. `msg` should be static.
ASTExcData* ast_exc_data_init(const char* msg, AST* trace);
// Destroy an `ASTExecData`.
void ast_exc_data_destroy(ASTExcData* exc);

// Argument list as anonymous struct.
#define ARGS                                                                   \
    struct {                                                                   \
        size_t argc;                                                           \
        AST** argv;                                                            \
    }

// Parameter list as anonymous struct.
#define PARS                                                                   \
    struct {                                                                   \
        size_t parc;                                                           \
        AST** parv;                                                            \
    }

// A built-in function.
typedef AST* (*ASTBIFData)(size_t argc, AST** argv, Scope* scope);

// Create a built-in function.
ASTBIFData* ast_bif_data_init(AST* fn(size_t, AST**, Scope*));
// Destroy an `ASTBIFData`.
void ast_bif_data_destroy(ASTBIFData* bif);

// A lambda.
typedef struct {
    PARS;      // The parameters the lambda can accept.
    AST* body; // The body expression to be executed.
} ASTLambdaData;

// Creates a new `ASTLambdaData`.
ASTLambdaData* ast_lambda_data_init(size_t parc, AST** parv, AST* body);
// Destroy an `ASTLambdaData`.
void ast_lambda_data_destroy(ASTLambdaData*);

// A call.
typedef struct {
    ARGS;     // The arguments the call is made with.
    AST* exp; // The expression the call is to.
} ASTCallData;

// Create a new `ASTCallData`.
ASTCallData* ast_call_data_init(size_t argc, AST** argv, AST* exp);
// Destroy an `ASTCallData`.
void ast_call_data_destroy(ASTCallData* call);

// A definition. Associates a name and kind with an expression.
typedef struct {
    char* name;
    AST* kind; // If NULL, assume `Any` kind.
    AST* exp;
} ASTDefData;

// Create a new `ASTDefData`.
ASTDefData* ast_def_data_init(char* name, AST* kind, AST* exp);
// Destroy an `ASTDefData`.
void ast_def_data_destroy(ASTDefData* vdef);

// A reference.
typedef struct {
    char* to; // What the reference's to.
} ASTRefData;

// Create a new `ASTRefData`.
ASTRefData* ast_ref_data_init(char* to);
// Destroy an `ASTRefData`.
void ast_ref_data_destroy(ASTRefData* call);

// A code block.
typedef struct {
    AST** inside; // What's inside the block.
    size_t ln;    // How many ASTs are in the block.
} ASTBlockData;

// Create a new `ASTBlockData`.
ASTBlockData* ast_block_data_init(AST** inside, size_t ln);
// Destroy an `ASTBlockData`, recursively.
void ast_block_data_destroy(ASTBlockData* block);

typedef struct {
    char* name; // Argument name.
} ASTArgData;

// Create a new `ASTArgData`.
ASTArgData* ast_arg_data_init(char* name);
// Destroy an `ASTArgData`.
void ast_arg_data_destroy(ASTArgData* arg);

// Find the expression associated with a name in the nearest scope.
AST* ast_find(Scope* scope, char* name);

// A force operator.
typedef struct {AST* body;} ASTForceData;
ASTForceData* ast_force_data_init(AST* body);
void ast_force_data_destroy(ASTForceData* force);

// A preserve operator.
typedef struct {AST* body;} ASTPreserveData;
ASTPreserveData* ast_preserve_data_init(AST* body);
void ast_preserve_data_destroy(ASTPreserveData* preserve);

#endif
