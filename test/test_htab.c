#include "../src/include/htab.h"
#include "Unity/src/unity.h"
#include "Unity/src/unity_internals.h"
#include <string.h>

void setUp() {}
void tearDown() {}

void test_htab() {
    char* key = "hello";
    char* data = "world";

    HTab* htab = htab_init();

    htab_ins(htab, key, data);

    TEST_ASSERT_EQUAL_STRING(data, htab_get(htab, key));
    TEST_ASSERT_NOT_EQUAL(data, htab_get(htab, "h"));
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_htab);

    return UNITY_END();
}
