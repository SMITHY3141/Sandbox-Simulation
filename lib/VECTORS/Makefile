
#AI was used here
#AItool: ChatGpt
#AI Wrote Code
CXX = g++
CFLAGS = -Wall -Wextra --std=c++17 -pedantic
#-I is the include flag, tells the compiler to look in a certain directory for
#	header files, -I. means search for headers in the same as the Makefile
#	(already the default)
#-L is for finding static/dynamic library directory for linking (after compilation)
#still don't know which library file we want specifically, one header might be
#	implemented by several different libraries
#static libraries are essentially a collection of object files, the linker will
#	only use the object files needed to resolve some signature, so you could
#	include a static file and never use it without any real consequence
#what I said above is a different story for dynamic libraries

#C is simple, there can only be one function with a given name.
#	So after compilation in C, functions calls in the machine code are given dummy
#	symbols, like jump to SYMBOL. Only in linking are these resolved, jump to
#	some specific location in the instruction memory.


OBJS = vectors.o
TARGETS = libvectors.a

all: $(TARGETS)

libvectors.a: $(OBJS)
	ar rcs $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
#% is a wildcard, so for anything that matches ... .o
#$@ is the taraget itself, so main.o
#$< is the first prereq, so main.c

clean:
	rm -f $(OBJS) $(TARGETS)


