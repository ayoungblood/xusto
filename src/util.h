/* util.h
 * Utility/debug defines and other miscellany
 */

#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "types.h"

// Debugging and internal status flags
#define MASK_DEBUG          ((flags_t)1<<0) // Show debugging messages
#define MASK_VERBOSE        ((flags_t)1<<1) // Show verbose messages
#define MASK_COLOR          ((flags_t)1<<2) // Colorized text output

// Return codes (for main)
#define RETURN_OK           0x00
#define RETURN_BAD_ARGS     0x01
#define RETURN_BAD_OPTS     0x02

// ANSI escapes
#define ANSI_C_BLACK        "\x1b[1;30m"
#define ANSI_C_RED          "\x1b[1;31m"
#define ANSI_C_YELLOW       "\x1b[1;33m"
#define ANSI_C_GREEN        "\x1b[1;32m"
#define ANSI_C_CYAN         "\x1b[1;36m"
#define ANSI_C_BLUE         "\x1b[1;34m"
#define ANSI_C_MAGENTA      "\x1b[1;35m"
#define ANSI_C_WHITE        "\x1b[1;37m"
#define ANSI_RESET          "\x1b[0m"
#define ANSI_BOLD           "\x1b[1m"
#define ANSI_UNDER          "\x1b[4m"

/* Print macros
 * (note that dprintf conflicts with POSIX, and vprintf conflicts with ISO C) */
// Print to stderr
#define eprintf(...) fprintf(stderr,__VA_ARGS__)
// Print to stderr if debug set
#define gprintf(...) if (flags & MASK_DEBUG) eprintf(__VA_ARGS__)
// Print to stderr if verbose set
#define bprintf(...) if (flags & MASK_VERBOSE) eprintf(__VA_ARGS__)
// Colorized print to stderr if debug set
#define gcprintf(COLOR__,...) if (flags & MASK_DEBUG) cprintf(COLOR__,__VA_ARGS__)
// Colorized print to stderr if verbose set
#define bcprintf(COLOR__,...) if (flags & MASK_VERBOSE) cprintf(COLOR__,__VA_ARGS__)
/* Print wrappers, because macros can't do everything */
// Colorized (env-aware) printing to stderr
void cprintf(const char *color, const char *format, ...);

flags_t flags;

void fp_list_cleanup(fp_list_t *fp_list);

#endif /* _UTIL_H */
