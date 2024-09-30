#include "unity/unity.h"

#include "../src/include//util.h"

void setUp() {}
void tearDown() {}

void test_is_even() {
    TEST_ASSERT_EQUAL(0, is_even(1));
    TEST_ASSERT_EQUAL(1, is_even(2));
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_is_even);
    return UNITY_END();
}
