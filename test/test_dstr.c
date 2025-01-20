#include "../src/include/dstr.h"
#include "Unity/src/unity.h"
#include <string.h>

void setUp() {};
void tearDown() {};

void test_dstr_init() {
    Dstr* dstr = dstr_init();
    TEST_ASSERT_EQUAL(0, strlen(dstr->buf));
    TEST_ASSERT_EQUAL(0, dstr->ln);
    TEST_ASSERT_EQUAL(DSTR_INITSZ, dstr->sz);
}

void test_dstr_append() {
    Dstr* dstr;

    // Test simple appending.
    dstr = dstr_init();
    char* hello_world = "Hello, world!";
    dstr_append(dstr, hello_world, strlen(hello_world));

    TEST_ASSERT_EQUAL_STRING(hello_world, dstr->buf);
    TEST_ASSERT_EQUAL(strlen(hello_world), dstr->ln);
    TEST_ASSERT_EQUAL(DSTR_INITSZ, dstr->sz);

    dstr_destroy(dstr);

    // Test buffer doubling.
    dstr = dstr_init();

    char h[DSTR_INITSZ + 20];
    memset(h, 'h', DSTR_INITSZ + 19);
    h[DSTR_INITSZ + 19] = '\0';
    dstr_append(dstr, h, strlen(h));

    TEST_ASSERT_EQUAL_STRING(h, dstr->buf);
    TEST_ASSERT_EQUAL(strlen(h), dstr->ln);
    TEST_ASSERT_EQUAL(DSTR_INITSZ * 2, dstr->sz);
}

void test_dstr_appendch() {
    Dstr* dstr;

    // Test simple character appending.
    dstr = dstr_init();
    char c = 'c';
    char* c_str = "c";
    dstr_appendch(dstr, c);

    TEST_ASSERT_EQUAL_STRING(c_str, dstr->buf);
    TEST_ASSERT_EQUAL(strlen(c_str), dstr->ln);
    TEST_ASSERT_EQUAL(DSTR_INITSZ, dstr->sz);

    dstr_destroy(dstr);

    // Test buffer doubling.
    dstr = dstr_init();

    // Test against this string.
    char h[DSTR_INITSZ + 20];
    memset(h, 'h', DSTR_INITSZ + 19);
    h[DSTR_INITSZ + 19] = '\0';

    for (int i = 0; i < DSTR_INITSZ + 19; i++) dstr_appendch(dstr, 'h');

    TEST_ASSERT_EQUAL_STRING(h, dstr->buf);
    TEST_ASSERT_EQUAL(strlen(h), dstr->ln);
    TEST_ASSERT_EQUAL(DSTR_INITSZ * 2, dstr->sz);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_dstr_init);
    RUN_TEST(test_dstr_append);
    RUN_TEST(test_dstr_appendch);
    return UNITY_END();
}
