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

// Convert an integer value into a UTF-8 byte string and print to stdout
void print_utf8(xint_t c) {
    // Octal notation because UTF-8 uses six bit groups
    if (c < 0200) {
        // 00 - 0177 (0x0 - 0x7f): one byte
        printf("%c",(char)(c      & 0177));
    } else if (c < 04000) {
        // 0200 - 03777 (0x80 - 0x7ff): two bytes
        // 0x80 - 0x7ff: two bytes
        printf("%c",(char)((c>>6  & 037) | 0300));
        printf("%c",(char)((c     & 077) | 0200));
    } else if (c < 0xffff) {
        // 04000 - 0177777 (0x800 - 0xffff): three bytes
        printf("%c",(char)((c>>12 & 017) | 0340));
        printf("%c",(char)((c>>6  & 077) | 0200));
        printf("%c",(char)((c     & 077) | 0200));
    } else {
        // 0200000 - 04177777 (0x10000 - 0x10FFFF): four bytes
        printf("%c",(char)((c>>18 &  07) | 0360));
        printf("%c",(char)((c>>12 & 077) | 0200));
        printf("%c",(char)((c>>6  & 077) | 0200));
        printf("%c",(char)((c     & 077) | 0200));
    }
}
