#include "../src/include/ast.h"
#include "Unity/src/unity.h"
#include "Unity/src/unity_internals.h"
#include <string.h>

void setUp() {}
void tearDown() {}

void test_ast_num() {
    ASTNumData* num = ast_num_data_init(12.0);
    AST* ast = ast_init(AST_TYPE_NUM, num);

    TEST_ASSERT_EQUAL(AST_TYPE_NUM, ast->type);
    TEST_ASSERT_EQUAL(12.0, *(ASTNumData*)ast->data);

    ast_destroy(ast);
}

void test_ast_call() {
    AST** argv = malloc(2*sizeof(AST*));
    argv[0] = ast_init(AST_TYPE_NUM, ast_num_data_init(1.0));
    argv[1] = ast_init(AST_TYPE_NUM, ast_num_data_init(2.0));

    char* name = malloc(2);
    strcpy(name, "f");

    ASTCallData* call = ast_call_data_init(name, 2, argv);

    AST* ast = ast_init(AST_TYPE_CALL, call);

    TEST_ASSERT_EQUAL(AST_TYPE_CALL, ast->type);
    TEST_ASSERT_EQUAL(name, ((ASTCallData*)ast->data)->to);
    TEST_ASSERT_EQUAL(2, ((ASTCallData*)ast->data)->argc);
    TEST_ASSERT_EQUAL(1.0, ((ASTCallData*)ast->data)->argv[0]);
    TEST_ASSERT_EQUAL(2.0, ((ASTCallData*)ast->data)->argv[1]);

    ast_destroy(ast);
}

void test_ast_vref() {
    char* s = malloc(2);
    strcpy(s, "x");
    ASTVrefData* vref = ast_vref_data_init(s);
    AST* ast = ast_init(AST_TYPE_VREF, vref);

    TEST_ASSERT_EQUAL(AST_TYPE_VREF, ast->type);
    ASTVrefData data = *(ASTVrefData*)ast->data;
    TEST_ASSERT_EQUAL_STRING("x", data.to);

    //ast_destroy(ast);
}

int main() {
    UNITY_BEGIN();
    //RUN_TEST(test_ast_num);
    //RUN_TEST(test_ast_call);
    //RUN_TEST(test_ast_vref);
    return UNITY_END();
}
