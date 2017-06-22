// unicode_test.c
// Compile and run with
//    gcc -Wall -Wextra -o ut unicode_test.c && ./ut && rm ut
// or, for hex dump
//    gcc -Wall -Wextra -o ut unicode_test.c && ./ut | xxd && rm ut

#include <stdio.h>
#include <stdint.h>

void print_utf8(uint64_t c) {
    // Octal notation because UTF-8 uses six bit groups
    if (c < 0200) {
        // 00 - 0177 (0x0 - 0x7f): one byte
        printf("%c",(char)(c & 0177));
    } else if (c < 04000) {
        // 0200 - 03777 (0x80 - 0x7ff): two bytes
        // 0x80 - 0x7ff: two bytes
        printf("%c",(char)((c>>6 & 037) | 0300));
        printf("%c",(char)((c & 077) | 0200));
    } else if (c < 0xffff) {
        // 04000 - 0177777 (0x800 - 0xffff): three bytes
        printf("%c",(char)((c>>12 & 017) | 0340));
        printf("%c",(char)((c>>6 & 077) | 0200));
        printf("%c",(char)((c & 077) | 0200));
    } else {
        // 0200000 - 04177777 (0x10000 - 0x10FFFF): four bytes
        printf("%c",(char)((c>>18 & 07) | 0360));
        printf("%c",(char)((c>>12 & 077) | 0200));
        printf("%c",(char)((c>>6 & 077) | 0200));
        printf("%c",(char)((c & 077) | 0200));
    }
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    // See https://en.wikipedia.org/wiki/UTF-8
    char *str1 = "\x24\n";
    char *str2 = "\xC2\xA2\n";
    char *str3 = "\xE2\x82\xAC\n";
    char *str4 = "\xF0\x90\x8D\x88\n";
    printf("%s",str1);
    printf("%s",str2);
    printf("%s",str3);
    printf("%s",str4);

    printf("\n\n");
    print_utf8(0x24);
    printf("\n");
    print_utf8(0xA2);
    printf("\n");
    print_utf8(0x20AC);
    printf("\n");
    print_utf8(0x10348);
    printf("\n");
    return 0;
}
