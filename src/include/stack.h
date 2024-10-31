#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#define STACK_MAX 64

typedef struct {
    size_t i;               // Current index in the stack.
    void* val[STACK_MAX];   // The stack itself.
} Stack;

Stack* stack_init();
// Destroy a stack.
// Note that `stack->i` must be `0`.
void stack_destroy(Stack* stack);

// Push a value to the stack.
void stack_push(Stack* stack, void* val);
// Pop a value from the stack.
void* stack_pop(Stack* stack);

#endif
