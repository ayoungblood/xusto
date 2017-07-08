/* state.h
 * The execution state type, represents a current execution state
 */

#ifndef _STATE_H
#define _STATE_H

#include "util.h"
#include "types.h"
#include "xstack.h"

/* Interpreter state type */
typedef struct state_t {
    // State flags
    state_flags_t flags;
    // Stack
    xstack_t *stack;
    // Instruction pointer and instruction vector
    vector3_t ip, iv;
    // Warp vector
    vector3_t warp;
    // Portal
    vector3_t portal;
} state_t;

#endif /* _STATE_H */
