# Configurable flags
CFLAGS ?= -Wall -g -pg
LDFLAGS ?=

# List of source files (targets)
TARGETS := countingsort radixsort sorttest suffixarray xload xmalloc

# Compiler
CC = gcc

# Object files (compiled without -DUNIT_TEST)
OBJ := $(TARGETS:%=%.o)

# Unit test executables (compiled with -DUNIT_TEST)
TESTS := $(TARGETS:%=%.unit_test)

# Default target
all: $(OBJ) $(TESTS)

# Compile .c to .o without -DUNIT_TEST
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .c to .unit_test with -DUNIT_TEST
%.unit_test: %.c
	$(CC) $(CFLAGS) -DUNIT_TEST $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(TESTS)

.PHONY: all clean
