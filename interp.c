/* interp.c
 * lang2d interpreter implementation
 *
 * Invoke as:
 *     > lang2d filename
 * Args:
 *     filename - path to source file to be executed
 */

#include "interp.h"

// Interpreter state struct
typedef struct {
    uint8_t  iptr[2]; // instruction pointer
    uint8_t  ivec[2]; // instruction vector
    uint8_t  bcon[2]; // beacon position
    uint8_t  warp[2]; // program space warp
    uint8_t  flags; // state flag bitfield
    uint8_t  pgm[256][256]; // program space
    uint16_t pgmsize[2]; // program space dimensions
    uint8_t* stack; // stack pointer, dynamic array
    uint64_t stacksize; // allocated stack height
} State;

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
    
    char c;
    i = j = 0;
    while ((c = fgetc(fsource)) != EOF) {
        if (c == '\\') {
            char header[256];
            fgets(header, sizeof(header), fsource);
            printf("Ignoring header: %s\n",header);
        } else if (c != '\n') {
            s.pgm[i][j] = c;
            ++i;
        } else {
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
