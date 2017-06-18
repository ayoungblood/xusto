/* util.c
 * Utility/debug defines and other miscellany
 */

#include "util.h"

 /* With some C standards, variadic macros with zero args in the variadic part
    result in a very angry compiler. Therefore, we use this function instead */
void cprintf(const char *color, const char *format, ...) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
    va_list args;
    va_start(args, format);
    if (flags & MASK_COLOR) {
        eprintf("%s", color);
        vfprintf(stderr, format, args);
        eprintf(ANSI_RESET);
    } else {
        vprintf(format, args);
    }
    va_end(args);
#pragma GCC diagnostic pop
}
