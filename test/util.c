#include "unity/unity.h"
#include "registry.h"
#include "unity/unity_internals.h"
#include "../src/include/util.h"

void test_is_even() {
    TEST_ASSERT_EQUAL(0, is_even(1));
    TEST_ASSERT_EQUAL(1, is_even(2));
}

void util_test() {
    UNITY_BEGIN();
    RUN_TEST(test_is_even);
    UNITY_END();
}

__attribute__((constructor)) void register_tests_util() {
    register_test(util_test);
}
