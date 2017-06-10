# Makefile
TARGET = xusto
CC = gcc
RM = rm -f
# C compiler flags
# (see https://stackoverflow.com/questions/3375697/useful-gcc-flags-for-c)
CFLAGS=-g -std=c11 -Wall -Wextra -pedantic -O3 -Wshadow -Wpointer-arith \
-Wstrict-prototypes -Wmissing-prototypes -Wswitch-default -Wunused-macros \
-Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wcast-qual -Wswitch-enum \
-Wconversion -Wold-style-definition -Wformat=2 -Winit-self \
-march=native -fwrapv
# -g: debugging symbols
# -std=c11: ISO/IEC9899:2011 (C11)
# -Wall -Wextra -pedantic: stricter warnings
# -O3: extra warnings and more speeeeeeed
# -Wshadow: warn if a local shadows something else
# -Wpointer-arith: warn on silly pointer operations
# -Wstrict-prototypes -Wmissing-prototypes: be strict about function prototypes
# -Wswitch-default: warn when a switch statement does not have a default case
# -Wunused-macros: macros not expanded are probably useless
# -Wfloat-equal: warn if floats are being compared for equality
# -Wundef: warn if undefined identifier is evaluted in an #if directive
# -Wcast-align: warn on pointer casts that increase required alignment
# -Wwrite-strings: better warnings about string constants
# -Wcast-qual: warn on unsafe type qualifier casts
# -Wswitch-enum: warn on un-cased unumerated values
# -Wconversion: warn on implicit conversions that may alter a value
# -Wold-style-definition: warn on old-style function definition
# -Wformat=2: additional format string warnings
# -Winit-self: warn in silliness
# -march=native: optimize for current platform
# -fwrapv: assume signed arithmetic overflows using twos-complement
#### Disabled flags
# #-save-temps -fverbose-asm -masm=intel: make prettier disassembly (disabled)
# #-Waggregate-return: warn on returning structures or unions (disabled)
# # -Wunreachable-code: self-explanatory (disabled)
# #-D_FORTIFY_SOURCE=2: runtime buffer overflow checks (disabled)
# #-Wno-unused-result: don't warn on unused result (disabled)

# echo | gcc -E -dM -
# gcc -E

LIBS =

# Preprocessor flags
CPPFLAGS =
LDFLAGS = -g -Wall -Wextra
# #-fmudflap -lmudflap: runtime checking for safety (disabled)
# LDFLAGS="-Wl,-z,now -Wl,-z,relro" ?
LDLIBS =

SRCS=src/xusto.c
OBJS=$(subst .c,.o,$(SRCS))

.PHONY: test clean run

# Get all the header files and object files
HEADERS = $(wildcard src/*.h)
OBJECTS = $(patsubst %.c, %.o, $(wildcard src/*.c))

# Set the version string from the git commit tag, unless we can't
HAVE_GIT := $(shell command -v git 2>/dev/null)
ifdef HAVE_GIT
VERSION = $(shell git rev-parse --short HEAD)
else
VERSION = "0.0.0"
endif

STRFLAGS = -D TARGET_STRING="\"$(TARGET)\"" -D VERSION_STRING="\"$(VERSION)\""

# Silence a few warnings in unit tests
TESTFLAGS = -Wno-strict-prototypes -Wno-old-style-definition

# Build all the object files
%.o: %.c $(HEADERS)
		$(CC) $(STRFLAGS) $(CFLAGS) -c $< -o $@

# Build the target
$(TARGET): $(OBJECTS)
		$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

all: $(TARGET)

run: $(TARGET)
		./$(TARGET) u.3

# Build, run, and clean unit tests
test: $(OBJECTS) all
		# vector3 tests
		$(CC) $(CFLAGS) $(TESTFLAGS) src/types.o $(LIBS) -o unit-test/vector3-test unit-test/vector3-test.c
		unit-test/vector3-test
		-@$(RM) unit-test/vector3-test
		# space_hashtable tests
		$(CC) $(CFLAGS) $(TESTFLAGS) src/types.o src/space_hashtable.o $(LIBS) -o unit-test/space_hashtable-test unit-test/space_hashtable-test.c
		unit-test/space_hashtable-test
		-@$(RM) unit-test/space_hashtable-test
		# space tests
		$(CC) $(CFLAGS) $(TESTFLAGS) src/types.o src/space_hashtable.o src/space.o $(LIBS) -o unit-test/space-test unit-test/space-test.c
		unit-test/space-test
		-@$(RM) unit-test/space-test
		@# clean up
		-@$(RM) -r unit-test/*.dSYM

clean:
		-$(RM) src/*.o
		-$(RM) -r *.dSYM
		-$(RM) src/*.gch
		-$(RM) src/*/*.gch
		-$(RM) $(TARGET)
