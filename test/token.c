#include "unity/unity.h"
#include <stdlib.h>

#include "../src/include/token.h"
#include "registry.h"
#include "unity/unity_internals.h"

void test_token_init() {
    char* s = malloc(sizeof("Hello, world!"));
    s = "Hello, world!";
    Token* t = token_init(TOKEN_TYPE_CALL, s);
    TEST_ASSERT_EQUAL(TOKEN_TYPE_CALL, t->type);
    TEST_ASSERT_EQUAL_STRING("Hello, world!", t->val);
}

int test_token() {
    UNITY_BEGIN();
    RUN_TEST(test_token_init);
    return UNITY_END();
}

__attribute__((constructor)) void register_token() {
    register_test(test_token);
}
