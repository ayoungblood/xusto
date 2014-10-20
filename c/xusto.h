/* interp.h
 * Xusto interpreter header file
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

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

// Initial stack length at program start
#define STACKLEN_INITIAL  8
// Maximum length of the stack
#define STACKLEN_LIMIT    65536

// Sluggishness of execution
#define SLUGGISHNESS      37

// Interpreter state flag defines
#define STATE_F_EXECUTE   0x01
#define STATE_F_PUSHCHAR  0x02

#define STATE_F_EXCEPTION 0x20
#define STATE_F_VERBOSE   0x40
#define STATE_F_DEBUG     0x80

// Main return macros
#define MAIN_RV_CAREFREE  0x00
#define MAIN_RV_ERR       0x80
#define MAIN_RV_CRASH     0x81

// Convenience macros
#define PUSHVAL(v,s)    (*(++s->stack) = v)
#define POPVAL(s)       ((s->stack == s->stackbase)?0:*((s->stack)--))
#define PEEKVAL(s)      ((s->stack == s->stackbase)?0:*(s->stack))
#define CURRENTINSTR(s) (s->pgm[s->iptr[0]][s->iptr[1]])

// Types
typedef struct {
    // Interpreter state struct
    uint8_t  iptr[2]; // instruction pointer
    int8_t   ivec[2]; // instruction vector
    uint8_t  prtl[2]; // portal position
    int8_t   warp[2]; // program space warp
    uint8_t  flags; // state flag bitfield
    uint8_t  pgm[256][256]; // program space
    uint8_t  pgmsize[2]; // program space dimensions
    uint8_t* stack; // stack pointer, dynamic array
    uint8_t* stackbase; // bottom of stack
    uint16_t stackmax; // allocated length of stack
} State;

typedef struct {
    // Command line options
    uint8_t flags;
    uint8_t flagsMask;
} Options;

// Functions
void execute(State* s); // execute the current instruction
void update(State* s); // update the interpreter state
void message(const char* msg, int code, char* extra);
void processHeader(State* s, char* h); // ingest a given header line
void dumpStack(State* s); // print contents of stack
uint8_t random_u8(void); // generate a random value
uint8_t moonphase(void); // get the current phase of the moon, 0-2
