#include "../src/include/util.h"
#include "registry.h"
#include "unity/unity.h"

int test_is_even() {
    UNITY_BEGIN();
    TEST_ASSERT_EQUAL(0, is_even(1));
    TEST_ASSERT_EQUAL(1, is_even(2));
    return UNITY_END();
}

__attribute__((constructor)) void register_tests_util() {
    register_test(test_is_even);
}
