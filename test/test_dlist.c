#include "../src/include/dlist.h"
#include "Unity/src/unity.h"

void setUp() {};
void tearDown() {};

void test_dlist_init() {
    DList* dlist = dlist_init();
    TEST_ASSERT_EQUAL(0, dlist->ln);
    TEST_ASSERT_EQUAL(DLIST_INITSZ, dlist->sz);
}

void test_dlist_append() {
    DList* dlist;

    // Test simple appending.
    dlist = dlist_init();
    int* n = malloc(sizeof(int));
    *n = 1;
    dlist_append(dlist, n);

    TEST_ASSERT_EQUAL(n, dlist->buf[0]);
    TEST_ASSERT_EQUAL(1, dlist->ln);

    dlist_destroy(dlist);

    // Test buffer doubling.
    dlist = dlist_init();

    for (int i = 0; i < 129; i++) dlist_append(dlist, &i);

    TEST_ASSERT_EQUAL(129, dlist->ln);
    TEST_ASSERT_EQUAL(DLIST_INITSZ*2, dlist->sz);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_dlist_init);
    RUN_TEST(test_dlist_append);
    return UNITY_END();
}
