/*****************************************************************************\
 * xusto.c                                                                   *
 * Xusto interpreter, now with 0% more plus plus                             *
\*****************************************************************************/

#include "xusto.h"

extern flags_t flags;

int main(int argc, char **argv) {
    int i;
    /* List of source files that will be set by the arguments wrapper */
    fp_list_t source_fp_list;
    source_fp_list.length = 0;
    /* Parse arguments and options, initializing the file list */
    int rv = arguments(argc, argv, &flags, &source_fp_list);
    if (rv != 0) return (rv == -1)?RETURN_OK:rv;
    /* Loop through source file list and execute files */
    for (i = 0; i < source_fp_list.length; ++i) {
        printf("Parsing %s\n", source_fp_list.filenames[i]);
        rv = parse(source_fp_list.files[i], source_fp_list.filenames[i]);
        if (rv != 0) return rv;
        // Close the file. If the fp is NULL, something bad happened
        if (source_fp_list.files[i]) {
            printf("Closing %s\n", source_fp_list.filenames[i]);
            fclose(source_fp_list.files[i]);
        } else {
            printf("Possible badness with %s, continuing...\n",
                source_fp_list.filenames[i]);
        }
    }
    fp_list_cleanup(&source_fp_list);
    return RETURN_OK;
}

int parse(FILE *fp, char *filename) {
    long file_length = 0;
    char buffer[PARSE_BUFFER_SIZE + 1];
    size_t bytes_read = 0;
    buffer[PARSE_BUFFER_SIZE] = '\0';
    if (fp == NULL) eprintf("Error parsing %s\n", filename);
    /* Get the length of the file (in bytes) */
    fseek(fp, 0, SEEK_END);
    file_length = ftell(fp);
    rewind(fp);
    eprintf("%s is %ld bytes long\n", filename, file_length);
    /* Iterate through the file, byte-by-byte */
    unsigned char b = 0, b1 = 0, b2 = 0, b3 = 0;     // bytes from the file
    uint32_t v;                 // assembled value
    while (bytes_read < (unsigned long)file_length) {
        bytes_read += fread(&b, 1, 1, fp);
        printf("first byte: %02x (0%04o)\n", b, b);
        /* Stone-age UTF-8 parsing: check the first byte and read more as necessary */
        // If the first bit is clear, it's a single byte
        if (~b & 0x80) {
            printf("1-byte code point: %02x\n", b);
            // 7-bit code point (1 byte, U+0000 - U+007F)
            // Byte 0: 0b0xxxxxxx
            v = b;
        } else { // Otherwise, it's a multi-byte code point
            if ((b & 0340) == 0300) {
                // 11-bit code point (2 byte, U+0080 - U+07FF)
                // Byte 0: 0b110xxxxx
                // Byte 1: 0b10xxxxxx
                bytes_read += fread(&b1, 1, 1, fp);
                printf("2-byte code point: %02x, %02x\n", b, b1);
                v = ((uint32_t)b1 & 0077) | ((uint32_t)b & 0037)<<6;
            } else if ((b & 0360) == 0340) {
                // 16-bit code point (3 byte, U+0800 - U+FFFF)
                // Byte 0: 0b1110xxxx
                // Byte 1: 0b10xxxxxx
                // Byte 2: 0b10xxxxxx
                bytes_read += fread(&b1, 1, 1, fp);
                bytes_read += fread(&b2, 1, 1, fp);
                printf("3-byte code point: %02x, %02x, %02x\n", b, b1, b2);
                v = ((uint32_t)b2 & 0077) | ((uint32_t)b1 & 0077)<<6 | ((uint32_t)b & 0017)<<12;
            } else if ((b & 0370) == 0360) {
                // 21-bit code point (4 byte, U+10000 - U+10FFFF)
                // Byte 0: 0b11110xxx
                // Byte 1: 0b10xxxxxx
                // Byte 2: 0b10xxxxxx
                // Byte 3: 0b10xxxxxx
                bytes_read += fread(&b1, 1, 1, fp);
                bytes_read += fread(&b2, 1, 1, fp);
                bytes_read += fread(&b3, 1, 1, fp);
                printf("4-byte code point: %02x, %02x, %02x, %02x\n", b, b1, b2, b3);
                v = ((uint32_t)b3 & 0077) | ((uint32_t)b2 & 0077)<<6 | ((uint32_t)b1 & 0077)<<12 | ((uint32_t)b & 0007)<<18;
            } else {
                // We are desynchronized or the file is corrupted
                printf("Desynchronized at %zu bytes\n", bytes_read);
            }
        }
        printf("v: 0x%06x\n", v);
    }
    return 0;
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
                printf( "Usage: %s [option]... [file ...]\n" \
                        "Run %s on on source file(s). If no file is provided, \n" \
                        "read from " ANSI_BOLD "stdin" ANSI_RESET ".\n",
                        TARGET_STRING, TARGET_STRING);
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
                eprintf("Failed to parse command line argument. Exiting.\n");
                return RETURN_BAD_OPTS;
        }
    }

    /* Iterate through any remaining command line arguments (not options). */
    if (optind < argc) {
        // Get the number of (potential) files we have and allocate
        fp_list->length = argc-optind;
        fp_list->files = (FILE**)malloc(sizeof(FILE*)*(size_t)(fp_list->length));
        fp_list->filenames = (char**)malloc(sizeof(char*)*(size_t)(fp_list->length));
        // For each file, try to open it
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
        // We are reading from stdin
        fp_list->length = 1;
        fp_list->files = (FILE**)malloc(sizeof(FILE*)*(size_t)(fp_list->length));
        fp_list->filenames = (char**)malloc(sizeof(char*)*(size_t)(fp_list->length));
        fp_list->files[0] = stdin;
        fp_list->filenames[0] = strdup("stdin");
    }
    // If we reach here, we have succesfully opened all the files
    return RETURN_OK; // everything is OK
}

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
