/* xstack.c
 * LIFO stack of cells
 */

#include "xstack.h"

/* Create and initialize a new stack */
xstack_t *stack_create(size_t size) {
    bprintf(2,"Creating stack of size %zu\n",size);
    xstack_t *stack;
    if ((stack = (xstack_t*)malloc(sizeof(xstack_t))) == NULL) {
        eprintf("Failed to create stack\n");
        return NULL;
    }
    if ((stack->start = (cell_t*)malloc(size*sizeof(cell_t))) == NULL) {
        eprintf("Failed to allocate memory for stack of size %zu\n", size);
        return NULL;
    }
    stack->top = stack->start;
    stack->max = size;
    (*stack->top).i = 0; // an "empty" stack still has one element, set to zero
    return stack;
}
/* Deallocate and destroy an existing stack */
void stack_destroy(xstack_t *stack) {
    if (!stack) {
        eprintf("Cannot destroy what doesn't exist\n");
        return;
    }
    bprintf(2,"Destroying stack of size %zu\n",stack_size(stack));
    free(stack->start);
    free(stack);
    stack = NULL;
}
/* Push a value onto the stack */
void stack_push(xstack_t *stack, cell_t value) {
    // Check the size, expand if necessary
    if (stack_size(stack) > (stack->max >> 1) + (stack->max >> 2)) {
        if ((stack->start = (cell_t*)realloc(stack->start,stack->max<<1)) == NULL) {
            eprintf("Failed to expand stack to size %zu, out of memory\n",stack->max<<1);
            return;
        }
    }
    *(++stack->top) = value;
    //*stack->top = value;
}
/* Pop a value from the stack */
cell_t stack_pop(xstack_t *stack) {
    cell_t cell;
    if (stack->start == stack->top) {
        // Peek if empty
        cell = *stack->top;
    } else {
        // Otherwise, pop
        cell = *(stack->top--);
    }
    return cell;
}
/* Peek the top of the stack */
cell_t stack_peek(xstack_t *stack) {
    return *stack->top;
}
/* Get the size of the stack */
size_t stack_size(xstack_t *stack) {
    return (size_t)(stack->top - stack->start);
}
/* Print the top n stack entries */
void stack_print(xstack_t *stack, uint64_t n) {
    const size_t size = stack_size(stack);
    size_t i = size-1;
    cell_t *cell = stack->top;
    while (cell != stack->start && i + n - size + 1 != 0) {
        printf("  0x%04zx: 0x%016"XIx" (%"XId")\n",i--,cell->i,cell->i);
        --cell;
    }
    return;
}
