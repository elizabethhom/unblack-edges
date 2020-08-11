# New syntax rules added:  wildcards (%.o, %.c) and special variables:
# $@ - the target of the rule
# $< - the first dependency
# $^ - all dependencies
#
# Last updated: February 4, 2016

############## Variables ###############

CC = gcc # The compiler being used

# Updating include path to use Comp 40 .h files and CII interfaces
IFLAGS = -I. -I/comp/40/build/include -I/usr/sup/cii40/include/cii

# Compile flags
# Set debugging information, allow the c99 standard,
# max out warnings, and use the updated include path
CFLAGS = -g -std=c99 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
# Set debugging information and update linking path
# to include course binaries and CII implementations
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64

# Libraries needed for linking
# Both programs need cii40 (Hanson binaries) and *may* need -lm (math)
# Only brightness requires the binary for pnmrdr.
LDLIBS = -lpnmrdr -lcii40 -lm

INCLUDES = $(shell echo *.h)

############### Rules ###############

all: unblackedges

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

unblackedges: unblackedges.o bit2.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	
clean:
	rm -f sudoku unblackedges my_useuarray2 my_usebit2 *.o
