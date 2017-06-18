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
#define STATE_F_EXCEPTION       (1<<2)

/* Execute from a space
 * @space: a space to execute
 * Returns -1 when the caller should return without errors,
 * returns >1 when the caller should return with errors,
 * returns 0 otherwise */
int execute(space_t *space);

/* Parse options and arguments
 * @argc: argc argument count from main()
 * @argv: argv argument vector from main()
 * @flags: flag pointer
 * @fp_list: list of file pointers into which source files are placed
 * Returns -1 when the caller should return without errors,
 * returns >1 when the caller should return with errors,
 * returns 0 otherwise */
int arguments(int argc, char **argv, flags_t *flags, fp_list_t *fp_list);

/* Parse source file into a space
 * @fp: file pointer to an open source file
 * @filepath: path of the open source file
 * @space: pointer to an existing space into which instructions are parsed
 * Returns >1 when the caller should return with errors,
 * return 0 otherwise */
int parse(FILE *fp, char *filepath, space_t *space);
