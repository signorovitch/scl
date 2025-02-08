#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#define STACK_MAX 64

typedef struct {
    size_t ln;            // The length of the stack (i.e., how many elements).
    void* buf[STACK_MAX]; // The stack itself.
} Stack;

// Create a `Stack`.
Stack* stack_init();
// Destroy a `Stack`.
// Note that `->i` must be `0`, i.e. the `Stack` must be empty.
void stack_destroy(Stack* stack);

// Push a value to the `Stack`.
void stack_push(Stack* stack, void* val);
// Pop a value from the `Stack`.
void* stack_pop(Stack* stack);

#endif
