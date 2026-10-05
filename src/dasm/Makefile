##############################################################################
## Source-level Makefile for as1600  (Linux/Unix Native Build)
##############################################################################

##############################################################################
## Project directory structure
##############################################################################
B=../../bin

##############################################################################
## Project-wide build flags
##############################################################################
P          = 

#WARN    = -Wall -W -Wshadow -Wpointer-arith 
#WARN   += -Wbad-function-cast -Wcast-qual 
WARN =

#CC = $(P) egcs
 CC = $(P) gcc


#OPT_FLAGS = -O
#OPT_FLAGS = -O2 -ggdb $(WARN)
 OPT_FLAGS = -O6 -fomit-frame-pointer $(WARN)
#OPT_FLAGS = -O2 -pg -ggdb $(WARN)

CFLAGS = $(OPT_FLAGS) -I. -I.. $(DEF_FLAGS)
LFLAGS = -L../lib

##############################################################################

PROGS   = $(B)/dasm1600 $(B)/dasm1600o

all: $(PROGS)

clean:

$(B)/dasm1600: dasm1600.c
	$(CC) -s -o $(B)/dasm1600 $(CFLAGS) dasm1600.c -static

$(B)/dasm1600o: dasm1600o.c
	$(CC) -s -o $(B)/dasm1600o $(CFLAGS) dasm1600o.c -static

