/*****************************************************************************\
 * xusto.cpp                                                                 *
 * Xusto interpreter, now with 100% more plus plus                           *
\*****************************************************************************/

#include "xusto.h"

long flags = 0;

int main(int argc, char *argv[]) {
  // Validate command line arguments --------------------
  int opt;
  opterr = 0;                                      // silence getopt complaints
  while ((opt = getopt(argc, argv, "dv")) != -1) { // iterate through options
    switch (opt) {
    case 'd': // -d: debug
      flags |= STATE_F_DEBUG;
      break;
    case 'v': // -v: verbose
      flags |= STATE_F_VERBOSE;
      break;
    case '?': // unknown option...
      // eprintf(MSG_NO_OPTION "%c\n", char(optopt));
      char option[] = {char(optopt), 0};
      message(MSG_WRN_BADOPTION, option);
      break;
    }
  }
  // done parsing options, check for remaining arguments
  if ((argc <= 1) || (argv[argc - 1] == NULL) || (optind == argc)) {
    message(MSG_ERR_NOARGS, 0);
    return RETURN_BAD_ARGS;
  }
  for (; optind < argc; ++optind) {
    printf("arg[%d]: %s\n", optind, argv[optind]);
  }
}

void message(const char *msg, int code, char *extra) {
  char *type;
  char *color;
  switch (code >> 8) {
  case 0: // SHOULD NOT HAPPEN
    color = ANSI_C_MAGENTA;
    type = MSG_TYP_CATASTROPHIC;
    break;
  case 1: // ERROR
    color = ANSI_C_RED;
    type = MSG_TYP_ERROR;
    break;
  case 2: // WARNING
    color = ANSI_C_YELLOW;
    type = MSG_TYP_WARNING;
    break;
  case 3: // INFO (not used)
    color = ANSI_C_GREEN;
    type = MSG_TYP_INFO;
    break;
  case 4: // DEBUG
    color = ANSI_C_GREEN;
    type = MSG_TYP_DEBUG;
    break;
  default: // should not be reached
    printf(ANSI_C_MAGENTA STR_BADERROR "\n" ANSI_C_RESET);
    return;
  }
  char *ex = extra;
  if (!extra)
    ex = "";
  if (flags & STATE_F_DEBUG) {
    printf("%s%s 0x%04x: %s%s\n" ANSI_C_RESET, color, type, code, msg, ex);
  } else {
    printf("%s%s%s\n" ANSI_C_RESET, color, msg, ex);
  }
  return;

  // printf("%s\n", msg);
}
