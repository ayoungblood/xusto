/*****************************************************************************\
 * xusto.c                                                                   *
 * Xusto interpreter, now with 0% more plus plus                             *
\*****************************************************************************/

#include "xusto.h"

extern flags_t flags;

int main(int argc, char **argv) {
    int i;
    space_t *space = NULL;
    /* List of source files that will be set by the arguments wrapper */
    fp_list_t source_fp_list;
    source_fp_list.length = 0;
    /* Parse arguments and options, initializing the file list */
    int rv = arguments(argc, argv, &flags, &source_fp_list);
    if (rv != 0) return (rv == -1)?RETURN_OK:rv;
    /* Loop through source file list and execute files */
    for (i = 0; i < source_fp_list.length; ++i) {
        // Create a space
        // @TODO space initialization values should probably be smarter
        space = space_create(vector3(0x100,0x100,0x4),1<<12);
        if (!space) {
            eprintf("Failed to create program space for %s\n", source_fp_list.filepaths[i]);
            return RETURN_INIT_FAIL;
        }
        // Parse the file
        bprintf(1,"Parsing %s\n", source_fp_list.filepaths[i]);
        rv = parse(source_fp_list.files[i], source_fp_list.filepaths[i], space);
        if (rv != 0) return rv;
        // Execute the file
        rv = execute(space);
        if (rv != 0) return rv;
        // Close the file
        bprintf(1,"Closing %s\n", source_fp_list.filepaths[i]);
        fclose(source_fp_list.files[i]);
        // Clean up
        space_destroy(space);
    }
    fp_list_cleanup(&source_fp_list);
    return RETURN_OK;
}

int execute(space_t *space) {
    // Instruction pointer and instruction vector
    vector3_t ip, iv;
    // Current instruction
    cell_t instr;
    // Initialize instruction vector and instruction pointer
    ip = vector3(0,0,0);
    iv = vector3(1,0,0);
    space_print(space,vector3(0,0,0),vector3(12,12,0), 0);
    // Start executing
    /*
    while (1) {
        // Get the current instruction
        instr = space_get(space, ip);
        // Execute the current instruction
        switch (instr.i) {

        }
        // Advance the instruction pointer
        ip = vector3_addv(ip, iv);
    }*/

    return 0;
}

int parse(FILE *fp, char *filepath, space_t *space) {
    long file_length = 0;
    size_t bytes_read = 0, rv = 0;
    unsigned char b = 0, b1 = 0, b2 = 0, b3 = 0; // bytes from the file
    uint32_t v; // assembled value
    // Cell and write pointer for writing to space
    cell_t cell;
    vector3_t wp = {0,0,0};
    /* Sanity check the file pointer */
    if (fp == NULL) eprintf("Error parsing %s\n", filepath);
    /* Get the length of the file (in bytes) */
    fseek(fp, 0, SEEK_END);
    file_length = ftell(fp);
    rewind(fp);
    bprintf(2,"%s is %ld bytes long\n", filepath, file_length);
    /* Iterate through the file, byte-by-byte */
    while (bytes_read < (unsigned long)file_length) {
        // Check that we can actually read bytes, so if we are reading from
        // stdin, we will exit the loop when we can't read any more bytes
        // This is not needed on the wider codepoints, because if an incomplete
        // codepoint is encountered, we will at most read three bad bytes
        rv = fread(&b, 1, 1, fp);
        if (rv != 1) break;
        bytes_read += rv;
        /* Stone-age UTF-8 parsing: check the first byte and read more as necessary */
        if (~b & 0x80) { // If the MSB is clear, single byte codepoint
            // 7-bit code point (1 byte, U+0000 - U+007F)
            // Byte 0: 0b0xxxxxxx
            v = b & 0177;
        } else { // Otherwise, it's a multi-byte codepoint
            if ((b & 0340) == 0300) {
                // 11-bit code point (2 byte, U+0080 - U+07FF)
                // Byte 0: 0b110xxxxx
                // Byte 1: 0b10xxxxxx
                bytes_read += fread(&b1, 1, 1, fp);
                v = ((uint32_t)b1 & 0077) | ((uint32_t)b & 0037)<<6;
            } else if ((b & 0360) == 0340) {
                // 16-bit code point (3 byte, U+0800 - U+FFFF)
                // Byte 0: 0b1110xxxx
                // Byte 1: 0b10xxxxxx
                // Byte 2: 0b10xxxxxx
                bytes_read += fread(&b1, 1, 1, fp);
                bytes_read += fread(&b2, 1, 1, fp);
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
                v = ((uint32_t)b3 & 0077) | ((uint32_t)b2 & 0077)<<6 | ((uint32_t)b1 & 0077)<<12 | ((uint32_t)b & 0007)<<18;
            } else {
                // An invalid UTF-8 byte.
                // Either we are horribly desynchronized or the file is corrupted
                // We intentionally expose a UTF-8 parser vulnerability and
                // take the byte verbatim, but also print a warning
                cprintf(ANSI_C_YELLOW, "Warning: Possible corruption in %s@b%zu: 0x%02x\n",
                    filepath, bytes_read, b);
                v = b;
            }
        }
        if (v <= 0x1f) { // character is a parser instruction, execute it
            //printf("parser instruction: 0x%02x\n", v);
            switch (v) {
                case 0x00: // NUL (^@) Null
                    // do absolutely nothing
                    break;
                case 0x03: // ETX (^C) End of Text
                    // should probably do something with this
                    break;
                case 0x04: // EOT (^D) End of Transmission
                    // should probably do something with this
                    break;
                case 0x07: // BEL (^G) Bell
                    // Whack the parser, causing it to say "OUCH"
                    if (flags & MASK_VERBOSE) {
                        cprintf(ANSI_C_YELLOW, "OUCH (%s@b%zu)\n", filepath, bytes_read);
                    } else {
                        cprintf(ANSI_C_YELLOW, "OUCH\n");
                    }
                    break;
                case 0x08: // BS (^H) Backspace
                    // move left by 1
                    wp.x -= 1;
                    break;
                case 0x09: // HT (^I) Horizontal Tabulation
                    // move right by 4
                    wp.x += 4;
                    break;
                case 0x0a: // LF (^J) Line Feed
                    // reset left/right and move down by 1 (*nix line endings)
                    wp.x = 0;
                    wp.y += 1;
                    break;
                case 0x0b: // VT (^K) Vertical Tabulation
                    // move down by 1
                    wp.y += 1;
                    break;
                case 0x0c: // FF (^L) Form Feed
                    // move into by 1
                    wp.z += 1;
                    break;
                case 0x0d: // CR (^M) Carriage Return
                    // reset left/right
                    wp.x = 0;
                    break;
                case 0x18: // CAN (^X) Cancel
                    // @TODO parser exit and dump
                    break;
                case 0x19: // EM (^Y) End of medium
                    // @TODO parser stop
                    break;
                default: // Reserved/unimplemented/non-existent parser instruction
                    cprintf(ANSI_C_RED, "Reserved parser instruction in %s@b%zu: 0x%02x. Exiting.\n",
                        filepath, bytes_read, v);
                    return 1; // @TODO FIXME
            }
        } else { // character is a regular instruction, store it and advance the parser pointer
            cell.i = v;
            space_set(space, wp, cell);
            ++wp.x;
        }
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
            {"debug",           no_argument,        0, 'd'},
            {"help",            no_argument,        0, 'h'},
            {"version",         no_argument,        0, 'V'},
            {"verbose",         optional_argument,  0, 'v'}, // level
            {0, 0, 0, 0}
        };
        c = getopt_long (argc, argv, "c:dhVv::",long_options, &option_index);
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
                printf( "Usage: %s [option]... [file ...]\n"
                        "Run %s on source file(s). If no file is provided, \n"
                        "read from " ANSI_BOLD "stdin" ANSI_RESET ".\n\n"
                        "Options:\n"
                        "    "ANSI_BOLD"--color "ANSI_RESET ANSI_UNDER"mode"ANSI_RESET ANSI_BOLD", -c "ANSI_RESET ANSI_UNDER"mode"ANSI_RESET ANSI_BOLD"\n"
                        "       Colorized output behavior. "ANSI_UNDER"mode"ANSI_RESET" may be "ANSI_BOLD"disable"ANSI_RESET", which disables\n"
                        "       colorized output; "ANSI_BOLD"force"ANSI_RESET", which colorizes the output; or "ANSI_BOLD"auto"ANSI_RESET",\n"
                        "       the default, which attempts to automatically detect whether to colorize\n"
                        "       based on environment variables.\n"
                        "    "ANSI_BOLD"--debug, -d"ANSI_RESET"\n"
                        "       Enables debugging output. Disabled by default.\n" \
                        "    "ANSI_BOLD"--help, -h"ANSI_RESET"\n"
                        "       Prints this usage information and exits.\n"
                        "    "ANSI_BOLD"--version, -V"ANSI_RESET"\n"
                        "       Prints simulator version information.\n"
                        "    "ANSI_BOLD"--verbose, -v"ANSI_RESET"\n"
                        "       Enable verbose output. Disabled by default.\n"
                        "\nFor options with string arguments, arguments may be shortened\n"
                        "as long as the arguments are not ambiguous.\n",
                        TARGET_STRING, TARGET_STRING);
                return -1; // caller should exit without errors
            case 'V': // --version
                printf("%s - Interpreter for the Xusto language (v. %s)\n",
                    TARGET_STRING,VERSION_STRING);
                return -1; // caller should exit without errors
            case 'v': // --verbose
                if (optarg != NULL) {
                    if (!strcmp(optarg,"v") || !strcmp(optarg,"2")) {
                        *f |= (2<<SHIFT_VERBOSE);
                    } else if (!strcmp(optarg,"vv") || !strcmp(optarg,"3")) {
                        *f |= (3<<SHIFT_VERBOSE);
                    } else {
                        eprintf("Invalid verbosity level: %s\n", optarg);
                    }
                } else {
                    *f |= (1<<SHIFT_VERBOSE);
                }
                break;
            case '?': // error
                /* getopt_long already printed an error message. */
                break;
            default: // bad error
                eprintf("Failed to parse command line argument. Exiting.\n");
                return RETURN_BAD_OPTS;
        }
    }
    bprintf(2,"flags = 0x%08x\n", flags);

    /* Iterate through any remaining command line arguments (not options). */
    if (optind < argc) {
        // Get the number of (potential) files we have and allocate
        fp_list->length = argc-optind;
        fp_list->files = (FILE**)malloc(sizeof(FILE*)*(size_t)(fp_list->length));
        fp_list->filepaths = (char**)malloc(sizeof(char*)*(size_t)(fp_list->length));
        // For each file, try to open it
        for (i = 0; i < fp_list->length; ++i) {
            fp_list->files[i] = fopen(argv[optind + i],"r");
            fp_list->filepaths[i] = argv[optind + i];
            if (!fp_list->files[i]) {
                cprintf(ANSI_C_RED,"You lied to me when you told me this was a file: %s\n",
                    fp_list->filepaths[i]);
                return RETURN_BAD_ARGS; // exit with error
            }
        }
    } else {
        // We are reading from stdin
        fp_list->length = 1;
        fp_list->files = (FILE**)malloc(sizeof(FILE*)*(size_t)(fp_list->length));
        fp_list->filepaths = (char**)malloc(sizeof(char*)*(size_t)(fp_list->length));
        fp_list->files[0] = stdin;
        fp_list->filepaths[0] = malloc(6*sizeof(char));
        strncpy(fp_list->filepaths[0], "stdin", 6);
    }
    // If we reach here, we have succesfully opened all the files
    return RETURN_OK; // everything is OK
}
