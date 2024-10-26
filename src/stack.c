#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "include/util.h"
#include "include/stack.h"

Stack* stack_init() {
    talloc(Stack, stack);

    memset(stack->val, 0, sizeof(void*) * STACK_MAX);
    stack->i = 0;

    return stack;
}

void stack_destroy(Stack* stack) {
    // Can only free an empty stack.
    assert(stack->i == 0);
    free(stack);
}

void stack_push(Stack* stack, void* val) {
    if (stack->i >= STACK_MAX) {
        log_dbgf("Ran out of stack (max: %d)", STACK_MAX);
        return;
    }

    stack->val[stack->i] = val;
    stack->i++;
}

void* stack_pop(Stack* stack) {
    if (stack->i <= 0) {
        log_dbg("Can't pop empty stack.");
        return (void*)-1;
    }

    return stack->val[--stack->i];
}
