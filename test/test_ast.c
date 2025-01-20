#include "../src/include/ast.h"
#include "Unity/src/unity.h"
#include "Unity/src/unity_internals.h"

void setUp() {}
void tearDown() {}

void test_ast_num() {
    ASTNumData* num = ast_num_data_init(12.0);
    AST* ast = ast_init(AST_TYPE_NUM, num);

    TEST_ASSERT_EQUAL(AST_TYPE_NUM, ast->type);
    TEST_ASSERT_EQUAL(12.0, *(ASTNumData*)ast->data);
}


int main() {
    UNITY_BEGIN();
    RUN_TEST(test_ast_num);
    return UNITY_END();
}
