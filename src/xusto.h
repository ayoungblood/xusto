/*****************************************************************************\
 * xusto.h                                                                   *
 * Header file and general defines for the Xusto interpreter                 *
\*****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <getopt.h>

#include "util.h"
#include "types.h"
#include "space.h"

#define LANG_EN
#include "i18n.h"

// Interpreter state flag bits
#define STATE_F_EXECUTE         (1<<0)
#define STATE_F_PUSHCHAR        (1<<1)
#define STATE_F_EXCEPTION       (1<<4)
#define STATE_F_VERBOSE         (1<<5)
#define STATE_F_DEBUG           (1<<7)
#define STATE_F_DEBUGLEVELSTART (8)
#define STATE_F_DEBUGLEVELMASK  (3)

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
