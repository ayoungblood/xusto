/*****************************************************************************\
 * xusto.c                                                                   *
 * Xusto interpreter, now with 0% more plus plus                             *
\*****************************************************************************/

#include "xusto.h"

extern flags_t flags;

int main(int argc, char **argv) {
    /* List of source files that will be set by the arguments wrapper */
    fp_list_t source_fp_list;
    source_fp_list.length = 0;
    /* Parse arguments and options, initializing the file list */
    int rv = arguments(argc, argv, &flags, &source_fp_list);
    if (rv != 0)
        return (rv == -1)?RETURN_OK:rv;
    int i;
    /* Loop through source file list and execute it */
    char buf[180];
    for (i = 0; i < source_fp_list.length; ++i) {
        printf("Parsing %s\n", source_fp_list.filenames[i]);
        while (fgets(buf, sizeof(buf), source_fp_list.files[i]) != NULL ) {

            // Read the instruction into memory
            printf("%s", buf);
        }
        if (source_fp_list.files[i]) {
            printf("Closing %s\n", source_fp_list.filenames[i]);
            fclose(source_fp_list.files[i]);
        } else {
            printf("Possible badness with %s, continuing...\n", source_fp_list.filenames[i]);
        }
    }
    fp_list_cleanup(&source_fp_list);
    return RETURN_OK;
}

int arguments(int argc, char **argv, flags_t *f, fp_list_t *fp_list) {
    /* Clear flags */
    *f = 0x0;
    /* Automatically configure colorized output based on CLICOLOR and TERM
       environment variables (CLICOLOR=1 or TERM=xterm-256color) */
    char* crv;
    if ((crv = getenv("CLICOLOR"))) { // CLICOLOR is set
        if (!strcmp(crv,"1")) {
            *f |= MASK_COLOR;
        } else {
            *f &= ~MASK_COLOR;
        }
    } else { // CLICOLOR not defined, check TERM
        if ((crv = getenv("TERM"))) { // TERM is set
            if (!strcmp(crv,"xterm-256color")) {
                *f |= MASK_COLOR;
            } else {
                *f &= ~MASK_COLOR;
            }
        } else { // CLICOLOR and TERM not defined, give up
            *f &= ~MASK_COLOR;
        }
    }
    /* Parse command line options with getopt */
    int i, c, option_index = 0;
    //opterr = 0; // disable getopt_long default errors
    while (1) {
        static struct option long_options[] = {
            /* Simulator options */
            {"color",           required_argument,  0, 'c'}, // (disabled,auto,force)
            {"debug",           no_argument,        0, 'd'}, // level
            {"help",            no_argument,        0, 'h'},
            {"version",         no_argument,        0, 'V'},
            {"verbose",         no_argument,        0, 'v'},
            {0, 0, 0, 0}
        };
        c = getopt_long (argc, argv, "c:dhVv",long_options, &option_index);
        if (c == -1) break; // Detect the end of the options.

        switch (c) {
            /* Simulator options */
            case 'c': // --color
                if (!strcmp(optarg,"disabled") || !strcmp(optarg,"d")) {
                    *f &= ~MASK_COLOR;
                } else if (!strcmp(optarg,"force") || !strcmp(optarg,"f")) {
                    *f |= MASK_COLOR;
                } else if (!strcmp(optarg,"auto") || !strcmp(optarg,"a")) {
                    // do nothing, already set above
                } else {
                    printf("Invalid color setting: %s\n", optarg);
                }
                break;
            case 'd': // --debug
                *f |= MASK_DEBUG;
                break;
            case 'h': // --help
                printf("halp\n");
                return -1; // caller should exit without errors
            case 'V': // --version
                printf("%s - Interpreter for the Xusto language (v. %s)\n",
                    TARGET_STRING,VERSION_STRING);
                return -1; // caller should exit ok
            case 'v': // --verbose
                *f |= MASK_VERBOSE;
                break;
            case '?': // error
                /* getopt_long already printed an error message. */
                break;
            default: // bad error
                printf("Failed to parse command line argument. Exiting.\n");
                return RETURN_BAD_OPTS;
        }
    }

    /* Iterate through any remaining command line arguments (not options). */
    if (optind < argc) {
        printf("Have %d files to read\n", argc-optind);
        fp_list->length = argc-optind;
        fp_list->files = (FILE**)malloc(sizeof(FILE*)*(size_t)(fp_list->length));
        fp_list->filenames = (char**)malloc(sizeof(char*)*(size_t)(fp_list->length));
        for (i = 0; i < fp_list->length; ++i) {
            fp_list->files[i] = fopen(argv[optind + i],"r");
            fp_list->filenames[i] = argv[optind + i];
            if (!fp_list->files[i]) {
                cprintf(ANSI_C_RED,"You lied to me when you told me this was a file: %s\n",
                    fp_list->filenames[i]);
                return RETURN_BAD_ARGS; // exit with error
            }
        }
    } else {
        cprintf(ANSI_C_RED,"Cannot interpret nothing! Exiting.\n");
        return RETURN_BAD_ARGS; // exit with error
    }
    return RETURN_OK; // everything is OK
}
/*
void parse(const char *filename) {
    FILE *fileptr;
    char *buffer;
    long filelen;

    fileptr = fopen(filename, "rb"); // Open the file in binary mode
    fseek(fileptr, 0, SEEK_END);     // Jump to the end of the file
    filelen = ftell(fileptr);        // Get the current byte offset in the file
    rewind(fileptr);                 // Jump back to the beginning of the file
    unsigned char b, b1, b2, b3;     // bytes from the file
    unsigned long v;                 // assembled value
    for (int i = 0; i < filelen; ++i) {
        fread(&b, 1, 1, fileptr);
        // stone-age UTF-8 parsing
        if (~b >> 7 & 1) { // 7-bit code point (first byte 0b0xxxxxxx)
            v = b;
        } else { // 11, 16, or 21-bit code point or continuation byte
            // printf(">>0x%0X\r\n", b);

            if ((b & 0xe0) == 0xc0) { // 11-bit code point (first byte 0b110xxxxx,
                // 1 byte follows)
                // printf("==0x%0X\r\n", b & 0xe0);
                fread(&b1, 1, 1, fileptr);
                v = (b & 0x1f) << 6 | (b1 & 0x3f);
            } else if (b >> 4 & 0x0f == 0x0e) { // 16-bit code point (first byte
                // 0b1110xxxx, 2 bytes follow)
                v = 0x0;
            } else if (b >> 3 & 0x1f == 0x1e) { // 21-bit code point (first byte
                // 0b11110xxx, 3 bytes follow)
                v = 0x0;
            } else { // we are desynchronized
                0xffffffff;
            }
        }
        printf("0x%0X\r\n", v);
    }

    // buffer = (char *)malloc((filelen + 1) *
    //                      sizeof(char)); // Enough memory for file + \0
    // fread(buffer, filelen, 1, fileptr);    // Read in the entire file

    fclose(fileptr); // Close the file
}
*/
/*
void message(const char *msg, int code, char *extra) {
    const char *type;
    const char *color;
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
    const char *ex = extra;
    if (!extra)
    ex = "";
    if (flags & STATE_F_DEBUG) {
        printf("%s%s 0x%04x: %s%s\n" ANSI_C_RESET, color, type, code, msg, ex);
    } else {
        printf("%s%s%s\n" ANSI_C_RESET, color, msg, ex);
    }
    return;
}
*/
