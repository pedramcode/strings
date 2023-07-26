#ifndef STACK_H
#define STACK_H

#include <malloc.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
    void **data;
    size_t length;
} Stack_t;

Stack_t *stack_new();

void stack_push(Stack_t *stack, void *item);

void *stack_pop(Stack_t *stack);

void *stack_peek(Stack_t *stack);

bool stack_is_empty(Stack_t *stack);


void stack_free(Stack_t *stack);

void stack_purge(Stack_t *stack);

#endif //STACK_H
