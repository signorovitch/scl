#ifndef REGISTRY_H
#define REGISTRY_H

#include <stdio.h>

// Test functions neither consume nor return anything.
typedef void (*Test)(void);

// Register a new test function.
void register_test(Test t);

// Run all registered tests.
void run_all_tests();

#endif
