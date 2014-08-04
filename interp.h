/* interp.h
 * lang2d interpreter header
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
// #include <unistd.h>
#include <stdlib.h>

#include "i18n/lang-en.h"

// ASCII Colors
#define ANSI_C_BLACK   "\x1b[1;30m"
#define ANSI_C_RED     "\x1b[1;31m"
#define ANSI_C_YELLOW  "\x1b[1;33m"
#define ANSI_C_GREEN   "\x1b[1;32m"
#define ANSI_C_CYAN    "\x1b[1;36m"
#define ANSI_C_BLUE    "\x1b[1;34m"
#define ANSI_C_MAGENTA "\x1b[1;35m"
#define ANSI_C_WHITE   "\x1b[1;37m"
#define ANSI_C_RESET   "\x1b[0m"

// The maximum length of the stack
#define LIMIT_STACKLEN   16384

// Interpreter state flag defines
#define STATE_F_EXECUTE  0x01
#define STATE_F_PUSHCHAR 0x02

#define STATE_F_DEBUG    0x80

// Functions
void message(const char* msg, int code, char* extra) {
    char* type;
    switch (code >> 8) {
        case 0: // SHOULD NOT HAPPEN
            type = ANSI_C_RED MSG_TYP_CATASTROPHIC;
            break;
        case 1: // ERROR
            type = ANSI_C_RED MSG_TYP_ERROR;
            break;
        case 2: // WARNING
            type = ANSI_C_YELLOW MSG_TYP_WARNING;
            break;
        case 3: // INFO
            type = ANSI_C_GREEN MSG_TYP_INFO;
            break;
        case 4: // DEBUG
            type = ANSI_C_GREEN MSG_TYP_DEBUG;
            break;
        default: // should not be reached
            printf(ANSI_C_RED MSTR_BADERROR ANSI_C_RESET);
            return;
    }
    char* ex = extra;
    if (!extra) ex = "";
    printf("%s 0x%04x: %s %s\n" ANSI_C_RESET, type, code, msg, ex);
    return;
}

