#include "registry.h"
#include "unity/unity.h"

void (*tests[30])();

void setUp() {}
void tearDown() {}

int main() {
    run_all_tests();
    return 0;
}
