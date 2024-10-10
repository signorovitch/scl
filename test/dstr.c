#include "../src/include/dstr.h"
#include "unity/unity.h"
#include "registry.h"

void test_dstr_init() {
    Dstr* dstr = dstr_init();

    TEST_ASSERT_EQUAL_INT(DSTR_INITSZ, dstr->bufsz);
    TEST_ASSERT_EQUAL_STRING(malloc(DSTR_INITSZ), dstr->buf);
    TEST_ASSERT_EQUAL_size_t(0, dstr->ln);
}

void test_dstr_append() {
    char* str1 = malloc(2);
    str1 = "h";

    char* str2 = malloc(DSTR_INITSZ);
    str2 = "h";


    Dstr* dstr = dstr_init();
    dstr_append(dstr, 1, str1);

    TEST_ASSERT_EQUAL_STRING(str2, dstr->buf);
}

int test_dstr() {
    UNITY_BEGIN();
    RUN_TEST(test_dstr_init);
    RUN_TEST(test_dstr_append);
    UNITY_END();
    return 0;
}

__attribute__((constructor)) void register_dstr() {
    register_test(test_dstr);
}
