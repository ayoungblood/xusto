/*****************************************************************************\
 * xusto.h                                                                   *
 * Header file and general defines for the Xusto interpreter                 *
\*****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <getopt.h>
#include <string.h>

#include "util.h"

#define LANG_EN
#include "i18n.h"

// Interpreter state flag bits
#define STATE_F_EXECUTE 0x01
#define STATE_F_PUSHCHAR 0x02
#define STATE_F_EXCEPTION 0x20
#define STATE_F_VERBOSE 0x40
#define STATE_F_DEBUG 0x80

//void parse(const char *filename);
//void message(const char *msg, int code, char *extra);

// Parse options and arguments
// Returns -1 when the caller should return without errors,
// returns >1 when the caller should return with errors,
// returns 0 otherwise
int arguments(int argc, char **argv, flags_t *flags, fp_list_t *fp_list);

// Parse source file
// Returns >1 when the caller should return with errors,
// return 0 otherwise
int parse(FILE *fp, char *filename);

// Message types
#define MSG_TYP_CATASTROPHIC "CATASTROPHIC"
#define MSG_TYP_ERROR "ERROR"
#define MSG_TYP_WARNING "Warning"
#define MSG_TYP_INFO "Info"
#define MSG_TYP_DEBUG "debug"
