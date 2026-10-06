# Reproducible standalone build of the modern Mira-derived AS1600.
#
# This source closure is intentionally separate from the legacy Release 3
# build in ../.  It provides the assembler required by the modern examples
# while the wider toolchain migration remains in progress.

CC      ?= gcc
CXX     ?= g++
CFLAGS  ?= -O3 -std=gnu99 -I.
CXXFLAGS ?= -O3 -std=c++14 -I.
LDFLAGS ?= -static -lrt

BIN     := ../../bin/as1600

ASM_SRC := asm/as1600.tab.c asm/fraosub.c asm/frapsub.c asm/frasmain.c
ASM_SRC += asm/fryylex.c asm/intermed.c asm/memo_string.c asm/collect.c
ASM_SRC += asm/intvec.c
ASM_CPP := asm/cpp_main.cpp
ICART_SRC := icart/icartrom.c icart/icartbin.c icart/icarttag.c
ICART_SRC += icart/icarttag_authors.c
SUPPORT_SRC := misc/crc16.c misc/types.c misc/ll.c misc/printer.c
SUPPORT_SRC += plat/plat_gen.c plat/gnu_getopt.c
IMASM_CPP := imasm/c_wrap.cpp imasm/parser.cpp imasm/token.cpp imasm/strfifo.cpp
BINCFG_SRC := bincfg/bincfg.c bincfg/bincfg_grmr.tab.c bincfg/bincfg_lex.c
META_SRC := metadata/metadata.c metadata/cfgvar_metadata.c metadata/icarttag_metadata.c
IO_SRC := minilzo/minilzo.c file/file.c lzoe/lzoe.c svn_revision.c

C_SRC := $(ASM_SRC) $(ICART_SRC) $(SUPPORT_SRC) $(BINCFG_SRC) $(META_SRC) $(IO_SRC)
OBJ := $(C_SRC:.c=.o) $(ASM_CPP:.cpp=.o) $(IMASM_CPP:.cpp=.o)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)
