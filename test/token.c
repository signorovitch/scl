#include <stdlib.h>
#include "unity/unity.h"

#include "registry.h"
#include "unity/unity_internals.h"
#include "../src/include/token.h"

void test_token_init() {
    char* s = malloc(sizeof("Hello, world!"));
    s = "Hello, world!";
    Token* t = token_init(TOKEN_TYPE_CALL, s);
    TEST_ASSERT_EQUAL(TOKEN_TYPE_CALL, t->type);
    TEST_ASSERT_EQUAL_STRING("Hello, world!", t->val);
}

void test_token_destroy() {
    char* s = malloc(1 * sizeof(char));
    *s = 'h';
    Token* t = token_init(TOKEN_TYPE_CALL, s);
    token_destroy(t);
}

void token_test() {
    UNITY_BEGIN();
    RUN_TEST(test_token_init);
    UNITY_END();
}

__attribute__((constructor)) void register_tests_token() {
    register_test(token_test);
}
