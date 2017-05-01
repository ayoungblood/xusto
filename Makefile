# Makefile
TARGET = xusto
CC = gcc
RM = rm -f
# C compiler flags
# (see https://stackoverflow.com/questions/3375697/useful-gcc-flags-for-c)
CFLAGS=-g -std=c11 -Wall -Wextra -Wshadow -Wpointer-arith \
-Wstrict-prototypes -Wmissing-prototypes -Wswitch-default -Wunused-macros
# -g: debugging symbols
# -std=c11: ISO/IEC9899:2011 (C11)
# -Wall -Wextra -pedantic: stricter warnings
# -Wshadow: warn if a local shadows something else
# -Wpointer-arith: warn on silly pointer operations
# -Wstrict-prototypes -Wmissing-prototypes: be strict about function prototypes
# -Wswitch-default: warn when a switch statement does not have a default case
# -Wunused-macros: macros not expanded are probably useless

# #-save-temps -fverbose-asm -masm=intel: make prettier disassembly (disabled)

# -Wfloat-equal
# -Wundef
# -Wcast-align
# -Wwrite-strings
# -Waggregate-return
# -Wcast-equal
# -Wswitch-enum
# -Wconversion
# -Wunreachable-code
# -Wformat=2
# -Wformat-security
# -D_FORTIFY_SOURCE=2
# -fPIE -fstack-protector-all
# -O3
# -Winit-self
# -pedantic (for C only)
# -fmudflap
# -march=native
# -Werror-implicit-function-declaration
# -Wold-style-definition
# -Wno-unused-result
# -ftrapv
# -Wconversion -Wsign-conversion

# echo | gcc -E -dM -
# gcc -E


# Preprocessor flags
CPPFLAGS =
LDFLAGS = -g
# LDFLAGS="-Wl,-z,now -Wl,-z,relro" ?
LDLIBS =

SRCS=src/xusto.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: xusto

xusto: $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

xusto.o: src/xusto.cpp

clean:
	-$(RM) src/*.o
	-$(RM) -r *.dSYM
	-$(RM) src/*.gch
	-$(RM) src/*/*.gch

dist-clean: clean
	-$(RM) $(TARGET)

test: xusto
	-./xusto -dv test.xs3
