##############################################################################
## Source-level Makefile for SDK-1600
##
## The whole make process is driven from the top-level Makefile.  In contrast
## to the (poor) advice given in the Make documentation, I do _not_ call make
## recursively to build the source of this project.
##############################################################################

##############################################################################
## Project directory structure
##############################################################################
B=../bin
L=../lib

##############################################################################
## Project-wide build flags
##############################################################################
P = export PATH;

# Set "X" to be the executable extension
X =        


WARN    = -Wall -W -Wshadow -Wpointer-arith \
			-Wbad-function-cast -Wcast-qual 

 CC = $(P) gcc
#CC = $(P) /usr/local/gcc-3.0.1/bin/gcc

 DEF_FLAGS = -DDIRECT_HCIF

#OPT_FLAGS = -ggdb 
#OPT_FLAGS = -O
#OPT_FLAGS = -O2 -ggdb $(WARN)
 OPT_FLAGS = -O6 -fomit-frame-pointer $(WARN)
#OPT_FLAGS = -O2 -pg -ggdb $(WARN)

# SDK-1600 predates C99; retain GNU C89 compatibility on modern compilers.
CFLAGS = $(OPT_FLAGS) -std=gnu89 -I. -I.. $(DEF_FLAGS)
LFLAGS = -L../lib


OBJS=
PROG=
TOCLEAN=core

PROGS=$(PROG)

CFLAGS += $(SDL_CFLAGS) 
LFLAGS += $(SDL_LFLAGS)

##############################################################################
## Generic build-rules
##############################################################################

.PHONY: modern-as1600

all: $(OBJS) $(PROG)

# Build the Mira-derived assembler required by the modern examples.  It is
# intentionally separate from the legacy Release 3 assembler sources above.
modern-as1600:
	$(MAKE) -C modern-as1600

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(CFLAGS) $(LFLAGS)

clean:
	rm -f $(OBJS) 
	rm -f $(TOCLEAN)

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

##############################################################################
## Makefile.common includes all the subMakefiles and such
##############################################################################
 include Makefile.common
