/*****************************************************************************\
 * xusto.h                                                                   *
 * Header file and general defines for the Xusto interpreter                 *
\*****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "util.h"

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

//void parse(const char *filename);
//void message(const char *msg, int code, char *extra);

// Message types
#define MSG_TYP_CATASTROPHIC "CATASTROPHIC"
#define MSG_TYP_ERROR "ERROR"
#define MSG_TYP_WARNING "Warning"
#define MSG_TYP_INFO "Info"
#define MSG_TYP_DEBUG "debug"
