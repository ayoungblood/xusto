CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall -Wextra
LDFLAGS=-g
LDLIBS=

SRCS=src/xusto.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
#TARGET=xusto

all: xusto

xusto: $(OBJS)
	$(CXX) $(LDFLAGS) -o xusto $(OBJS) $(LDLIBS)

xusto.o: src/xusto.cpp

clean:
	-$(RM) src/*.o
	-$(RM) -r *.dSYM

dist-clean: clean
	-$(RM) xusto
