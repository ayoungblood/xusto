/* interp.c
 * lang2d interpreter implementation
 *
 * Invoke as:
 *     > lang2d filename
 * Args:
 *     filename - path to source file to be executed
 * Options:
 *     -d - set debug flag
 */

#include "interp.h"

int main(int argc, char **argv) {
    State s;
    int i = 0, j = 0;
    
    // miscellaneous interpreter housekeeping
    setbuf(stdout, NULL); // disable buffering on stdout
    
    // initialize interpreter state
    // values may be overridden by source file header
    s.iptr[0] = s.iptr[1] = 0;
    s.ivec[0] = 1;
    s.ivec[1] = 0;
    s.bcon[0] = s.bcon[1] = 0;
    s.warp[0] = s.warp[1] = 0;
    s.flags = 0x00 | STATE_F_EXECUTE;
    for (i=0; i<256; ++i)
        for (j=0; j<256; ++j)
            s.pgm[i][j] = ' ';
    s.pgmsize[0] = s.pgmsize[1] = 0;
    s.stack = malloc(sizeof(s.stack)*STACKLEN_INITIAL);
    s.stackbase = s.stack;
    s.stackmax = 64;
    
    // validate args, if they exist
    if (argc == 1) {
        message(MSG_ERR_NOSOURCE,0);
        return 0;
    } else {
        for (i=0; i<argc-2; ++i) {
            switch (argv[i+1][1]) {
                case 'd': // debug
                    s.flags |= STATE_F_DEBUG;
                    break;
                default:
                    message(MSG_WRN_BADOPTION,&argv[i+1][1]);
                    break;
            }
        }
    }
    // read in the source file to program space
    // the source file is always the last argument to the interpreter
    FILE *fsource = fopen(argv[argc-1], "r");
    if (!fsource) {
        message(MSG_ERR_BADSOURCE,0);
        return 0;
    }
    s.pgmsize[0] = s.pgmsize[1] = 0;
    char c;
    i = j = 0;
    while ((c = fgetc(fsource)) != EOF) {
        if (c == '\\') {
            char header[256];
            fgets(header, sizeof(header), fsource);
            // search header string for options and modify interpreter state
            processHeader(&s,header);
        } else if (c != '\n') {
            if (i==255) {
                message(MSG_ERR_SOURCEDIMS,0);
                return MAIN_RV_ERR;
            }
            s.pgm[i][j] = c;
            ++i;
        } else {
            if (j==255) {
                message(MSG_ERR_SOURCEDIMS,0);
                return MAIN_RV_ERR;
            }
            s.pgmsize[0] = s.pgmsize[0]<i?i:s.pgmsize[0];
            ++j;
            i = 0;
        }
    }
    s.pgmsize[1] = j;
    fclose(fsource);
    
    // if debug, print program space
    if (s.flags & STATE_F_DEBUG) {
        printf("\n#### " MISCSTR_PGMSPACE " ####\n");
        for (j=0; j<s.pgmsize[1]; ++j) {
            for (i=0; i<s.pgmsize[0]; ++i) {
                printf("%c",s.pgm[i][j]);
            }
            printf("\n");
        }
        printf("#### " MISCSTR_PGMSIZE ": [0x%02X,0x%02X] ####\n\n",s.pgmsize[0],s.pgmsize[1]);
    }
    
    // run
    while (s.flags & STATE_F_EXECUTE) {
        execute(&s);
        update(&s);
        usleep(SLUGGISHNESS);
    }
    printf("\n");
    message(MSG_INFO_EOX,0);
    return 0;
}
/********
 * Execute the current instruction
 */
void execute(State* s) {
    uint8_t x;
    char c[4];
    if (s->flags & STATE_F_PUSHCHAR && CURRENTINSTR(s) != '"') {
        PUSHVAL(CURRENTINSTR(s),s);
        return;
    }
    switch (CURRENTINSTR(s)) {
        // values
        case '0': PUSHVAL(0x0,s); break;
        case '1': PUSHVAL(0x1,s); break;
        case '2': PUSHVAL(0x2,s); break;
        case '3': PUSHVAL(0x3,s); break;
        case '4': PUSHVAL(0x4,s); break;
        case '5': PUSHVAL(0x5,s); break;
        case '6': PUSHVAL(0x6,s); break;
        case '7': PUSHVAL(0x7,s); break;
        case '8': PUSHVAL(0x8,s); break;
        case '9': PUSHVAL(0x9,s); break;
        case 'a': PUSHVAL(0xa,s); break;
        case 'b': PUSHVAL(0xb,s); break;
        case 'c': PUSHVAL(0xc,s); break;
        case 'd': PUSHVAL(0xd,s); break;
        case 'e': PUSHVAL(0xe,s); break;
        case 'f': PUSHVAL(0xf,s); break;
        // math operators
        case '+':
            PUSHVAL(POPVAL(s)+POPVAL(s),s);
            break;
        case '-':
            x = POPVAL(s);
            PUSHVAL(POPVAL(s)-x,s);
            break;
        case '*':
            PUSHVAL(POPVAL(s)*POPVAL(s),s);
            break;
        case '/':
            x = POPVAL(s);
            PUSHVAL(POPVAL(s)/x,s);
            break;
        case '%':
            x = POPVAL(s);
            PUSHVAL(POPVAL(s)%x,s);
            break;
        //other operators
        case '&':
            x = POPVAL(s);
            PUSHVAL(POPVAL(s)&x,s);
            break;
        case '|':
            x = POPVAL(s);
            PUSHVAL(POPVAL(s)|x,s);
            break;
        case 'r':
            x = POPVAL(s);
            PUSHVAL(POPVAL(s)^x,s);
            break;
        case 'R':
            x = POPVAL(s);
            PUSHVAL(POPVAL(s)>>x,s);
            break;
        case 'L':
            x = POPVAL(s);
            PUSHVAL(POPVAL(s)<<x,s);
            break;
        case '~':
            PUSHVAL(~POPVAL(s),s);
            break;
        case '!':
            PUSHVAL(!POPVAL(s),s);
            break;
        // directions
        case '>':
            s->ivec[0] = 1;
            s->ivec[1] = 0;
            break;
        case 'v':
            s->ivec[0] = 0;
            s->ivec[1] = 1;
            break;
        case '<':
            s->ivec[0] = 255;
            s->ivec[1] = 0;
            break;
        case '^':
            s->ivec[0] = 0;
            s->ivec[1] = 255;
            break;
        // input/output
        case '[':
            printf("%i", POPVAL(s));
            break;
        case ']':
            printf("%c", POPVAL(s));
            break;
        case '{':
            printf("%i", PEEKVAL(s));
            break;
        case '}':
            printf("%c", PEEKVAL(s));
            break;
        // other
        case '_': // teleport
            s->iptr[0] += s->ivec[0];
            s->iptr[1] += s->ivec[1];
            break;
        case '"': // toggle PUSHCHAR
            s->flags ^= STATE_F_PUSHCHAR;
            break;
        case '?': // toggle DEBUG
            s->flags ^= STATE_F_DEBUG;
            message(MSG_DBG_ENABLED,0);
            break;
        case 'H': // halt
            s->flags &= ~STATE_F_EXECUTE;
            break;
        case ' ': // nop
            break;
        default: // unrecognized instructions
            (void)sprintf(c,": %c",CURRENTINSTR(s));
            message(MSG_ERR_BADINSTR,c);
            break;
    }
    return;
}
/********
 * Update the interpreter state
 */
void update(State* s) {
    // detect wraparound in order to assign warp
    /*
    if ((int)s->iptr[0] + s->ivec[0] > 255) {
        // we are wrapping over in +x
        s->iptr[1] += s->warp[1];
        printf("wrapping in +x, warp applied\n");
    }
    */
    // advance instruction pointer
    s->iptr[0] += s->ivec[0];
    s->iptr[1] += s->ivec[1];
    return;
}
/********
 * Print messages to the user
 */
void message(const char* msg, int code, char* extra) {
    char* type;
    switch (code >> 8) {
        case 0: // SHOULD NOT HAPPEN
            type = ANSI_C_RED MSG_TYP_CATASTROPHIC;
            break;
        case 1: // ERROR
            type = ANSI_C_RED MSG_TYP_ERROR;
            break;
        case 2: // WARNING
            type = ANSI_C_YELLOW MSG_TYP_WARNING;
            break;
        case 3: // INFO
            type = ANSI_C_GREEN MSG_TYP_INFO;
            break;
        case 4: // DEBUG
            type = ANSI_C_GREEN MSG_TYP_DEBUG;
            break;
        default: // should not be reached
            printf(ANSI_C_RED MISCSTR_BADERROR "\n" ANSI_C_RESET);
            return;
    }
    char* ex = extra;
    if (!extra) ex = "";
    printf("%s 0x%04x: %s %s\n" ANSI_C_RESET, type, code, msg, ex);
    return;
}
/********
 * Process header string
 */
void processHeader(State* s, char* h) {
    char* token;
    char c[8];
    unsigned int v;
    // flags
    token = strstr(h,"f:");
    if (token) {
        if (sscanf(token+2,"0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","f");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->flags = (uint8_t)v;
        }
    }
    // warp x
    token = strstr(h,"wx:");
    if (token) {
        if (sscanf(token+3,"0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","wx");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->warp[0] = (uint8_t)v;
        }
    }
    // warp y
    token = strstr(h,"wy:");
    if (token) {
        if (sscanf(token+3, "0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","wy");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->warp[1] = (uint8_t)v;
        }
    }
    // beacon x
    token = strstr(h,"bx:");
    if (token) {
        if (sscanf(token+3, "0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","bx");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->warp[0] = (uint8_t)v;
        }
    }
    // beacon y
    token = strstr(h,"by:");
    if (token) {
        if (sscanf(token+3, "0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","by");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->warp[1] = (uint8_t)v;
        }
    }
    // iptr x
    token = strstr(h,"px:");
    if (token) {
        if (sscanf(token+3, "0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","px");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->iptr[0] = (uint8_t)v;
        }
    }
    // iptr y
    token = strstr(h,"py:");
    if (token) {
        if (sscanf(token+3, "0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","py");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->iptr[1] = (uint8_t)v;
        }
    }
    // ivec x
    token = strstr(h,"vx:");
    if (token) {
        if (sscanf(token+3, "0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","vx");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->ivec[0] = (uint8_t)v;
        }
    }
    // ivec y
    token = strstr(h,"vy:");
    if (token) {
        if (sscanf(token+3, "0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","vy");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->ivec[1] = (uint8_t)v;
        }
    }
    // pgmsize x
    token = strstr(h,"sx:");
    if (token) {
        if (sscanf(token+3, "0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","sx");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->pgmsize[0] = (uint8_t)v;
        }
    }
    // pgmsize y
    token = strstr(h,"sy:");
    if (token) {
        if (sscanf(token+3, "0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","sy");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->pgmsize[1] = (uint8_t)v;
        }
    }
    // if debug, print out header settings
    if (s->flags & STATE_F_DEBUG) {
        printf(MISCSTR_FLAGS ": 0x%02X\n",s->flags);
        printf(MISCSTR_WARP ": [0x%02X,0x%02X]\n",s->warp[0],s->warp[1]);
        printf(MISCSTR_BEACON ": [0x%02X,0x%02X]\n",s->bcon[0],s->bcon[1]);
        printf(MISCSTR_IPTR ": [0x%02X,0x%02X]\n",s->iptr[0],s->iptr[1]);
        printf(MISCSTR_IVEC ": [0x%02X,0x%02X]\n",s->ivec[0],s->ivec[1]);
        printf(MISCSTR_PGMSIZE ": [0x%02X,0x%02X]\n",s->pgmsize[0],s->pgmsize[1]);
    }
}
