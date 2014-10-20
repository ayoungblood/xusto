/* interp.c
 * Xusto interpreter implementation
 *
 * Invoke as:
 *     > xusto filename
 * Args:
 *     filename - path to source file to be executed
 * Options:
 *     -d - set debug flag
 *     -v - set verbose flag
 */

#include "xusto.h"

int main(int argc, char **argv) {
    State s;
    Options opts;
    int i = 0, j = 0;

    // miscellaneous interpreter housekeeping
    setbuf(stdout, NULL); // disable buffering on stdout

    // initialize interpreter state
    // values may be overridden by source file header
    s.iptr[0] = s.iptr[1] = 0;
    s.ivec[0] = 1;
    s.ivec[1] = 0;
    s.prtl[0] = s.prtl[1] = 0;
    s.warp[0] = s.warp[1] = 0;
    s.flags = 0x00 | STATE_F_EXECUTE;
    for (i=0; i<256; ++i)
        for (j=0; j<256; ++j)
            s.pgm[i][j] = ' ';
    s.pgmsize[0] = s.pgmsize[1] = 0;
    s.stack = malloc(sizeof(s.stack)*STACKLEN_INITIAL);
    s.stackbase = s.stack;
    s.stackmax = STACKLEN_INITIAL;

    // initialize opts (keeps track of command line options)
    // many of these override values set by header or defaults
    opts.flags = 0;
    opts.flagsMask = 0;

    // validate args, if they exist
    if (argc == 1) {
        message(MSG_ERR_NOSOURCE,0);
        return 0;
    } else {
        for (i=0; i<argc-2; ++i) {
            switch (argv[i+1][1]) {
                case 'd': // -d: force debug
                    opts.flags |= STATE_F_DEBUG;
                    opts.flagsMask |= STATE_F_DEBUG;
                    break;
                case 'v': // -v: verbose
                    opts.flags |= STATE_F_VERBOSE;
                    opts.flagsMask |= STATE_F_VERBOSE;
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

    // override flags with command line options
    s.flags |= opts.flags & opts.flagsMask;
    // s.flags &= ~opts.flags | ~opts.flagsMask;

    // if debug or verbose, print out header settings
    if (s.flags & STATE_F_DEBUG || s.flags & STATE_F_VERBOSE) {
        printf(ANSI_C_MAGENTA MISCSTR_FLAGS ": 0x%02X\n",s.flags);
        printf(MISCSTR_WARP ": [0x%02X,0x%02X]\n",s.warp[0],s.warp[1]);
        printf(MISCSTR_PORTAL ": [0x%02X,0x%02X]\n",s.prtl[0],s.prtl[1]);
        printf(MISCSTR_IPTR ": [0x%02X,0x%02X]\n",s.iptr[0],s.iptr[1]);
        printf(MISCSTR_IVEC ": [0x%02X,0x%02X]\n",s.ivec[0],s.ivec[1]);
        printf(MISCSTR_PGMSIZE ": [0x%02X,0x%02X]\n" ANSI_C_RESET,s.pgmsize[0],s.pgmsize[1]);
        if (s.flags & STATE_F_DEBUG) {
            printf("\n" ANSI_C_MAGENTA "#### " MISCSTR_PGMSPACE " ####" ANSI_C_RESET "\n");
            for (j=0; j<s.pgmsize[1]; ++j) {
                for (i=0; i<s.pgmsize[0]; ++i) {
                    printf("%c",s.pgm[i][j]);
                }
                printf("\n");
            }
            printf(ANSI_C_MAGENTA "#### " MISCSTR_PGMSIZE ": [0x%02X,0x%02X] ####" ANSI_C_RESET "\n\n",
                s.pgmsize[0],s.pgmsize[1]);
        }
    }

    // run
    while (s.flags & STATE_F_EXECUTE) {
        execute(&s);
        update(&s);
        usleep(SLUGGISHNESS);
        if (s.flags & STATE_F_EXCEPTION) {
            break;
        }
    }
    // if there is an exception, there is likely no stack to dump
    // inform user and exit posthaste
    if (s.flags & STATE_F_EXCEPTION) {
        message(MSG_ERR_EXITEXC,0);
        return MAIN_RV_CRASH;
    }

    if (s.flags & STATE_F_DEBUG) {
        message(MSG_DBG_EOX,0);
        dumpStack(&s);
    } else if (s.flags & STATE_F_VERBOSE) {
        message(MSG_INFO_EOX,0);
    }

    return MAIN_RV_CAREFREE;
}

/********
 * Execute the current instruction
 */
void execute(State* s) {
    uint8_t x, y, i;
    int v, w;
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
        // binary & logical operators
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
        // comparison operators
        case 'G': // greater than
            x = POPVAL(s);
            POPVAL(s) > x ? PUSHVAL(1,s) : PUSHVAL(0,s);
            break;
        case '=': // equality
            x = POPVAL(s);
            POPVAL(s) == x ? PUSHVAL(1,s) : PUSHVAL(0,s);
            break;
        // flow control & branching
        case '>':
            s->ivec[0] = 1;
            s->ivec[1] = 0;
            break;
        case 'v':
            s->ivec[0] = 0;
            s->ivec[1] = 1;
            break;
        case '<':
            s->ivec[0] = -1;
            s->ivec[1] =  0;
            break;
        case '^':
            s->ivec[0] =  0;
            s->ivec[1] = -1;
            break;
        case 'x':
            s->ivec[0] = POPVAL(s);
            break;
        case 'y':
            s->ivec[1] = POPVAL(s);
            break;
        case 'B':
            s->ivec[0] = -s->ivec[0];
            s->ivec[1] = -s->ivec[1];
            break;
        case '_': // teleport
            s->iptr[0] += s->ivec[0];
            s->iptr[1] += s->ivec[1];
            break;
        case 'T': // branch left/right
            if (POPVAL(s)) {
                s->ivec[0] = 1;
                s->ivec[1] = 0;
            } else {
                s->ivec[0] = -1;
                s->ivec[1] =  0;
            }
            break;
        case 'K': // branch up/down
            if (POPVAL(s)) {
                s->ivec[0] = 0;
                s->ivec[1] = 1;
            } else {
                s->ivec[0] =  0;
                s->ivec[1] = -1;
            }
            break;
        case 'Q': // teleport 50% of the time
            if (random_u8() < 128) {
                s->iptr[0] += s->ivec[0];
                s->iptr[1] += s->ivec[1];
            }
            break;
        // stack manipulation
        case 'S': // swap
            *(s->stack) ^= *(s->stack-1);
            *(s->stack-1) ^= *(s->stack);
            *(s->stack) ^= *(s->stack-1);
            break;
        case 'P': // pop
            POPVAL(s);
            break;
        case 'D': // duplicate
            PUSHVAL(PEEKVAL(s),s);
            break;
        // execution control
        case ' ': // nop
            break;
        case 'H': // halt
            s->flags &= ~STATE_F_EXECUTE;
            break;
        case 'w': // wait
            // this instruction will pause execution for
            // 37ms multiplied by the current phase of the moon
            x = moonphase();
            for (i=0;i<x;++i) {
                usleep(37*1000);
            }
            break;
        // input/output
        case 'i': // read in an integer from stdin and push it
            v = scanf("%d",&w);
            if (v != EOF) { // if we got anything, push
                PUSHVAL((uint8_t)(w%256),s);
            }
            // clear buffer until newline
            while (getchar() != '\n');
            break;
        case 's': // read in a character from stdin and push it
            v = scanf("%c",c);
            if (v != EOF) { // if we got anything, push
                PUSHVAL((uint8_t)*c,s);
            }
            // clear buffer until newline
            while (getchar() != '\n');
            break;
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
        case 'W': // whack the interpreter
            printf("\n" MISCSTR_OUCH "\n");
            break;
        // program space modification
        case 'm': // mutate [x,y] to v
            v = POPVAL(s);
            y = POPVAL(s);
            x = POPVAL(s);
            s->pgm[x][y] = (uint8_t)v;
            break;
        case 'g': // get: push value at [x,y]
            y = POPVAL(s);
            x = POPVAL(s);
            PUSHVAL(s->pgm[x][y],s);
            break;
        // other stuff
        case '`': // set warp
            y = POPVAL(s);
            s->warp[0] = POPVAL(s);
            s->warp[1] = y;
            break;
        case '#': // drop portal
            s->prtl[0] = s->iptr[0];
            s->prtl[1] = s->iptr[1];
            break;
        case '@': // use portal
            s->iptr[0] = s->prtl[0];
            s->iptr[1] = s->prtl[1];
            break;
        case '"': // toggle PUSHCHAR
            s->flags ^= STATE_F_PUSHCHAR;
            break;
        case '?': // toggle DEBUG
            s->flags ^= STATE_F_DEBUG;
            message(MSG_DBG_ENABLED,0);
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
    // Many things are still a bit broken in here,
    // and determining precedence will be a difficult task
    // also, TODO: fix signed warp and warp behaviour
    int tmpiptr[2], tmpwarp[2] = {0,0};
    tmpiptr[0] = (int)s->iptr[0] + s->ivec[0];
    tmpiptr[1] = (int)s->iptr[1] + s->ivec[1];
    while (tmpiptr[0] > s->pgmsize[0]-1) {
        tmpiptr[0] -= s->pgmsize[0];
        tmpwarp[1] += s->warp[1];
    }
    while (tmpiptr[0] < 0) {
        tmpiptr[0] += s->pgmsize[0];
        tmpwarp[1] -= s->warp[1];
    }
    while (tmpiptr[1] > s->pgmsize[1]-1) {
        tmpiptr[1] -= s->pgmsize[1];
        tmpwarp[0] += s->warp[0];
    }
    while (tmpiptr[1] < 0) {
        tmpiptr[1] += s->pgmsize[1];
        tmpwarp[0] -= s->warp[0];
    }

    s->iptr[0] = tmpiptr[0] + tmpwarp[0];
    s->iptr[1] = tmpiptr[1] + tmpwarp[1];

    // if we're in danger of falling off the end of the stack,
    // grow the stack carefully. Magic number 8 is magic
    if (s->stack-s->stackbase > s->stackmax-8) {
        s->stackmax += STACKLEN_INITIAL;
        if (s->flags & STATE_F_DEBUG) {message(MSG_DBG_STACKRESIZE,0);}
        long int diff = s->stack-s->stackbase;
        s->stackbase = (uint8_t*)realloc(s->stackbase, s->stackmax*sizeof(uint8_t));
        if (!s->stackbase) {
            message(MSG_ERR_STACKALLOC,0);
            s->flags |= STATE_F_EXCEPTION;
        }
        s->stack = s->stackbase + diff;
    }
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
    // portal x
    token = strstr(h,"lx:");
    if (token) {
        if (sscanf(token+3, "0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","bx");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->prtl[0] = (uint8_t)v;
        }
    }
    // portal y
    token = strstr(h,"ly:");
    if (token) {
        if (sscanf(token+3, "0x%02X",&v) != 1) {
            (void)sprintf(c,": %s","by");
            message(MSG_WRN_HEADERTOKEN,c);
        } else {
            s->prtl[1] = (uint8_t)v;
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
}

/********
 * Print the contents of the stack
 */
void dumpStack(State* s) {
    uint8_t *p, v;
    printf("stack: %p, stackbase: %p, diff: %ld\n" ANSI_C_RED,s->stack,s->stackbase,s->stack-s->stackbase);
    while (s->stack > s->stackbase) {
        p = s->stack;
        v = POPVAL(s);
        printf("%p: 0x%02X",p,v);
        if (v >= 32 && v < 127) {
            printf("  %c",(char)v);
        }
        printf("\n");
    }
    printf(ANSI_C_RESET);
}

/********
 * Generate a random value
 */
uint8_t random_u8(void) {
    // using a weak, untested algorithm from
    // http://www.avrfreaks.net/index.php?name=PNphpBB2&file=printview&t=126506
    // TODO fix
    static uint8_t s=0xd0, a=0x89;
    s^=s<<3;
    s^=s>>5;
    s^=a++>>2;
    return s;
}

/********
 * Get the current phase of the moon
 * This is only guaranteed to work on Unix- and Posix-compliant systems
 */
uint8_t moonphase(void) {
    time_t now = time(NULL); // epoch time in s,
    time_t nm = (time_t)1389126900;
    uint8_t phase = ((now-nm) % 2551443)/(24*3600) + 1;
    if (phase > 29) {message(MSG_WRN_MOONPHASE,0);}
    return phase;
}
