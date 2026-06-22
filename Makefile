

CXX = g++
CFLAGS = -Wall -Wextra --std=c++17 -Iinclude -Ilib -Ilib/SFML-2.6.2/include
#-I is the include flag, tells the compiler to look in a certain directory for
#	header files, -I. means search for headers in the same as the Makefile
#	(already the default)


LDFLAGS = -Llib -Llib/SFML-2.6.2/lib -Wl,-rpath,lib/SFML-2.6.2/lib
#-L is for finding static/dynamic library directory for linking (after compilation)
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system
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


#these could all be done implicitly btw, don't need to write out the rules

FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreFoundation

SRC = $(shell find src -name "*.cpp")
OBJ = $(SRC:.cpp=.o)

sandbox: $(OBJ)
	$(CXX) $(OBJ) -o sandbox $(LDFLAGS) $(LDLIBS) $(FRAMEWORKS)

debug: CFLAGS += -g
debug: sandbox

#-pthread here if needed


src/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

#% is a wildcard, so for anything that matches ... .o
#$@ is the taraget itself, so main.o
#$< is the first prereq, so main.c

clean:
	rm -f src/*.o src/*/*.o sandbox

