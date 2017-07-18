/* xstack.h
 * LIFO stack of cells
 */

#ifndef _XSTACK_H
#define _XSTACK_H

#include "util.h"
#include "types.h"

typedef struct xstack_t {
    cell_t *start;
    cell_t *top;
    size_t max;
} xstack_t;
// xstack_t because stack_t conflicts with GNU stack

/* Create and initialize a new stack */
xstack_t *stack_create(size_t max);
/* Deallocate and destroy an existing stack */
void stack_destroy(xstack_t *stack);
/* Push a value onto the stack */
void stack_push(xstack_t *stack, cell_t value);
/* Pop a value from the stack */
cell_t stack_pop(xstack_t *stack);
/* Peek the top of the stack */
cell_t stack_peek(xstack_t *stack);
/* Get the size of the stack */
size_t stack_size(xstack_t *stack);
/* Print the top n stack entries */
void stack_print(xstack_t *stack, uint64_t n);

#endif /* _XSTACK_H */
