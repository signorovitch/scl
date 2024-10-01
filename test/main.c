#include "unity/unity.h"
#include "registry.h"

void (*tests[30])();

void setUp() {}
void tearDown() {}

int main() {
    run_all_tests();
    return 0;
}
