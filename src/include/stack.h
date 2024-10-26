#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#define STACK_MAX 64

typedef struct {
    size_t i;
    void* val[STACK_MAX];
} Stack;

Stack* stack_init();
// stack->i must be 0.
void stack_destroy(Stack* stack);

void stack_push(Stack* stack, void* val);
void* stack_pop(Stack* stack);

#endif
