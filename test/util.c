#include "../src/include/util.h"
#include "registry.h"
#include "unity/unity.h"

void test_is_even() {
    TEST_ASSERT_EQUAL(0, is_even(1));;
    TEST_ASSERT_EQUAL(1, is_even(2));
}

int test_util() {
    UNITY_BEGIN();
    RUN_TEST(test_is_even);
    return UNITY_END();
}

__attribute__((constructor)) void register_tests_util() {
    register_test(test_util);
}
