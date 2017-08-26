/*****************************************************************************\
 * xusto.c                                                                   *
 * Xusto interpreter, now with 0% more plus plus                             *
\*****************************************************************************/

#include "xusto.h"

extern options_t options;

int main(int argc, char **argv) {
    int i;
    // A program needs a space and a state to run
    space_t *space = NULL;
    state_t state;
    // List of source files that will be set by the arguments() wrapper
    fp_list_t source_fp_list;
    source_fp_list.length = 0;
    // Parse arguments and options, initializing the file list and setting options
    int rv = arguments(argc, argv, &options, &source_fp_list);
    if (rv != RETURN_OK) return (rv == RETURN_OK_THRU)?RETURN_OK:rv;
    // Loop through source file list and execute each file
    for (i = 0; i < source_fp_list.length; ++i) {
        // Create a space
        // @TODO space initialization values should probably be smarter
        space = space_create(vector3(0x100,0x100,0x4),1<<12);
        if (!space) {
            eprintf("Failed to create program space for %s\n", source_fp_list.filepaths[i]);
            return RETURN_INIT_FAIL;
        }
        // Initialize state
        if ((state.stack = stack_create(1<<8)) == NULL) return RETURN_INIT_FAIL;
        state.ip        = vector3(0,0,0);
        state.iv        = vector3(1,0,0);
        state.warp      = vector3(0,0,0);
        state.portal    = vector3(0,0,0);
        // Parse the file
        bprintf(1,"Parsing %s\n", source_fp_list.filepaths[i]);
        rv = parse(source_fp_list.files[i], source_fp_list.filepaths[i], space);
        if (rv != RETURN_OK) return (rv == RETURN_OK_THRU)?RETURN_OK:rv;
        // Execute the file
        state.flags |= STATE_F_EXECUTE;
        rv = execute(space, state);
        if (rv != RETURN_OK) return (rv == RETURN_OK_THRU)?RETURN_OK:rv;
        // Close the file
        bprintf(1,"Closing %s\n", source_fp_list.filepaths[i]);
        fclose(source_fp_list.files[i]);
        // Clean up
        space_destroy(space);
        stack_destroy(state.stack);
    }
    fp_list_cleanup(&source_fp_list);
    return RETURN_OK;
}

int execute(space_t *space, state_t state) {
    int rv;
    // Temporary vector
    vector3_t tempv;
    // Current instruction
    cell_t instr;
    // Temporary cells for stack operations
    cell_t a, b, c, x, y, z;

    //space_print(space,vector3(0,0,0),vector3(15,7,0), 0);
    // Start executing
    while (state.flags & STATE_F_EXECUTE) {
        // Get the current instruction
        instr = space_get(space, state.ip);
        // If PUSHCHAR set, push the instruction onto the stack (do not execute)
        // and advance. The only character that exits PUSHCHAR is "
        if (state.flags & STATE_F_PUSHCHAR && instr.i != 0x0022) {
            stack_push(state.stack,cell(instr.i));
            state.ip = vector3_addv(state.ip, state.iv);
            goto LOOP_END;
        }
        // Execute the current instruction
        switch (instr.i) {
        case 0x0000: // NULL: no-op
        case 0x0020: // SPACE: no-op
            break;
        case 0x0021: // !: logical NOT
            a = stack_pop(state.stack);
            if (a.i) stack_push(state.stack,cell(0)); else stack_push(state.stack,cell(1));
            break;
        case 0x0022: // ": PUSHCHAR
            state.flags ^= STATE_F_PUSHCHAR;
            break;
        case 0x0023: // #: drop portal
            state.portal = state.ip;
            break;

        case 0x0025: // %: modulo
            a = stack_pop(state.stack);
            b = stack_pop(state.stack);
            stack_push(state.stack,cell(b.i % a.i));
            break;
        case 0x0026: // &: bitwise AND
            a = stack_pop(state.stack);
            b = stack_pop(state.stack);
            stack_push(state.stack,cell(a.i & b.i));
            break;
        case 0x0027: // ': lazy print
            do {
                a = stack_pop(state.stack);
                print_utf8(a.i);
            } while (a.i != 0);
            break;

        case 0x002A: // *: multiply
            a = stack_pop(state.stack);
            b = stack_pop(state.stack);
            stack_push(state.stack,cell(b.i * a.i));
            break;
        case 0x002B: // +: add
            a = stack_pop(state.stack);
            b = stack_pop(state.stack);
            stack_push(state.stack,cell(b.i + a.i));
            break;

        case 0x002D: // -: subtract
            a = stack_pop(state.stack);
            b = stack_pop(state.stack);
            stack_push(state.stack,cell(b.i - a.i));
            break;
        case 0x002E: // .: out
            state.iv = vector3(0,0,-1);
            break;
        case 0x002F: // /: divide
            a = stack_pop(state.stack);
            b = stack_pop(state.stack);
            stack_push(state.stack,cell(b.i / a.i));
            break;
        case 0x0030: // 0: push 0x00
            stack_push(state.stack,cell(0x00));
            break;
        case 0x0031: // 1: push 0x01
            stack_push(state.stack,cell(0x01));
            break;
        case 0x0032: // 2: push 0x02
            stack_push(state.stack,cell(0x02));
            break;
        case 0x0033: // 3: push 0x03
            stack_push(state.stack,cell(0x03));
            break;
        case 0x0034: // 4: push 0x04
            stack_push(state.stack,cell(0x04));
            break;
        case 0x0035: // 5: push 0x05
            stack_push(state.stack,cell(0x05));
            break;
        case 0x0036: // 6: push 0x06
            stack_push(state.stack,cell(0x06));
            break;
        case 0x0037: // 7: push 0x07
            stack_push(state.stack,cell(0x07));
            break;
        case 0x0038: // 8: push 0x08
            stack_push(state.stack,cell(0x08));
            break;
        case 0x0039: // 9: push 0x09
            stack_push(state.stack,cell(0x09));
            break;
        case 0x003A: // :: duplicate
            a = stack_peek(state.stack);
            stack_push(state.stack,a);
            break;

        case 0x003C: // <: left
            state.iv = vector3(-1,0,0);
            break;
        case 0x003D: // =: equality
            a = stack_pop(state.stack);
            b = stack_pop(state.stack);
            if (a.i == b.i)
                stack_push(state.stack,cell(0x1));
            else
                stack_push(state.stack,cell(0x0));
            break;
        case 0x003E: // >: right
            state.iv = vector3(1,0,0);
            break;

        case 0x0040: // @: use portal
            state.ip = state.portal;
            break;

        case 0x0042: // B: bounce
            state.iv = vector3(-state.iv.x,-state.iv.y,-state.iv.z);
            break;

        case 0x0046: // F: flip
            a = stack_pop(state.stack);
            b = stack_pop(state.stack);
            c = stack_pop(state.stack);
            stack_push(state.stack,a);
            stack_push(state.stack,b);
            stack_push(state.stack,c);
            break;
        case 0x0047: // G: get
            x = stack_pop(state.stack);
            y = stack_pop(state.stack);
            z = stack_pop(state.stack);
            stack_push(state.stack,space_get(space,vector3(x.i,y.i,z.i)));
            break;
        case 0x0048: // H: halt
            // clear execute bit
            state.flags &= ~STATE_F_EXECUTE;
            bprintf(1,"Halted execution at (%"XId",%"XId",%"XId")\n",
                state.ip.x,state.ip.y,state.ip.z);
            break;

        case 0x004A: // J: jump
            x = stack_pop(state.stack);
            y = stack_pop(state.stack);
            z = stack_pop(state.stack);
            state.ip = vector3(x.i,y.i,z.i);
            break;

        case 0x0052: // R: jump subroutine
            x = stack_pop(state.stack);
            y = stack_pop(state.stack);
            z = stack_pop(state.stack);
            stack_push(state.stack,cell(state.ip.z));
            stack_push(state.stack,cell(state.ip.y));
            stack_push(state.stack,cell(state.ip.x));
            state.ip = vector3(x.i,y.i,z.i);
            break;
        case 0x0053: // S: set
            x = stack_pop(state.stack);
            y = stack_pop(state.stack);
            z = stack_pop(state.stack);
            a = stack_pop(state.stack);
            space_set(space,vector3(x.i,y.i,z.i),a);
            break;

        case 0x0056: // V: set vector
            x = stack_pop(state.stack);
            y = stack_pop(state.stack);
            z = stack_pop(state.stack);
            state.iv = vector3(x.i,y.i,z.i);
            break;
        case 0x0057: // W: set warp
            x = stack_pop(state.stack);
            y = stack_pop(state.stack);
            z = stack_pop(state.stack);
            state.warp = vector3(x.i,y.i,z.i);
            break;

        case 0x005B: // [: print integer
            a = stack_pop(state.stack);
            printf("%"XId"",a.i);
            break;
        case 0x005C: // \: swap
            a = stack_pop(state.stack);
            b = stack_pop(state.stack);
            stack_push(state.stack,a);
            stack_push(state.stack,b);
            break;
        case 0x005D: // ]: print character
            a = stack_pop(state.stack);
            print_utf8(a.i);
            break;
        case 0x005E: // ^: up
            state.iv = vector3(0,-1,0);
            break;
        case 0x005F: // _: trampoline
            state.ip = vector3_addv(state.ip, state.iv);
            break;

        case 0x0061: // a: push 0x0a
            stack_push(state.stack,cell(0x0a));
            break;
        case 0x0062: // b: push 0x0b
            stack_push(state.stack,cell(0x0b));
            break;
        case 0x0063: // c: push 0x0c
            stack_push(state.stack,cell(0x0c));
            break;
        case 0x0064: // d: push 0x0d
            stack_push(state.stack,cell(0x0d));
            break;
        case 0x0065: // e: push 0x0e
            stack_push(state.stack,cell(0x0e));
            break;
        case 0x0066: // f: push 0x0f
            stack_push(state.stack,cell(0x0f));
            break;

        case 0x0069: // i: if, up/down
            a = stack_pop(state.stack);
            if (a.i)
                state.iv = vector3(0,0,1);
            else
                state.iv = vector3(0,0,-1);
            break;
        case 0x006A: // j: bitwise XOR
            a = stack_pop(state.stack);
            b = stack_pop(state.stack);
            stack_push(state.stack,cell(a.i ^ b.i));
            break;

        case 0x006C: // l: if, left/right
            a = stack_pop(state.stack);
            if (a.i)
                state.iv = vector3(1,0,0);
            else
                state.iv = vector3(-1,0,0);
            break;

        case 0x006F: // o: into
            state.iv = vector3(0,0,1);
            break;
        case 0x0070: // p: pop
            (void)stack_pop(state.stack);
            break;


        case 0x0075: // u: if, up/down
            a = stack_pop(state.stack);
            if (a.i)
                state.iv = vector3(0,1,0);
            else
                state.iv = vector3(0,-1,0);
            break;
        case 0x0076: // v: down
            state.iv = vector3(0,1,0);
            break;

        case 0x0078: // x: conditional XY rotate
            tempv = state.iv;
            a = stack_pop(state.stack);
            if (a.i) {
                state.iv.y = tempv.x;
                state.iv.x = -tempv.y;
            } else {
                state.iv.y = -tempv.x;
                state.iv.x = tempv.y;
            }
            break;

        case 0x007B: // {: peek print integer
            a = stack_peek(state.stack);
            printf("%"XId"",a.i);
            break;
        case 0x007C: // |: bitwise OR
            a = stack_pop(state.stack);
            b = stack_pop(state.stack);
            stack_push(state.stack,cell(a.i | b.i));
            break;
        case 0x007D: // }: peek print char
            a = stack_peek(state.stack);
            print_utf8(a.i);
            break;
        case 0x007E: // ~: bitwise NOT
            a = stack_peek(state.stack);
            stack_push(state.stack,cell(~a.i));
            break;
        ///////////////////////////////////////////////////
        case 0x0058: // @TODO temporary debug dump
            stack_print(state.stack,8);
            break;
        default:
            if (VERBOSITY)
                eprintf("Unimplemented instruction: %#"XIx" at %"XId",%"XId",%"XId"\n",
                    instr.i,state.ip.x,state.ip.y,state.ip.z);
            else
                eprintf("Unimplemented instruction: %#"XIx"\n",instr.i);
            return 0;
        }
        state.ip = vector3_addv(state.ip, state.iv);
LOOP_END: // instructions may jump here to skip the usual instruction pointer advancement
        if (INTERACTIVE) if ((rv = interactive()) != RETURN_OK) return (rv == RETURN_OK_THRU)?RETURN_OK:rv;
    }
    // Flush stdout @TODO should we disable buffering on stdout instead?
    fflush(stdout);
    return 0;
}

int parse(FILE *fp, char *filepath, space_t *space) {
    size_t file_length = 0, bytes_read = 0, rv = 0;
    unsigned char b = 0, b1 = 0, b2 = 0, b3 = 0; // bytes from the file
    uint32_t v; // assembled value
    // Cell and write pointer for writing to space
    cell_t cell;
    vector3_t wp = {0,0,0};
    /* Sanity check the file pointer */
    if (fp == NULL) eprintf("Error parsing %s\n", filepath);
    /* Get the length of the file (in bytes) */
    fseek(fp, 0, SEEK_END);
    file_length = (size_t)ftell(fp);
    rewind(fp);
    bprintf(2,"%s is %zu bytes long\n", filepath, file_length);
    /* Iterate through the file, byte-by-byte */
    while (bytes_read < file_length) {
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
                case 0x01: // SOH (^@) Start of Heading
                    // Reset the write pointer left/right and upwards (±x, -y)
                    wp.x = 0;
                    wp.y = 0;
                    break;
                case 0x02: // STX (^B) Start of Text
                    // @TODO Set the initial instruction pointer from the write pointer
                    break;
                case 0x03: // ETX (^C) End of Text
                    // Stop parsing and exit loudly
                    if (VERBOSITY) {
                        cprintf(ANSI_C_RED, "Received ^D. Parsing halted (%s@b%zu)\n", filepath, bytes_read);
                    } else {
                        cprintf(ANSI_C_RED, "Received ^D. Parsing halted.\n");
                    }
                    return RETURN_HALTED;
                    break; // never reached
                case 0x04: // EOT (^D) End of Transmission
                    // Stop parsing and exit quietly
                    if (VERBOSITY) {
                        cprintf(ANSI_C_GREEN, "Received ^D. Exiting parser.\n");
                    }
                    return RETURN_OK_THRU;
                    break; // never reached

                case 0x07: // BEL (^G) Bell
                    // Whack the parser, causing it to say "OUCH"
                    if (VERBOSITY) {
                        cprintf(ANSI_C_YELLOW, "OUCH (%s@b%zu)\n", filepath, bytes_read);
                    } else {
                        cprintf(ANSI_C_YELLOW, "OUCH\n");
                    }
                    break;
                case 0x08: // BS (^H) Backspace
                    // Move write pointer one position leftwards (-x)
                    wp.x -= 1;
                    break;
                case 0x09: // HT (^I) Horizontal Tabulation
                    // Move write pointer four positions rightwards (+x)
                    wp.x += 4;
                    break;
                case 0x0a: // LF (^J) Line Feed
                    // Move write pointer one position downwards and reset left/right (+y, ±x) (*nix line endings)
                    wp.x = 0;
                    wp.y += 1;
                    break;
                case 0x0b: // VT (^K) Vertical Tabulation
                    // Move write pointer one position downwards (+y)
                    wp.y += 1;
                    break;
                case 0x0c: // FF (^L) Form Feed
                    // Move write pointer one position into (+z)
                    wp.z += 1;
                    break;
                case 0x0d: // CR (^M) Carriage Return
                    // Reset the write pointer left/right (±x)
                    wp.x = 0;
                    break;

                case 0x18: // CAN (^X) Cancel
                    // Stop parsing and dump (do not execute)
                    // @TODO dump the space
                    return RETURN_HALTED;
                    break;
                case 0x19: // EM (^Y) End of medium
                    // Stop parsing and execute (EOF)
                    return RETURN_OK;
                    break;

                default: // Reserved/unimplemented/non-existent parser instruction
                    cprintf(ANSI_C_RED, "Reserved parser instruction in %s@b%zu: 0x%02x. Exiting.\n",
                        filepath, bytes_read, v);
                    return RETURN_BAD_INST;
            }
        } else { // character is a regular instruction, store it and advance the parser pointer
            cell.i = v;
            space_set(space, wp, cell);
            ++wp.x;
        }
    }
    return RETURN_OK;
}

int arguments(int argc, char **argv, options_t *o, fp_list_t *fp_list) {
    /* Clear options */
    *o = 0x0;
    /* Automatically configure colorized output based on CLICOLOR and TERM
       environment variables (CLICOLOR=1 or TERM=xterm-256color) */
    char* crv;
    if ((crv = getenv("CLICOLOR"))) { // CLICOLOR is set
        if (!strcmp(crv,"1")) {
            *o |= MASK_COLOR;
        } else {
            *o &= ~MASK_COLOR;
        }
    } else { // CLICOLOR not defined, check TERM
        if ((crv = getenv("TERM"))) { // TERM is set
            if (!strcmp(crv,"xterm-256color")) {
                *o |= MASK_COLOR;
            } else {
                *o &= ~MASK_COLOR;
            }
        } else { // CLICOLOR and TERM not defined, give up
            *o &= ~MASK_COLOR;
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
                    *o &= ~MASK_COLOR;
                } else if (!strcmp(optarg,"force") || !strcmp(optarg,"f")) {
                    *o |= MASK_COLOR;
                } else if (!strcmp(optarg,"auto") || !strcmp(optarg,"a")) {
                    // do nothing, already set above
                } else {
                    printf("Invalid color setting: %s\n", optarg);
                }
                break;
            case 'd': // --debug
                *o |= MASK_DEBUG;
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
                return RETURN_OK_THRU; // caller should exit without errors
            case 'V': // --version
                printf("%s - Interpreter for the Xusto language (v. %s)\n",
                    TARGET_STRING,VERSION_STRING);
                return RETURN_OK_THRU; // caller should exit without errors
            case 'v': // --verbose
                if (optarg != NULL) {
                    if (!strcmp(optarg,"v") || !strcmp(optarg,"2")) {
                        *o |= (2<<SHIFT_VERBOSE);
                    } else if (!strcmp(optarg,"vv") || !strcmp(optarg,"3")) {
                        *o |= (3<<SHIFT_VERBOSE);
                    } else {
                        eprintf("Invalid verbosity level: %s\n", optarg);
                    }
                } else {
                    *o |= (1<<SHIFT_VERBOSE);
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
    bprintf(2,"options = 0x%08x\n", options);

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

int interactive(void) {
    return RETURN_OK;
}
