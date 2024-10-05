#include "unity/unity.h"
#include <stdlib.h>

#include "../src/include/token.h"
#include "registry.h"

int test_token_init() {
    UNITY_BEGIN();
    char* s = malloc(sizeof("Hello, world!"));
    s = "Hello, world!";
    Token* t = token_init(TOKEN_TYPE_CALL, s);
    TEST_ASSERT_EQUAL(TOKEN_TYPE_CALL, t->type);
    TEST_ASSERT_EQUAL_STRING("Hello, world!", t->val);
    return UNITY_END();
}

__attribute__((constructor)) void register_tests_token() {
    register_test(test_token_init);
}
