/*****************************************************************************\
 * xusto.h                                                                   *
 * Header file and general defines for the Xusto interpreter                 *
\*****************************************************************************/

#include <cstdio>
#include <unistd.h>

#define LANG_EN
#include "i18n.h"

// Return codes
#define RETURN_OK 0x00
#define RETURN_BAD_ARGS 0x01

// Interpreter state flag bits
#define STATE_F_EXECUTE 0x01
#define STATE_F_PUSHCHAR 0x02
#define STATE_F_EXCEPTION 0x20
#define STATE_F_VERBOSE 0x40
#define STATE_F_DEBUG 0x80

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

void parse(const char *filename);
void message(const char *msg, int code, char *extra);

// ASCII Colors
#define ANSI_C_BLACK "\x1b[1;30m"
#define ANSI_C_RED "\x1b[1;31m"
#define ANSI_C_YELLOW "\x1b[1;33m"
#define ANSI_C_GREEN "\x1b[1;32m"
#define ANSI_C_CYAN "\x1b[1;36m"
#define ANSI_C_BLUE "\x1b[1;34m"
#define ANSI_C_MAGENTA "\x1b[1;35m"
#define ANSI_C_WHITE "\x1b[1;37m"
#define ANSI_C_RESET "\x1b[0m"

// Message types
#define MSG_TYP_CATASTROPHIC "CATASTROPHIC"
#define MSG_TYP_ERROR "ERROR"
#define MSG_TYP_WARNING "Warning"
#define MSG_TYP_INFO "Info"
#define MSG_TYP_DEBUG "debug"
