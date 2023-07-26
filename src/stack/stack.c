#include "stack.h"


Stack_t *stack_new() {
    Stack_t *stack = malloc(sizeof(Stack_t));
    stack->length = 0;
    stack->data = malloc(sizeof(void *) * stack->length);
    return stack;
}

void stack_push(Stack_t *stack, void *item) {
    stack->length++;
    stack->data = (void **) realloc(stack->data, sizeof(void *) * stack->length);
    stack->data[stack->length - 1] = item;
}

void *stack_pop(Stack_t *stack) {
    if (stack_is_empty(stack)) {
        return NULL;
    }
    void *top = stack_peek(stack);
    stack->length--;
    stack->data = (void **) realloc(stack->data, sizeof(void *) * stack->length);
    return top;
}

void *stack_peek(Stack_t *stack) {
    if (stack_is_empty(stack)) {
        return NULL;
    }
    return stack->data[stack->length - 1];
}

bool stack_is_empty(Stack_t *stack) {
    return stack->length == 0;
}

void stack_free(Stack_t *stack) {
    free(stack->data);
    free(stack);
}

void stack_purge(Stack_t *stack) {
    for (int i = 0; i < stack->length; i++) {
        free(stack->data[i]);
    }
    stack_free(stack);
}