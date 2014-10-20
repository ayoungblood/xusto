# Makefile for xusto
#

# compiler
CC = gcc

# compiler flags
CFLAGS = -g -Wall -Wextra

# target
TARGET = xusto


$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

all: $(TARGET)

clean:
	rm -Rfv $(TARGET) *.dSYM
