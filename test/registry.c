#include "registry.h"
#include "unity/unity_internals.h"

#define TESTS_MAX 128

static Test tests[TESTS_MAX];
static int test_count = 0;

void register_test(Test t) {
    if (test_count < TESTS_MAX) {
        tests[test_count] = t;
        test_count++;
    } else printf("Maximum number of tests (%d) exceeded.\n", TESTS_MAX);
}

void run_all_tests() {
    for (int i = 0; i < test_count; i++) {
        int res = tests[i]();
    }
}
