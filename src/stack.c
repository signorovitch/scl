#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "include/stack.h"
#include "include/util.h"

Stack* stack_init() {
    talloc(Stack, stack);

    memset(stack->buf, 0, sizeof(void*) * STACK_MAX);
    stack->ln = 0;

    return stack;
}

void stack_destroy(Stack* stack) {
    // Can only free an empty stack.
    assert(stack->ln == 0);
    free(stack);
}

void stack_push(Stack* stack, void* val) {
    if (stack->ln >= STACK_MAX) {
        log_dbgf("Ran out of stack (max: %d)", STACK_MAX);
        return;
    }

    stack->buf[stack->ln] = val;
    stack->ln++;
    log_dbgf("pushed to stack, inc ln to %ld", stack->ln);
}

void* stack_pop(Stack* stack) {
    if (stack->ln <= 0) {
        log_dbg("Can't pop empty stack.");
        return (void*)-1;
    }

    return stack->buf[--stack->ln];
}
