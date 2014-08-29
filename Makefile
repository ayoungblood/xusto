# Makefile for lang2d
#

# compiler
CC = gcc

# compiler flags
CFLAGS = -g -Wall -Wextra

# target
TARGET = interp


$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

all: $(TARGET)

clean:
	rm -Rv $(TARGET) *.dSYM
	@echo Completed clean
