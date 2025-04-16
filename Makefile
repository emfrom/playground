# Configurable flags
CC = gcc
CFLAGS ?= -Wall -g -pg
LDFLAGS ?=

# List of source files (targets)
TARGETS := countingsort radixsort sorttest suffixarray xload xmalloc



# Object files (compiled without -DUNIT_TEST)
OBJ := $(TARGETS:%=%.o)

# Unit test executables (compiled with -DUNIT_TEST)
TESTS := $(TARGETS:%=%.test)

# Default target
#all: $(OBJ) $(TESTS)
all: $(TESTS)

# Compile .c to .o without -DUNIT_TEST
#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $@

# Compile .c to .test with -DUNIT_TEST
%.test: %.c
	$(CC) $(CFLAGS) -DUNIT_TEST $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(TESTS) gmon.out

.PHONY: all clean
