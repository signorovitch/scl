#ifndef AST_PRINT_H
#define AST_PRINT_H

#include "ast.h"

// Print an `AST`, recursively.
void ast_print(AST* ast);
// Helper function to `ast_print()`, where `i` is indentation level.
void ast_print_i(AST* ast, int i);

// Print an `ASTNumData`.
void ast_num_print(ASTNumData*, int i);

// Print an `ASTBoolData`.
void ast_bool_print(ASTBoolData*, int i);

// Print an `ASTExecData`.
void ast_exc_print(ASTExcData*, int i);

// Print an `ASTCallData`.
void ast_call_print(ASTCallData*, int i);

// Print an `ASTVDefData`.
void ast_vdef_print(ASTVDefData*, int depth);

// Print an `ASTVRefData`.
void ast_vref_print(ASTVrefData*, int i);

// Print an `ASTBlockData`.
void ast_block_print(ASTBlockData*, int i);

// Print an `ASTFDefData`.
void ast_fdef_print(ASTFDefData* fdef, int i);

// Print an `ASTArgData`.
void ast_arg_print(ASTArgData* arg, int i);

// Print an `ASTLambdaData`.
void ast_lambda_print(ASTLambdaData* arg, int i);

// Print an `ASTBIFData`.
void ast_bif_print(ASTBIFData* arg, int i);

#endif
