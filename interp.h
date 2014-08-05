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

// Main return macros
#define MAIN_RV_ERR      0x100

// Types
typedef struct {
    // Interpreter state struct
    uint8_t  iptr[2]; // instruction pointer
    uint8_t  ivec[2]; // instruction vector
    uint8_t  bcon[2]; // beacon position
    uint8_t  warp[2]; // program space warp
    uint8_t  flags; // state flag bitfield
    uint8_t  pgm[256][256]; // program space
    uint16_t pgmsize[2]; // program space dimensions
    uint8_t* stack; // stack pointer, dynamic array
    uint64_t stacksize; // allocated stack height
} State;

// Functions
void execute(char* c, State* s);
void message(const char* msg, int code, char* extra);

