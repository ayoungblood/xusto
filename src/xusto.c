/*****************************************************************************\
 * xusto.c                                                                   *
 * Xusto interpreter, now with 0% more plus plus                             *
\*****************************************************************************/

#include "xusto.h"

extern flags_t flags;

int main(int argc, char **argv) {
    (void)argv;
    eprintf("argc: %d\n", argc);

    /* Parse command line options/arguments */
    /*
        -d, --debug
        -v, --verbose
        -c, --color
        -h, --help
        -V, --version
    */

    // Validate command line arguments --------------------
    /*
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
    // for each argument (which should be files), open, load, and execute
    for (; optind < argc; ++optind) {
        printf("arg[%d]: %s\n", optind, argv[optind]);
        //parse(argv[optind]);
    }
    */
    return RETURN_OK;
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
