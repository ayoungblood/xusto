/* interp.c
 * lang2d interpreter implementation
 *
 * Invoke as:
 *     > lang2d filename
 * Args:
 *     filename - path to source file to be executed
 */

#include "interp.h"

State s;

int main(int argc, char **argv) {
    int i = 0, j = 0;
    // initialize interpreter state
    // values may be overridden by source file header
    s.iptr[0] = s.iptr[1] = 0;
    s.ivec[0] = 1;
    s.ivec[1] = 0;
    s.bcon[0] = s.bcon[1] = 0;
    s.warp[0] = s.warp[1] = 0;
    s.flags = 0x00;
    for (i=0; i<256; ++i)
        for (j=0; j<256; ++j)
            s.pgm[i][j] = ' ';
    s.pgmsize[0] = s.pgmsize[1] = 0;
    s.stack = malloc(sizeof(s.stack)*64);
    s.stacksize = 64;
    
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
    // read in source
    // source file is last argument to interpreter
    FILE *fsource = fopen(argv[argc-1], "r");
    if (!fsource) {
        message(MSG_ERR_BADSOURCE,0);
        return 0;
    }
    s.pgmsize[0] = s.pgmsize[1] = 0;
    // load source into program space
    char c;
    i = j = 0;
    while ((c = fgetc(fsource)) != EOF) {
        if (c == '\\') {
            char header[256];
            fgets(header, sizeof(header), fsource);
            // FIXME process header properly
            printf("Ignoring header: %s\n",header);
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
    
    // print program space
    printf("########\n");
    for (j=0; j<s.pgmsize[1]; ++j) {
        for (i=0; i<s.pgmsize[0]; ++i) {
            printf("%c",s.pgm[i][j]);
        }
        printf("\n");
    }
    printf("########\nProgram size: %i,%i\n",s.pgmsize[0],s.pgmsize[1]);
    return 0;
}
/********
 * Execute an instruction
 */
void execute(char* c, State* s) {
    //
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
            printf(ANSI_C_RED MISCSTR_BADERROR ANSI_C_RESET);
            return;
    }
    char* ex = extra;
    if (!extra) ex = "";
    printf("%s 0x%04x: %s %s\n" ANSI_C_RESET, type, code, msg, ex);
    return;
}
