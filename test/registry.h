#ifndef REGISTRY_H
#define REGISTRY_H

#include <stdio.h>
#include "unity/unity.h"

typedef int (*Test)(void);

// Register a new test function.
void register_test(Test);

// Run all registered tests.
void run_all_tests();

#endif
