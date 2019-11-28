# Makefile to compile a complex program composed of many libraries

### Variables for this project ###
# These should be the only ones that need to be modified
# The files that must be compiled, with a .o extension
OBJECTS = mybarrier.o string_functions.o pgm_image.o gol.o
# The header files
DEPENDS = string_functions.h pgm_image.h mybarrier.h gol.h
# The executable program to be created
MAIN = main
SIMPLE = simple
OMP = omp
### Variables for the compilation rules ###
# These should work for most projects, but can be modified when necessary
# The compiler program to use
CC = gcc
# Options to use when compiling object files
CFLAGS = -Wall -g -std=c99 -pedantic -fopenmp -pthread # -O2
# Options to use for the final linking process
# This one links the math library
LDLIBS = -lm -fopenmp

### The rules ###
# These should work for most projects without change
# Special variable meanings:
#   $@  = The name of the rule
#   $^  = All the requirements for the rule
#   $<  = The first required file of the rule

# Default rule
all: $(MAIN) $(OMP) $(SIMPLE)

# Rule to make the executable
$(SIMPLE): simple.o $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Rule to make the executable
$(MAIN): main.o $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(OMP): omp.o $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Rule to make the object files
%.o: %.c $(DEPENDS)
	$(CC) $< -c -o $@ $(CFLAGS)

# Clear the compiled files
clean:
	rm -rf *.o $(MAIN)

clean_boards:
	rm Boards/padded_ig30{1..9}*
	rm Boards/padded_ig3{1..9}*

# Indicate the rules that do not refer to a file
.PHONY: clean all
