
/*  A Bison parser, made from asm/as1600.y
 by  GNU Bison version 1.27
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	REGISTER	257
#define	KOC_BDEF	258
#define	KOC_ELSE	259
#define	KOC_END	260
#define	KOC_ENDI	261
#define	KOC_EQU	262
#define	KOC_IF	263
#define	KOC_INCLUDE	264
#define	KOC_ORG	265
#define	KOC_RESM	266
#define	KOC_SDEF	267
#define	KOC_SET	268
#define	KOC_WDEF	269
#define	KOC_CHSET	270
#define	KOC_CHDEF	271
#define	KOC_CHUSE	272
#define	KOC_opcode	273
#define	KOC_opcode_i	274
#define	KOC_relbr	275
#define	KOC_relbr_x	276
#define	KOC_SDBD	277
#define	KOC_ROMW	278
#define	KOC_PROC	279
#define	KOC_ENDP	280
#define	KOC_STRUCT	281
#define	KOC_ENDS	282
#define	KOC_MEMATTR	283
#define	KOC_DDEF	284
#define	CONSTANT	285
#define	EOL	286
#define	KEOP_AND	287
#define	KEOP_DEFINED	288
#define	KEOP_EQ	289
#define	KEOP_GE	290
#define	KEOP_GT	291
#define	KEOP_HIGH	292
#define	KEOP_LE	293
#define	KEOP_LOW	294
#define	KEOP_LT	295
#define	KEOP_MOD	296
#define	KEOP_MUN	297
#define	KEOP_NE	298
#define	KEOP_NOT	299
#define	KEOP_OR	300
#define	KEOP_SHL	301
#define	KEOP_SHR	302
#define	KEOP_XOR	303
#define	KEOP_locctr	304
#define	LABEL	305
#define	STRING	306
#define	SYMBOL	307
#define	KTK_invalid	308

#line 1 "asm/as1600.y"


/*  NOTICE:  This code is based on the Public Domain AS2650.Y that comes
 *           with the Frankenstein Assembler, by Mark Zenier.  The changes
 *           that I, Joseph Zbiciak, have made are being placed under GPL.
 *           See GPL notice immediately below. 
 */

/* ======================================================================== */
/*  This program is free software; you can redistribute it and/or modify    */
/*  it under the terms of the GNU General Public License as published by    */
/*  the Free Software Foundation; either version 2 of the License, or       */
/*  (at your option) any later version.                                     */
/*                                                                          */
/*  This program is distributed in the hope that it will be useful,         */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       */
/*  General Public License for more details.                                */
/*                                                                          */
/*  You should have received a copy of the GNU General Public License       */
/*  along with this program; if not, write to the Free Software             */
/*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               */
/* ======================================================================== */
/*                 Copyright (c) 1998-1999, Joseph Zbiciak                  */
/* ======================================================================== */


/*
HEADER:     ;
TITLE:      Frankenstein Cross Assemblers;
VERSION:    2.0;
DESCRIPTION: "  Reconfigurable Cross-assembler producing Intel (TM)
                Hex format object records.  ";
KEYWORDS:   cross-assemblers, 1600, 1805, 2650, 6301, 6502, 6805, 6809, 
            6811, tms7000, 8048, 8051, 8096, z8, z80;
SYSTEM:     UNIX, MS-Dos ;
FILENAME:   as1600.y;
WARNINGS:   "This software is in the public domain.  
             Any prior copyright claims are relinquished.  

             This software is distributed with no warranty whatever.  
             The author takes no responsibility for the consequences 
             of its use.

             Yacc (or Bison) required to compile."  ;
SEE-ALSO:   as1600.ps, frasmain.c;  
AUTHORS:    Mark Zenier; Joe Zbiciak 
COMPILERS:  GCC
*/

/* 1600 instruction generation file, GI standard syntax */
/* September 25, 1999 */

/*
    description frame work parser description for framework cross assemblers
    history     February 2, 1988
                September 11, 1990 - merge table definition
                September 12, 1990 - short file names
                September 14, 1990 - short variable names
                September 17, 1990 - use yylex as external
*/

/* ======================================================================== *\

The CP-1610 supports the following basic opcode formats:

 ---------------------------------------  -------  --------------------------
  Format                                   Words    Description
 ---------------------------------------  -------  --------------------------
  0000 000 0oo                               1      Implied 1-op insns
  0000 000 100  bbppppppii  pppppppppp       3      Jump insns
  0000 000 1oo                               1      Implied 1-op insns
  0000 ooo ddd                               1      1-op insns, comb src/dst
  0000 110 0dd                               1      GSWD
  0000 110 1om                               1      NOP(2), SIN(2)
  0001 ooo mrr                               1      Rotate/Shift insns
  0ooo sss ddd                               1      2-op arith, reg->reg
  1000 zxc ccc  pppppppppp                   2      Branch insns
  1ooo 000 ddd  pppppppppp                   2      2-op arith, direct, reg
  1ooo mmm ddd                               1*     2-op arith, ind., reg
  1ooo 111 ddd  iiiiiiiiii                   2*     2-op arith, immed., reg
 ---------------------------------------  -------  --------------------------


 -----
  Key
 -----

  oo    -- Opcode field (dependent on format)
  sss   -- Source register,      R0 ... R7 (binary encoding)
  ddd   -- Destination register, R0 ... R7 (binary encoding)
  0dd   -- Destination register, R0 ... R3
  cccc  -- Condition codes (branches)
  x     -- External branch condition (0 == internal, 1 == examine BEXT)
  z     -- Branch displacement direction (1 == negative)
  m     -- Shift amount (0 == shift by 1, 1 == shift by 2)
  bb    -- Branch return register
  ii    -- Branch interrupt flag mode

 --------------------------------
  Branch Condition Codes  (cccc)
 --------------------------------
           n == 0                    n == 1
  n000  -- Always                    Never
  n001  -- Carry set/Greater than    Carry clear/Less than or equal
  n010  -- Overflow set              Overflow clear
  n011  -- Positive                  Negative
  n100  -- Equal                     Not equal
  n101  -- Less than                 Greater than or equal
  n110  -- Less than or equal        Greater than
  n111  -- Unequal sign and carry    Equal sign and carry


 -------------------------------
  Branch Return Registers  (bb)
 -------------------------------

  00   -- R4
  01   -- R5
  10   -- R6
  11   -- none (do not save return address)

 -------------------------------
  Branch Interrupt Modes   (ii)
 -------------------------------

  00   -- Do not change interrupt enable state
  01   -- Enable interrupts
  10   -- Disable interrupts
  11   -- Undefined/Reserved ?

 ------------
  SDBD notes
 ------------

  -- SDBD is supported on "immediate" and "indirect" modes only.

  -- An SDBD prefix on an immediate instruction sets the immediate constant
     to be 16 bits, stored in two adjacent 8-bit words.  The ordering is
     little-endian.

  -- An SDBD prefix on an indirect instruction causes memory to be
     accessed twice, bringing in (or out) two 8-bit words, again in
     little-endian order.  If a non-incrementing data counter is used,
     both accesses are to the same address.  Otherwise, the counter
     is post-incremented with each access.  Indirect through R6
     (stack addressing) is not allowed, although I suspect it works
     as expected (performing two accesses through R6).

 ------------------------
  General encoding notes
 ------------------------

  -- "Immediate" mode is encoded the same as "Indirect" mode, except that
     R7 is given as the indirect register.  I'm guessing R7 is implemented
     the same as R4 and R5, especially since MVOI does what you'd
     expect -- it (attempts to) write over its immediate operand!!!

  -- The PC value (in R7) used for arithmetic always points to the first
     byte after the instruction for purposes of arithmetic.  This is
     consistent with the observation that immediate mode is really
     indirect mode in disguise, with the instruction being only one word
     long initially.

  -- Several instructions are just special cases of other instructions,
     and therefore do not need special decoder treatment:

      -- TSTR Rx  -->  MOVR Rx, Rx
      -- JR Rx    -->  MOVR Rx, R7
      -- CLRR Rx  -->  XORR Rx, Rx
      -- B        -->  Branch with condition code 0000 ("Always")
      -- NOPP     -->  Branch with condition code 1000 ("Never")
      -- PSHR Rx  -->  MVO@ Rx, R6
      -- PULR Rx  -->  MVI@ R6, Rx

  -- "Direct" mode is encoded the same as "Indirect" mode, except 000
     (which corresponds to R0) is encoded in the indirect register field.
     This is why R0 cannot be used as a data counter, and why it has no
     "special use."

  -- Relative branches encode their sign bit in the opcode word, rather
     than relying on a sign-extended relative offset in their second word.
     This allows +/- 10-bit range in a 10-bit wide memory, or +/-
     16-bit range in a 16-bit memory.  To avoid redundant encoding, the
     offset is calculated slightly differently for negative vs. positive
     offset:

      -- Negative: address_of_branch + 1 - offset
      -- Positive: address_of_branch + 2 + offset

     I'm guessing it is implemented about like so in hardware:

      -- offset == pc + (offset ^ (sign ? -1 : 0))

 ---------------
  Opcode Spaces
 ---------------

  I've divided the CP-1610 opcode map into 12 different opcode
  spaces.  (I really should merge the two separate Implied 1-op
  spaces into one space.  Oh well...)  In the descriptions below,
  "n/i" means "not interruptible".  Defined flags: Sign, Zero, Carry,
  Overflow, Interrupt-enable, Double-byte-data.  Interrupt-enable and
  Double-byte-data are not user visible.

  -- Implied 1-op instructions, part A:     0000 000 0oo
     Each has a single, implied operand, if any.

         opcode   mnemonic n/i  SZCOID  description
      --   00       HLT                 Halts the CPU (until next interrupt?)
      --   01       SDBD    *        1  Set Double Byte Data
      --   10       EIS     *       1   Enable Interrupt System
      --   11       DIS     *       1   Disable Interrupt System

  -- Implied 1-op instructions, part B:     0000 000 1oo
     Each has a single, implied operand, if any.

         opcode   mnemonic n/i  SZCOID  description
      --   00       n/a                 Aliases the "Jump" opcode space
      --   01       TCI     *           Terminate Current Interrupt.
      --   10       CLRC    *           Clear carry flag
      --   11       SETC    *           Set carry flag

  -- Jump Instructions:                     0000 000 100 bbppppppii pppppppppp
     Unconditional jumps with optional return-address save and
     interrupt enable/disable.

          bb  ii   mnemonic n/i  SZCOID description
      --  11  00    J                   Jump.
      --  xx  00    JSR                 Jump.  Save return address in R4..R6
      --  11  01    JE              1   Jump and enable ints.
      --  xx  01    JSRE            1   Jump and enable ints.  Save ret addr.
      --  11  10    JD              0   Jump and disable ints
      --  xx  10    JSRD            0   Jump and disable ints.  Save ret addr.
      --  xx  11    n/a                 Invalid opcode.

  -- Register 1-op instructions             0000 ooo rrr
     Each has one register operand, encoded as 000 through 111.

         opcode   mnemonic n/i  SZCOID  description
      --   000      n/a                 Aliases "Implied", "Jump" opcode space
      --   001      INCR        XX      INCrement register
      --   010      DECR        XX      DECrement register
      --   011      COMR        XX      COMplement register (1s complement)
      --   100      NEGR        XXXX    NEGate register     (2s complement)
      --   101      ADCR        XXXX    ADd Carry to Register
      --   110      n/a                 Aliases "GSWD", "NOP/SIN" opcode space
      --   111      RSWD        XXXX    Restore Status Word from Register


  -- Get Status WorD                        0000 110 0rr
     This was given its own opcode space due to limited encoding on its
     destination register and complication with the NOP/SIN encodings.

  -- NOP/SIN                                0000 110 1om
     I don't know what the "m" bit is for.  I don't know what to do with SIN.

         opcode   mnemonic n/i  SZCOID  description
      --    0       NOP                 No operation
      --    1       SIN                 Software Interrupt (pulse PCIT pin) ?

  -- Shift/Rotate 1-op instructions         0001 ooo mrr
     These can operate only on R0...R3.  The "m" bit specifies whether the
     operation is performed once or twice.  The overflow bit is used for
     catching the second bit on the rotates/shifts that use the carry.

         opcode   mnemonic n/i  SZCOID  description
      --   000      SWAP    *   XX      Swaps bytes in word once or twice.
      --   001      SLL     *   XX      Shift Logical Left
      --   010      RLC     *   XXX2    Rotate Left through Carry/overflow
      --   011      SLLC    *   XXX2    Shift Logical Left thru Carry/overflow
      --   100      SLR     *   XX      Shift Logical Right
      --   101      SAR     *   XX      Shift Arithmetic Right
      --   110      RRC     *   XXX2    Rotate Left through Carry/overflow
      --   111      SARC    *   XXX2    Shift Arithmetic Right thru Carry/over

  -- Register/Register 2-op instructions    0ooo sss ddd
     Register to register arithmetic.  Second operand acts as src2 and dest.

         opcode   mnemonic n/i  SZCOID  description
      --   00x      n/a                 Aliases other opcode spaces
      --   010      MOVR        XX      Move register to register
      --   011      ADDR        XXXX    Add src1 to src2->dst
      --   100      SUBR        XXXX    Sub src1 from src2->dst
      --   101      CMPR        XXXX    Sub src1 from src2, don't store
      --   110      ANDR        XX      AND src1 with src2->dst
      --   111      XORR        XX      XOR src1 with src2->dst

  -- Conditional Branch instructions        1000 zxn ccc pppppppppppppppp
     The "z" bit specifies the direction for the offset.  The "x" bit
     specifies using an external branch condition instead of using flag
     bits.  Conditional brances are interruptible.  The "n" bit specifies
     branching on the opposite condition from 'ccc'.

          cond      n=0         Condition       n=1         Condition
      --  n000      B           always          NOPP        never
      --  n001      BC          C = 1           BNC         C = 0
      --  n010      BOV         O = 1           BNOV        O = 0
      --  n011      BPL         S = 0           BMI         S = 1
      --  n100      BZE/BEQ     Z = 1           BNZE/BNEQ   Z = 0
      --  n101      BLT/BNGE    S^O = 1         BGE/BNLT    S^O = 0
      --  n110      BLE/BNGT    Z|(S^O) = 1     BGT/BNLE    Z|(S^O) = 0
      --  n111      BUSC        S^C = 1         BESC        S^C = 0

  -- Direct/Register 2-op instructions      1ooo 000 rrr  pppppppppppppppp
     Direct memory to register arithmetic.  MVO uses direct address as
     a destination, all other operations use it as a source, with
     the register as the destination.

         opcode   mnemonic n/i  SZCOID  description
      --   000      n/a                 Aliases conditional branch opcodes
      --   001      MVO     *           Move register to direct address
      --   010      MVI                 Move direct address to register
      --   011      ADD         XXXX    Add src1 to src2->dst
      --   100      SUB         XXXX    Sub src1 from src2->dst
      --   101      CMP         XXXX    Sub src1 from src2, don't store
      --   110      AND         XX      AND src1 with src2->dst
      --   111      XOR         XX      XOR src1 with src2->dst


  -- Indirect/Register 2-op instructions    1ooo sss ddd
     A source of "000" is actually a Direct/Register opcode.
     A source of "111" is actually a Immediate/Register opcode.
     R4, R5 increment after each access.  If the D bit is set, two
     accesses are made through the indirect register, updating the
     address if R4 or R5.  R6 increments after writes, decrements
     before reads.

         opcode   mnemonic n/i  SZCOID  description
      --   000      n/a                 Aliases conditional branch opcodes
      --   001      MVO@    *           Move register to indirect address
      --   010      MVI@                Move indirect address to register
      --   011      ADD@        XXXX    Add src1 to src2->dst
      --   100      SUB@        XXXX    Sub src1 from src2->dst
      --   101      CMP@        XXXX    Sub src1 from src2, don't store
      --   110      AND@        XX      AND src1 with src2->dst
      --   111      XOR@        XX      XOR src1 with src2->dst

  -- Immediate/Register 2-op instructions   1ooo 111 ddd  pppp
     If DBD is set, the immediate value spans two adjacent bytes, little
     endian order.  Otherwise the immediate value spans one word.  This
     instruction really looks like indirect through R7, and I suspect
     that's how the silicon implements it.

         opcode   mnemonic n/i  SZCOID  description
      --   000      n/a                 Aliases conditional branch opcodes
      --   001      MVOI    *           Move register to immediate field!
      --   010      MVII                Move immediate field to register
      --   011      ADDI        XXXX    Add src1 to src2->dst
      --   100      SUBI        XXXX    Sub src1 from src2->dst
      --   101      CMPI        XXXX    Sub src1 from src2, don't store
      --   110      ANDI        XX      AND src1 with src2->dst
      --   111      XORI        XX      XOR src1 with src2->dst

\* ======================================================================== */

#include <stdio.h>
#include <string.h>
#include "asm/frasmdat.h"
#include "asm/fragcon.h"

#define yylex lexintercept

#define JSR_RG    0x0001
#define SHF_RG    0x0002
#define IND_RG    0x0004
#define SDBD      0x0008

#define ST_REGREG 0x0001
#define ST_REGEXP 0x0002
#define ST_EXPREG 0x0004
#define ST_REGCEX 0x0008
#define ST_CEXREG 0x0010
#define ST_REG    0x0020
#define ST_EXP    0x0040
#define ST_IMP    0x0080
#define ST_EXPEXP 0x0100
    
/* ======================================================================== */
/*  R0 .. R7 can be used as general-purpose registers.                      */
/*  R0 .. R3 can be used for shifts and GSWD.                               */
/*  R1 .. R6 can be used for indirect addressing.                           */
/*  R4 .. R6 can be used for JSR.                                           */
/* ======================================================================== */
static int  reg_type[8] = 
{ 
    SHF_RG,
    SHF_RG | IND_RG,
    SHF_RG | IND_RG,
    SHF_RG | IND_RG,
    JSR_RG | IND_RG,
    JSR_RG | IND_RG,
    JSR_RG | IND_RG,
    0
};
    
/* ======================================================================== */
/*  BDEF outputs a number as a ROMW width word directly.  Allowed width is  */
/*       determined by argument #2 to the expression.                       */
/*  WDEF outputs a 16-bit word as a Double Byte Data.                       */
/* ======================================================================== */
static char genbdef[] = "[1=].[2#]I$[1=]x";
static char genwdef[] = "[1=].FF&x[1=].8}.FF&x"; 

static char gensdbd[] = "0001x";

char ignosyn[] = "[Xinvalid syntax for instruction";
char ignosel[] = "[Xinvalid operands";

long    labelloc;
static int satsub;
int ifstkpt = 0;
int fraifskip = FALSE;
int struct_locctr = -1;

extern char *proc;
extern int   proc_len;
static char  currmode[32] = "";


static int sdbd = 0, is_sdbd = 0;
static int romw = 16; 
static unsigned romm = 0xFFFF;
static int first = 1;

static int fwd_sdbd = 0;

struct symel * endsymbol = SYMNULL;

#define SDBD_CHK \
    if (sdbd) { sdbd = 0; frawarn("SDBD not allowed with this instruction."); }


#line 434 "asm/as1600.y"
typedef union {
    int intv;
    long    longv;
    char    *strng;
    struct symel *symb;
} YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		153
#define	YYFLAG		-32768
#define	YYNTBASE	65

#define YYTRANSLATE(x) ((unsigned)(x) <= 308 ? yytranslate[x] : 73)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,    61,    62,     2,     2,     2,    63,
    64,    57,    55,    59,    56,     2,    58,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    60,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
    27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
    37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     3,     5,     8,    10,    13,    16,    18,    21,    25,
    29,    32,    34,    36,    38,    42,    45,    51,    56,    64,
    71,    78,    81,    83,    86,    91,    93,    95,    98,   100,
   102,   105,   108,   111,   114,   117,   120,   124,   128,   130,
   132,   136,   138,   141,   143,   147,   149,   152,   157,   159,
   162,   167,   169,   172,   177,   183,   188,   194,   197,   202,
   207,   210,   213,   216,   219,   222,   226,   230,   234,   238,
   242,   246,   250,   254,   258,   262,   266,   270,   274,   278,
   282,   286,   289,   291,   293,   295
};

static const short yyrhs[] = {    65,
    66,     0,    66,     0,    67,    32,     0,    32,     0,     1,
    32,     0,    69,     6,     0,     6,     0,    10,    52,     0,
    69,     8,    72,     0,    69,    14,    72,     0,     9,    72,
     0,     9,     0,     5,     0,     7,     0,    69,    11,    72,
     0,    11,    72,     0,    69,    11,    72,    59,    72,     0,
    11,    72,    59,    72,     0,    69,    11,    72,    59,    72,
    59,    52,     0,    11,    72,    59,    72,    59,    52,     0,
    29,    72,    59,    72,    59,    52,     0,    69,    16,     0,
    18,     0,    18,    72,     0,    17,    52,    59,    71,     0,
    69,     0,    68,     0,    69,    70,     0,    70,     0,    51,
     0,    51,    60,     0,     4,    71,     0,    30,    71,     0,
    13,    71,     0,    15,    71,     0,    12,    72,     0,    71,
    59,    72,     0,    71,    59,    52,     0,    72,     0,    52,
     0,     0,    59,    52,     0,    52,     0,    69,    25,     0,
    26,     0,    69,    27,    72,     0,    28,     0,    24,    72,
     0,    24,    72,    59,    72,     0,    23,     0,    21,    72,
     0,    22,    72,    59,    72,     0,    19,     0,    19,    72,
     0,    19,     3,    59,    72,     0,    19,     3,    59,    61,
    72,     0,    19,    72,    59,     3,     0,    19,    61,    72,
    59,     3,     0,    19,     3,     0,    19,     3,    59,     3,
     0,    20,     3,    59,     3,     0,    55,    72,     0,    56,
    72,     0,    45,    72,     0,    38,    72,     0,    40,    72,
     0,    72,    57,    72,     0,    72,    58,    72,     0,    72,
    55,    72,     0,    72,    56,    72,     0,    72,    42,    72,
     0,    72,    47,    72,     0,    72,    48,    72,     0,    72,
    37,    72,     0,    72,    36,    72,     0,    72,    41,    72,
     0,    72,    39,    72,     0,    72,    44,    72,     0,    72,
    35,    72,     0,    72,    33,    72,     0,    72,    46,    72,
     0,    72,    49,    72,     0,    34,    53,     0,    53,     0,
    62,     0,    31,     0,    63,    72,    64,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   513,   514,   517,   521,   522,   529,   534,   538,   558,   579,
   601,   633,   654,   672,   691,   714,   730,   754,   771,   800,
   821,   840,   856,   861,   886,   953,   965,   968,   989,  1000,
  1001,  1003,  1014,  1026,  1037,  1046,  1063,  1068,  1081,  1087,
  1102,  1107,  1119,  1138,  1151,  1180,  1195,  1220,  1257,  1271,
  1287,  1312,  1323,  1336,  1351,  1366,  1381,  1416,  1428,  1441,
  1454,  1458,  1462,  1466,  1470,  1474,  1478,  1482,  1486,  1490,
  1494,  1498,  1502,  1506,  1510,  1514,  1518,  1522,  1526,  1530,
  1534,  1538,  1542,  1546,  1550,  1554
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","REGISTER",
"KOC_BDEF","KOC_ELSE","KOC_END","KOC_ENDI","KOC_EQU","KOC_IF","KOC_INCLUDE",
"KOC_ORG","KOC_RESM","KOC_SDEF","KOC_SET","KOC_WDEF","KOC_CHSET","KOC_CHDEF",
"KOC_CHUSE","KOC_opcode","KOC_opcode_i","KOC_relbr","KOC_relbr_x","KOC_SDBD",
"KOC_ROMW","KOC_PROC","KOC_ENDP","KOC_STRUCT","KOC_ENDS","KOC_MEMATTR","KOC_DDEF",
"CONSTANT","EOL","KEOP_AND","KEOP_DEFINED","KEOP_EQ","KEOP_GE","KEOP_GT","KEOP_HIGH",
"KEOP_LE","KEOP_LOW","KEOP_LT","KEOP_MOD","KEOP_MUN","KEOP_NE","KEOP_NOT","KEOP_OR",
"KEOP_SHL","KEOP_SHR","KEOP_XOR","KEOP_locctr","LABEL","STRING","SYMBOL","KTK_invalid",
"'+'","'-'","'*'","'/'","','","':'","'#'","'$'","'('","')'","file","allline",
"line","labeledline","labelcolon","genline","exprlist","expr", NULL
};
#endif

static const short yyr1[] = {     0,
    65,    65,    66,    66,    66,    67,    67,    67,    67,    67,
    67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
    67,    67,    67,    67,    67,    67,    67,    68,    68,    69,
    69,    70,    70,    70,    70,    70,    71,    71,    71,    71,
    -1,    -1,    70,    70,    70,    70,    70,    70,    70,    70,
    70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
    72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
    72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
    72,    72,    72,    72,    72,    72
};

static const short yyr2[] = {     0,
     2,     1,     2,     1,     2,     2,     1,     2,     3,     3,
     2,     1,     1,     1,     3,     2,     5,     4,     7,     6,
     6,     2,     1,     2,     4,     1,     1,     2,     1,     1,
     2,     2,     2,     2,     2,     2,     3,     3,     1,     1,
     3,     1,     2,     1,     3,     1,     2,     4,     1,     2,
     4,     1,     2,     4,     5,     4,     5,     2,     4,     4,
     2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     2,     1,     1,     1,     3
};

static const short yydefact[] = {     0,
     0,     0,    13,     7,    14,    12,     0,     0,     0,     0,
     0,     0,    23,    52,     0,     0,     0,    49,     0,    44,
    46,     0,     0,     4,    30,     0,     2,     0,    27,    26,
    29,     5,    85,     0,     0,     0,     0,    40,    83,     0,
     0,    84,     0,    32,    39,    11,     8,    16,    36,    34,
    35,     0,    24,    58,     0,    53,     0,    50,     0,    47,
     0,    33,    31,     1,     3,     6,     0,     0,     0,    22,
    43,     0,     0,    28,    82,    64,    65,    63,    61,    62,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     9,    15,    10,
    45,    86,    38,    37,    79,    78,    74,    73,    76,    75,
    70,    77,    80,    71,    72,    81,    68,    69,    66,    67,
    18,    25,    59,     0,    54,     0,    56,    60,    51,    48,
     0,     0,     0,    55,    57,     0,    17,    20,    21,     0,
    19,     0,     0
};

static const short yydefgoto[] = {    26,
    27,    28,    29,    30,    31,    44,    45
};

static const short yypact[] = {   203,
   -31,   -12,-32768,-32768,-32768,   257,   -47,   257,   257,   -12,
   -12,   -43,   257,    71,     9,   257,   257,-32768,   257,-32768,
-32768,   257,   -12,-32768,   -42,   168,-32768,   -15,-32768,   236,
-32768,-32768,-32768,   -33,   257,   257,   257,-32768,-32768,   257,
   257,-32768,   257,   -38,   584,   584,-32768,   314,   584,   -38,
   -38,   -36,   584,   -35,   257,   341,   -34,   584,   368,   395,
   422,   -38,-32768,-32768,-32768,-32768,   257,   257,   257,-32768,
-32768,   257,   -21,-32768,-32768,   584,   584,   634,-32768,-32768,
   288,   237,   257,   257,   257,   257,   257,   257,   257,   257,
   257,   257,   257,   257,   257,   257,   257,   257,   257,   -12,
   104,   449,    29,    33,   257,   257,   257,   584,   476,   584,
   584,-32768,-32768,   584,   634,   658,   658,   658,   658,   658,
-32768,   658,   610,-32768,-32768,   610,     0,     0,-32768,-32768,
   503,   -38,-32768,   257,   584,    35,-32768,-32768,   584,   584,
   530,   257,   -13,   584,-32768,    -7,   557,-32768,-32768,     1,
-32768,    46,-32768
};

static const short yypgoto[] = {-32768,
    26,-32768,-32768,    24,    25,     4,    -6
};


#define	YYLAST		716


static const short yytable[] = {    46,
    32,    48,    49,    71,    47,    72,    53,    56,    52,    58,
    59,    57,    60,    50,    51,    61,    65,    63,    33,    75,
    82,    34,   100,   101,   104,    35,    62,    36,    76,    77,
    78,   137,    37,    79,    80,   138,    81,   145,   148,    38,
    39,    89,    40,    41,   149,   153,    92,    93,   102,    42,
    43,    64,   151,    73,    74,     0,    97,    98,     0,     0,
   108,   109,   110,     0,     0,   111,     0,     0,     0,     0,
     0,     0,     0,    54,     0,   114,   115,   116,   117,   118,
   119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
   129,   130,   131,     0,   135,     0,     0,     0,   139,   140,
   141,    33,     0,   132,    34,     0,   133,     0,    35,     0,
    36,     0,     0,     0,     0,    37,     0,     0,     0,     0,
     0,     0,     0,    39,     0,    40,    41,   144,     0,     0,
     0,    55,    42,    43,    33,   147,     0,    34,     0,     0,
     0,    35,     0,    36,     0,     0,     0,     0,    37,     0,
     0,     0,     0,     0,     0,     0,    39,     0,    40,    41,
     0,     0,     0,     0,   134,    42,    43,   152,     1,     0,
     0,     2,     3,     4,     5,     0,     6,     7,     8,     9,
    10,     0,    11,     0,    12,    13,    14,    15,    16,    17,
    18,    19,     0,    20,     0,    21,    22,    23,     0,    24,
     0,     0,     0,     1,     0,     0,     2,     3,     4,     5,
     0,     6,     7,     8,     9,    10,     0,    11,    25,    12,
    13,    14,    15,    16,    17,    18,    19,     0,    20,     0,
    21,    22,    23,     0,    24,     0,     0,     0,     0,     2,
     0,    66,     0,    67,     0,     0,    68,     9,    10,    69,
    11,    70,     0,    25,    14,    15,    16,    17,    18,    19,
    71,    20,    72,    21,     0,    23,     0,    33,     0,     0,
    34,     0,     0,     0,    35,     0,    36,     0,     0,     0,
     0,    37,     0,     0,     0,     0,    25,    33,   113,    39,
    34,    40,    41,     0,    35,     0,    36,     0,    42,    43,
     0,    37,     0,     0,     0,     0,     0,     0,     0,    39,
     0,    40,    41,     0,     0,     0,     0,     0,    42,    43,
    83,     0,    84,    85,    86,     0,    87,     0,    88,    89,
     0,    90,     0,    91,    92,    93,    94,     0,     0,     0,
     0,     0,    95,    96,    97,    98,    83,     0,    84,    85,
    86,   112,    87,     0,    88,    89,     0,    90,     0,    91,
    92,    93,    94,     0,     0,     0,     0,     0,    95,    96,
    97,    98,    99,    83,     0,    84,    85,    86,     0,    87,
     0,    88,    89,     0,    90,     0,    91,    92,    93,    94,
     0,     0,     0,     0,     0,    95,    96,    97,    98,   103,
    83,     0,    84,    85,    86,     0,    87,     0,    88,    89,
     0,    90,     0,    91,    92,    93,    94,     0,     0,     0,
     0,     0,    95,    96,    97,    98,   105,    83,     0,    84,
    85,    86,     0,    87,     0,    88,    89,     0,    90,     0,
    91,    92,    93,    94,     0,     0,     0,     0,     0,    95,
    96,    97,    98,   106,    83,     0,    84,    85,    86,     0,
    87,     0,    88,    89,     0,    90,     0,    91,    92,    93,
    94,     0,     0,     0,     0,     0,    95,    96,    97,    98,
   107,    83,     0,    84,    85,    86,     0,    87,     0,    88,
    89,     0,    90,     0,    91,    92,    93,    94,     0,     0,
     0,     0,     0,    95,    96,    97,    98,   136,    83,     0,
    84,    85,    86,     0,    87,     0,    88,    89,     0,    90,
     0,    91,    92,    93,    94,     0,     0,     0,     0,     0,
    95,    96,    97,    98,   142,    83,     0,    84,    85,    86,
     0,    87,     0,    88,    89,     0,    90,     0,    91,    92,
    93,    94,     0,     0,     0,     0,     0,    95,    96,    97,
    98,   143,    83,     0,    84,    85,    86,     0,    87,     0,
    88,    89,     0,    90,     0,    91,    92,    93,    94,     0,
     0,     0,     0,     0,    95,    96,    97,    98,   146,    83,
     0,    84,    85,    86,     0,    87,     0,    88,    89,     0,
    90,     0,    91,    92,    93,    94,     0,     0,     0,     0,
     0,    95,    96,    97,    98,   150,    83,     0,    84,    85,
    86,     0,    87,     0,    88,    89,     0,    90,     0,    91,
    92,    93,    94,     0,     0,     0,     0,     0,    95,    96,
    97,    98,    83,     0,    84,    85,    86,     0,    87,     0,
    88,    89,     0,    90,     0,     0,    92,    93,     0,     0,
     0,     0,     0,     0,    95,    96,    97,    98,    84,    85,
    86,     0,    87,     0,    88,    89,     0,    90,     0,     0,
    92,    93,     0,     0,     0,     0,     0,     0,    95,    96,
    97,    98,-32768,-32768,-32768,     0,-32768,     0,-32768,    89,
     0,-32768,     0,     0,    92,    93,     0,     0,     0,     0,
     0,     0,    95,    96,    97,    98
};

static const short yycheck[] = {     6,
    32,     8,     9,    25,    52,    27,    13,    14,    52,    16,
    17,     3,    19,    10,    11,    22,    32,    60,    31,    53,
    59,    34,    59,    59,    59,    38,    23,    40,    35,    36,
    37,     3,    45,    40,    41,     3,    43,     3,    52,    52,
    53,    42,    55,    56,    52,     0,    47,    48,    55,    62,
    63,    26,    52,    30,    30,    -1,    57,    58,    -1,    -1,
    67,    68,    69,    -1,    -1,    72,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,     3,    -1,    82,    83,    84,    85,    86,
    87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
    97,    98,    99,    -1,   101,    -1,    -1,    -1,   105,   106,
   107,    31,    -1,   100,    34,    -1,     3,    -1,    38,    -1,
    40,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    53,    -1,    55,    56,   134,    -1,    -1,
    -1,    61,    62,    63,    31,   142,    -1,    34,    -1,    -1,
    -1,    38,    -1,    40,    -1,    -1,    -1,    -1,    45,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,
    -1,    -1,    -1,    -1,    61,    62,    63,     0,     1,    -1,
    -1,     4,     5,     6,     7,    -1,     9,    10,    11,    12,
    13,    -1,    15,    -1,    17,    18,    19,    20,    21,    22,
    23,    24,    -1,    26,    -1,    28,    29,    30,    -1,    32,
    -1,    -1,    -1,     1,    -1,    -1,     4,     5,     6,     7,
    -1,     9,    10,    11,    12,    13,    -1,    15,    51,    17,
    18,    19,    20,    21,    22,    23,    24,    -1,    26,    -1,
    28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,     4,
    -1,     6,    -1,     8,    -1,    -1,    11,    12,    13,    14,
    15,    16,    -1,    51,    19,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    -1,    30,    -1,    31,    -1,    -1,
    34,    -1,    -1,    -1,    38,    -1,    40,    -1,    -1,    -1,
    -1,    45,    -1,    -1,    -1,    -1,    51,    31,    52,    53,
    34,    55,    56,    -1,    38,    -1,    40,    -1,    62,    63,
    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,    62,    63,
    33,    -1,    35,    36,    37,    -1,    39,    -1,    41,    42,
    -1,    44,    -1,    46,    47,    48,    49,    -1,    -1,    -1,
    -1,    -1,    55,    56,    57,    58,    33,    -1,    35,    36,
    37,    64,    39,    -1,    41,    42,    -1,    44,    -1,    46,
    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    56,
    57,    58,    59,    33,    -1,    35,    36,    37,    -1,    39,
    -1,    41,    42,    -1,    44,    -1,    46,    47,    48,    49,
    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,
    33,    -1,    35,    36,    37,    -1,    39,    -1,    41,    42,
    -1,    44,    -1,    46,    47,    48,    49,    -1,    -1,    -1,
    -1,    -1,    55,    56,    57,    58,    59,    33,    -1,    35,
    36,    37,    -1,    39,    -1,    41,    42,    -1,    44,    -1,
    46,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,
    56,    57,    58,    59,    33,    -1,    35,    36,    37,    -1,
    39,    -1,    41,    42,    -1,    44,    -1,    46,    47,    48,
    49,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
    59,    33,    -1,    35,    36,    37,    -1,    39,    -1,    41,
    42,    -1,    44,    -1,    46,    47,    48,    49,    -1,    -1,
    -1,    -1,    -1,    55,    56,    57,    58,    59,    33,    -1,
    35,    36,    37,    -1,    39,    -1,    41,    42,    -1,    44,
    -1,    46,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,
    55,    56,    57,    58,    59,    33,    -1,    35,    36,    37,
    -1,    39,    -1,    41,    42,    -1,    44,    -1,    46,    47,
    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,
    58,    59,    33,    -1,    35,    36,    37,    -1,    39,    -1,
    41,    42,    -1,    44,    -1,    46,    47,    48,    49,    -1,
    -1,    -1,    -1,    -1,    55,    56,    57,    58,    59,    33,
    -1,    35,    36,    37,    -1,    39,    -1,    41,    42,    -1,
    44,    -1,    46,    47,    48,    49,    -1,    -1,    -1,    -1,
    -1,    55,    56,    57,    58,    59,    33,    -1,    35,    36,
    37,    -1,    39,    -1,    41,    42,    -1,    44,    -1,    46,
    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    56,
    57,    58,    33,    -1,    35,    36,    37,    -1,    39,    -1,
    41,    42,    -1,    44,    -1,    -1,    47,    48,    -1,    -1,
    -1,    -1,    -1,    -1,    55,    56,    57,    58,    35,    36,
    37,    -1,    39,    -1,    41,    42,    -1,    44,    -1,    -1,
    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,    55,    56,
    57,    58,    35,    36,    37,    -1,    39,    -1,    41,    42,
    -1,    44,    -1,    -1,    47,    48,    -1,    -1,    -1,    -1,
    -1,    -1,    55,    56,    57,    58
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/lib/bison.simple"
/* This file comes from bison-1.27.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 216 "/usr/lib/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 3:
#line 518 "asm/as1600.y"
{
                clrexpr();
            ;
    break;}
case 5:
#line 523 "asm/as1600.y"
{
                clrexpr();
                yyerrok;
            ;
    break;}
case 6:
#line 530 "asm/as1600.y"
{
                endsymbol = yyvsp[-1].symb;
                nextreadact = Nra_end;
            ;
    break;}
case 7:
#line 535 "asm/as1600.y"
{
                nextreadact = Nra_end;
            ;
    break;}
case 8:
#line 539 "asm/as1600.y"
{
                if(nextfstk >= FILESTKDPTH)
                {
                    fraerror("include file nesting limit exceeded");
                }
                else
                {
                    infilestk[nextfstk].fnm = savestring(yyvsp[0].strng,strlen(yyvsp[0].strng));
                    if( (infilestk[nextfstk].fpt = path_fopen(yyvsp[0].strng,"r"))
                        ==(FILE *)NULL )
                    {
                        fraerror("cannot open include file");
                    }
                    else
                    {
                        nextreadact = Nra_new;
                    }
                }
            ;
    break;}
case 9:
#line 559 "asm/as1600.y"
{
                if(yyvsp[-2].symb->seg == SSG_UNDEF)
                {
                    pevalexpr(0, yyvsp[0].intv);
                    if(evalr[0].seg == SSG_ABS)
                    {
                        yyvsp[-2].symb->seg = SSG_EQU;
                        yyvsp[-2].symb->value = evalr[0].value;
                        prtequvalue("C: 0x%lx\n", evalr[0].value);
                    }
                    else
                    {
                        fraerror("noncomputable expression for EQU");
                    }
                }
                else
                {
                    fraerror("cannot change symbol value with EQU");
                }
            ;
    break;}
case 10:
#line 580 "asm/as1600.y"
{
                if(yyvsp[-2].symb->seg == SSG_UNDEF
                   || yyvsp[-2].symb->seg == SSG_SET)
                {
                    pevalexpr(0, yyvsp[0].intv);
                    if(evalr[0].seg == SSG_ABS)
                    {
                        yyvsp[-2].symb->seg = SSG_SET;
                        yyvsp[-2].symb->value = evalr[0].value;
                        prtequvalue("C: 0x%lx\n", evalr[0].value);
                    }
                    else
                    {
                        fraerror("noncomputable expression for SET");
                    }
                }
                else
                {
                    fraerror("cannot change symbol value with SET");
                }
            ;
    break;}
case 11:
#line 602 "asm/as1600.y"
{
                if((++ifstkpt) < IFSTKDEPTH)
                {
                    pevalexpr(0, yyvsp[0].intv);
                    if(evalr[0].seg == SSG_ABS)
                    {
                        if(evalr[0].value != 0)
                        {
                            elseifstk[ifstkpt] = If_Skip;
                            endifstk[ifstkpt] = If_Active;
                        }
                        else
                        {
                            fraifskip = TRUE;
                            elseifstk[ifstkpt] = If_Active;
                            endifstk[ifstkpt] = If_Active;
                        }
                    }
                    else
                    {
                        fraifskip = TRUE;
                        elseifstk[ifstkpt] = If_Active;
                        endifstk[ifstkpt] = If_Active;
                    }
                }
                else
                {
                    fraerror("IF stack overflow");
                }
            ;
    break;}
case 12:
#line 634 "asm/as1600.y"
{
                if(fraifskip) 
                {
                    if((++ifstkpt) < IFSTKDEPTH)
                    {
                            elseifstk[ifstkpt] = If_Skip;
                            endifstk[ifstkpt] = If_Skip;
                    }
                    else
                    {
                        fraerror("IF stack overflow");
                    }
                }
                else
                {
                    yyerror("syntax error");
                    YYERROR;
                }
            ;
    break;}
case 13:
#line 655 "asm/as1600.y"
{
                switch(elseifstk[ifstkpt])
                {
                case If_Active:
                    fraifskip = FALSE;
                    break;
                
                case If_Skip:
                    fraifskip = TRUE;
                    break;
                
                case If_Err:
                    fraerror("ELSE with no matching if");
                    break;
                }
            ;
    break;}
case 14:
#line 673 "asm/as1600.y"
{
                switch(endifstk[ifstkpt])
                {
                case If_Active:
                    fraifskip = FALSE;
                    ifstkpt--;
                    break;
                
                case If_Skip:
                    fraifskip = TRUE;
                    ifstkpt--;
                    break;
                
                case If_Err:
                    fraerror("ENDI with no matching if");
                    break;
                }
            ;
    break;}
case 15:
#line 692 "asm/as1600.y"
{
                pevalexpr(0, yyvsp[0].intv);
                if(evalr[0].seg == SSG_ABS)
                {
                    locctr   = 2 * (labelloc = evalr[0].value);
                    currseg  = 0;
                    strcpy(currmode,"+R");
                    if(yyvsp[-2].symb->seg == SSG_UNDEF)
                    {
                        yyvsp[-2].symb->seg = SSG_ABS;
                        yyvsp[-2].symb->value = labelloc;
                    }
                    else
                        fraerror( "multiple definition of label");

                    prtequvalue("C: 0x%lx\n", evalr[0].value);
                }
                else
                {
                    fraerror( "noncomputable expression for ORG");
                }
            ;
    break;}
case 16:
#line 715 "asm/as1600.y"
{
                pevalexpr(0, yyvsp[0].intv);
                if(evalr[0].seg == SSG_ABS)
                {
                    locctr   = 2 * (labelloc = evalr[0].value);
                    currseg  = 0;
                    strcpy(currmode,"+R");
                    prtequvalue("C: 0x%lx\n", evalr[0].value);
                }
                else
                {
                    fraerror(
                     "noncomputable expression for ORG");
                }
            ;
    break;}
case 17:
#line 731 "asm/as1600.y"
{
                pevalexpr(0, yyvsp[-2].intv);
                pevalexpr(1, yyvsp[0].intv);
                if(evalr[0].seg == SSG_ABS && evalr[1].seg == SSG_ABS)
                {
                    locctr   = 2 * (labelloc = evalr[0].value);
                    currseg  = (evalr[1].value - labelloc);
                    strcpy(currmode, currseg ? "" : "+R");
                    if(yyvsp[-4].symb->seg == SSG_UNDEF)
                    {
                        yyvsp[-4].symb->seg = SSG_ABS;
                        yyvsp[-4].symb->value = labelloc;
                    }
                    else
                        fraerror( "multiple definition of label");

                    prtequvalue("C: 0x%lx\n", evalr[0].value);
                }
                else
                {
                    fraerror( "noncomputable expression for ORG");
                }
            ;
    break;}
case 18:
#line 755 "asm/as1600.y"
{
                pevalexpr(0, yyvsp[-2].intv);
                pevalexpr(1, yyvsp[0].intv);
                if(evalr[0].seg == SSG_ABS && evalr[1].seg == SSG_ABS)
                {
                    locctr   = 2 * (labelloc = evalr[0].value);
                    currseg  = (evalr[1].value - labelloc);
                    strcpy(currmode, currseg ? "" : "+R");
                    prtequvalue("C: 0x%lx\n", evalr[0].value);
                }
                else
                {
                    fraerror(
                     "noncomputable expression for ORG");
                }
            ;
    break;}
case 19:
#line 772 "asm/as1600.y"
{
                pevalexpr(0, yyvsp[-4].intv);
                pevalexpr(1, yyvsp[-2].intv);
                if(evalr[0].seg == SSG_ABS && evalr[1].seg == SSG_ABS)
                {
                    char *s = yyvsp[0].strng;
                    if (strlen(s) > 30)
                        fraerror("Mode string is too long (max 30 chars)\n");
                    strcpy(currmode, s);

                    locctr   = 2 * (labelloc = evalr[0].value);
                    currseg  = (evalr[1].value - labelloc);

                    if(yyvsp[-6].symb->seg == SSG_UNDEF)
                    {
                        yyvsp[-6].symb->seg = SSG_ABS;
                        yyvsp[-6].symb->value = labelloc;
                    }
                    else
                        fraerror( "multiple definition of label");

                    prtequvalue("C: 0x%lx\n", evalr[0].value);
                }
                else
                {
                    fraerror( "noncomputable expression for ORG");
                }
            ;
    break;}
case 20:
#line 801 "asm/as1600.y"
{
                pevalexpr(0, yyvsp[-4].intv);
                pevalexpr(1, yyvsp[-2].intv);
                if(evalr[0].seg == SSG_ABS && evalr[1].seg == SSG_ABS)
                {
                    char *s = yyvsp[0].strng;
                    if (strlen(s) > 30)
                        fraerror("Mode string is too long (max 30 chars)\n");
                    strcpy(currmode, s);

                    locctr   = 2 * (labelloc = evalr[0].value);
                    currseg  = (evalr[1].value - labelloc);
                    prtequvalue("C: 0x%lx\n", evalr[0].value);
                }
                else
                {
                    fraerror(
                     "noncomputable expression for ORG");
                }
            ;
    break;}
case 21:
#line 822 "asm/as1600.y"
{
                pevalexpr(0, yyvsp[-4].intv);
                pevalexpr(1, yyvsp[-2].intv);
                if(evalr[0].seg == SSG_ABS && evalr[1].seg == SSG_ABS)
                {
                    char *s = yyvsp[0].strng;
                    if (strlen(s) > 30)
                        fraerror("Mode string is too long (max 30 chars)\n");

                    genmarec(evalr[0].value, evalr[1].value, s);
                }
                else
                {
                    fraerror(
                     "noncomputable expression for MEMATTR");
                }
            ;
    break;}
case 22:
#line 841 "asm/as1600.y"
{
                if(yyvsp[-1].symb->seg == SSG_UNDEF)
                {
                    yyvsp[-1].symb->seg = SSG_EQU;
                    if( (yyvsp[-1].symb->value = chtcreate()) <= 0)
                    {
                        fraerror("cannot create character translation table");
                    }
                    prtequvalue("C: 0x%lx\n", yyvsp[-1].symb->value);
                }
                else
                {
                    fraerror("multiple definition of label");
                }
            ;
    break;}
case 23:
#line 857 "asm/as1600.y"
{
                chtcpoint = (int *) NULL;
                prtequvalue("C: 0x%lx\n", 0L);
            ;
    break;}
case 24:
#line 862 "asm/as1600.y"
{
                pevalexpr(0, yyvsp[0].intv);
                if( evalr[0].seg == SSG_ABS)
                {
                    if( evalr[0].value == 0)
                    {
                        chtcpoint = (int *)NULL;
                        prtequvalue("C: 0x%lx\n", 0L);
                    }
                    else if(evalr[0].value < chtnxalph)
                    {
                        chtcpoint = chtatab[evalr[0].value];
                        prtequvalue("C: 0x%lx\n", evalr[0].value);
                    }
                    else
                    {
                        fraerror("nonexistent character translation table");
                    }
                }
                else
                {
                    fraerror("noncomputable expression");
                }
            ;
    break;}
case 25:
#line 887 "asm/as1600.y"
{
                int findrv, numret, *charaddr;
                char *sourcestr = yyvsp[-2].strng, *before;

                if(chtnpoint != (int *)NULL)
                {
                    for(satsub = 0; satsub < yyvsp[0].intv; satsub++)
                    {
                        before = sourcestr;

                        pevalexpr(0, exprlist[satsub]);
                        findrv = chtcfind(chtnpoint, &sourcestr,
                                &charaddr, &numret);
                        if(findrv == CF_END)
                        {
                            fraerror("more expressions than characters");
                            break;
                        }

                        if(evalr[0].seg == SSG_ABS)
                        {
                            switch(findrv)
                            {
                            case CF_UNDEF:
                                {
                        if(evalr[0].value < 0 ||
                            evalr[0].value > 255)
                        {
                            frawarn("character translation "
                                "value truncated");
                        }
                        *charaddr = evalr[0].value & 0xff;
                        prtequvalue("C: 0x%lx\n", evalr[0].value);
                                }
                                break;

                            case CF_INVALID:
                            case CF_NUMBER:
                                fracherror("invalid character "
                                       "to define", 
                                       before, sourcestr);
                                break;

                            case CF_CHAR:
                                fracherror("character already "
                                       "defined", 
                                       before, sourcestr);
                                break;
                            }
                        }
                        else
                        {
                            fraerror("noncomputable expression");
                        }
                    }

                    if( *sourcestr != '\0')
                    {
                        fraerror("more characters than expressions");
                    }
                }
                else
                {
                    fraerror("no CHARSET statement active");
                }
            ;
    break;}
case 26:
#line 954 "asm/as1600.y"
{
                if(yyvsp[0].symb->seg == SSG_UNDEF)
                {
                    yyvsp[0].symb->seg = SSG_ABS;
                    yyvsp[0].symb->value = labelloc;
                    prtequvalue("C: 0x%lx\n", labelloc);

                }
                else
                    fraerror("multiple definition of label");
            ;
    break;}
case 28:
#line 969 "asm/as1600.y"
{
                if (sdbd)
                    frawarn("label between SDBD and instruction");

                if(yyvsp[-1].symb->seg == SSG_UNDEF)
                {
                    yyvsp[-1].symb->seg   = SSG_ABS;
                    yyvsp[-1].symb->value = labelloc;
                }
                else
                    fraerror("multiple definition of label");

                if (locctr & 1) fraerror("internal error: PC misaligned.");

                labelloc = locctr >> 1;

                sdbd    = is_sdbd;
                is_sdbd = 0;
                first   = 0;
            ;
    break;}
case 29:
#line 990 "asm/as1600.y"
{
                if (locctr & 1) fraerror("internal error: PC misaligned.");
                labelloc = locctr >> 1;

                sdbd    = is_sdbd;
                is_sdbd = 0;
                first   = 0;
            ;
    break;}
case 32:
#line 1004 "asm/as1600.y"
{
                genlocrec(currseg, labelloc, TYPE_DATA, currmode);
                evalr[2].seg   = SSG_ABS;
                evalr[2].value = 8;
                for( satsub = 0; satsub < yyvsp[0].intv; satsub++)
                {
                    pevalexpr(1, exprlist[satsub]);
                    locctr += geninstr(genbdef);
                }
            ;
    break;}
case 33:
#line 1015 "asm/as1600.y"
{
                genlocrec(currseg, labelloc, TYPE_DATA, currmode);
                evalr[2].seg   = SSG_ABS;
                evalr[2].value = romw;
                for( satsub = 0; satsub < yyvsp[0].intv; satsub++)
                {
                    pevalexpr(1, exprlist[satsub]);
                    locctr += geninstr(genbdef);
                }
            ;
    break;}
case 34:
#line 1027 "asm/as1600.y"
{
                genlocrec(currseg, labelloc, TYPE_STRING, currmode);
                evalr[2].seg   = SSG_ABS;
                evalr[2].value = romw;
                for( satsub = 0; satsub < yyvsp[0].intv; satsub++)
                {
                    pevalexpr(1, exprlist[satsub]);
                    locctr += geninstr(genbdef);
                }
            ;
    break;}
case 35:
#line 1038 "asm/as1600.y"
{
                genlocrec(currseg, labelloc, TYPE_DBDATA|TYPE_DATA, currmode);
                for( satsub = 0; satsub < yyvsp[0].intv; satsub++)
                {
                    pevalexpr(1, exprlist[satsub]);
                    locctr += geninstr(genwdef);
                }
            ;
    break;}
case 36:
#line 1047 "asm/as1600.y"
{
                pevalexpr(0, yyvsp[0].intv);
                if(evalr[0].seg == SSG_ABS)
                {
                    locctr = 2 * (labelloc + evalr[0].value);
                    prtequvalue("C: 0x%lx\n", labelloc);
                    genlocrec(currseg, labelloc, TYPE_HOLE, currmode);
                    genrsrv(labelloc + evalr[0].value);
                }
                else
                {
                    fraerror("noncomputable result for RMB expression");
                }
            ;
    break;}
case 37:
#line 1064 "asm/as1600.y"
{
                exprlist[nextexprs++] = yyvsp[0].intv;
                yyval.intv = nextexprs;
            ;
    break;}
case 38:
#line 1069 "asm/as1600.y"
{
                char *s = yyvsp[0].strng;
                long accval = 0;

                while (*s)
                {
                    accval = chtran(&s);
                    exprlist[nextexprs++] = 
                        exprnode(PCCASE_CONS,0,IGP_CONSTANT,0,accval,SYMNULL);
                }
                yyval.intv = nextexprs;
            ;
    break;}
case 39:
#line 1082 "asm/as1600.y"
{
                nextexprs = 0;
                exprlist[nextexprs++] = yyvsp[0].intv;
                yyval.intv = nextexprs;
            ;
    break;}
case 40:
#line 1088 "asm/as1600.y"
{
                char *s = yyvsp[0].strng;
                long accval = 0;

                while (*s)
                {
                    accval = chtran(&s);
                    exprlist[nextexprs++] = 
                        exprnode(PCCASE_CONS,0,IGP_CONSTANT,0,accval,SYMNULL);
                }
                yyval.intv = nextexprs;
            ;
    break;}
case 41:
#line 1103 "asm/as1600.y"
{
                stringlist[nextstrs++] = yyvsp[0].strng;
                yyval.intv = nextstrs;
            ;
    break;}
case 42:
#line 1108 "asm/as1600.y"
{
                nextstrs = 0;
                stringlist[nextstrs++] = yyvsp[0].strng;
                yyval.intv = nextstrs;
            ;
    break;}
case 43:
#line 1120 "asm/as1600.y"
{
                if (proc)
                    fraerror("Nested procedures/structures are not allowed.");

                proc     = strdup(yyvsp[-1].symb->symstr);
                proc_len = strlen(proc);

                if(yyvsp[-1].symb->seg == SSG_UNDEF)
                {
                    yyvsp[-1].symb->seg   = SSG_ABS;
                    yyvsp[-1].symb->value = labelloc;
                    prtequvalue("C: 0x%lx\n", labelloc);

                }
                else
                    fraerror("multiple definition of label");
            ;
    break;}
case 44:
#line 1139 "asm/as1600.y"
{
                if (!proc || struct_locctr != -1)
                    fraerror("ENDP w/out PROC.");

                free(proc);
                proc     = NULL;
                proc_len = 0;
            ;
    break;}
case 45:
#line 1152 "asm/as1600.y"
{
                if (proc)
                    fraerror("Nested procedures/structures are not allowed.");

                proc     = strdup(yyvsp[-2].symb->symstr);
                proc_len = strlen(proc);
                struct_locctr = locctr;

                pevalexpr(0, yyvsp[0].intv);
                if(evalr[0].seg == SSG_ABS)
                {
                    locctr = 2 * (labelloc = evalr[0].value);
                    if(yyvsp[-2].symb->seg == SSG_UNDEF)
                    {
                        yyvsp[-2].symb->seg = SSG_ABS;
                        yyvsp[-2].symb->value = labelloc;
                    }
                    else
                        fraerror( "multiple definition of label");

                    prtequvalue("C: 0x%lx\n", evalr[0].value);
                }
                else
                {
                    fraerror( "noncomputable expression for ORG");
                }
            ;
    break;}
case 46:
#line 1181 "asm/as1600.y"
{
                if (!proc || struct_locctr == -1)
                    fraerror("ENDS w/out STRUCT.");

                free(proc);
                proc     = NULL;
                proc_len = 0;
                locctr = struct_locctr;
                struct_locctr = -1;
            ;
    break;}
case 47:
#line 1196 "asm/as1600.y"
{
                genlocrec(currseg, labelloc, TYPE_HOLE, currmode);
                pevalexpr(0, yyvsp[0].intv);
                if(evalr[0].seg == SSG_ABS)
                {
                    romw = evalr[0].value;

                    if (romw < 8 || romw > 16)
                        fraerror("ROMWIDTH out of range");

                    romm = 0xFFFFU >> (16 - romw);
                }
                else
                {
                    fraerror("noncomputable expression for ROMWIDTH");
                }

                if (!first)
                {
                    frawarn("Code appears before ROMW directive.");
                }

                fwd_sdbd = 0;
            ;
    break;}
case 48:
#line 1221 "asm/as1600.y"
{
                genlocrec(currseg, labelloc, TYPE_HOLE, currmode);
                pevalexpr(0, yyvsp[-2].intv);
                pevalexpr(1, yyvsp[0].intv);
                if(evalr[0].seg == SSG_ABS)
                {
                    romw = evalr[0].value;

                    if (romw < 8 || romw > 16)
                        fraerror("ROMWIDTH out of range");

                    romm = 0xFFFFU >> (16 - romw);
                }
                else
                    fraerror("noncomputable expression for ROMWIDTH");

                if (!first)
                {
                    frawarn("Code appears before ROMW directive.");
                }

                if (evalr[1].seg == SSG_ABS)
                {
                    fwd_sdbd = evalr[1].value;

                    if (fwd_sdbd > 1 || fwd_sdbd < 0)
                        fraerror("SDBD mode flag must be 0 or 1.");
                } else
                    fraerror("noncomputable expression for ROMWIDTH");

            ;
    break;}
case 49:
#line 1258 "asm/as1600.y"
{
                if (sdbd)
                    frawarn("Two SDBDs in a row.");

                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                locctr += geninstr(findgen(yyvsp[0].intv, ST_IMP, 0));
                is_sdbd = SDBD;
            ;
    break;}
case 50:
#line 1272 "asm/as1600.y"
{
                unsigned rel_addr = labelloc + 2;
                int dir;

                SDBD_CHK

                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                pevalexpr(1, yyvsp[0].intv);

                evalr[3].seg   = SSG_ABS;
                evalr[3].value = romw;

                locctr += geninstr(findgen(yyvsp[-1].intv, ST_EXP, sdbd));
            ;
    break;}
case 51:
#line 1288 "asm/as1600.y"
{
                unsigned rel_addr = labelloc + 2;
                int dir;

                SDBD_CHK

                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                pevalexpr(1, yyvsp[-2].intv);
                pevalexpr(4, yyvsp[0].intv);

                if (evalr[4].seg != SSG_ABS)
                    fraerror("Must have constant expr for BEXT condition");

                evalr[3].seg   = SSG_ABS;
                evalr[3].value = romw;

                locctr += geninstr(findgen(yyvsp[-3].intv, ST_EXPEXP, sdbd));
            ;
    break;}
case 52:
#line 1313 "asm/as1600.y"
{
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                locctr += geninstr(findgen(yyvsp[0].intv, ST_IMP, sdbd));
            ;
    break;}
case 53:
#line 1324 "asm/as1600.y"
{
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                pevalexpr(1, yyvsp[0].intv);
                locctr += geninstr(findgen(yyvsp[-1].intv, ST_EXP, sdbd));
            ;
    break;}
case 54:
#line 1337 "asm/as1600.y"
{
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                evalr[1].value = yyvsp[-2].intv;
                pevalexpr(2, yyvsp[0].intv);
                evalr[3].seg    = SSG_ABS;
                evalr[3].value  = romw;
                locctr += geninstr(findgen(yyvsp[-3].intv, ST_REGEXP, reg_type[yyvsp[-2].intv]|sdbd));
            ;
    break;}
case 55:
#line 1352 "asm/as1600.y"
{
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                evalr[1].value  = yyvsp[-3].intv;
                evalr[3].seg    = SSG_ABS;
                evalr[3].value  = romw;
                pevalexpr(2, yyvsp[0].intv);
                locctr += geninstr(findgen(yyvsp[-4].intv, ST_REGCEX, reg_type[yyvsp[-3].intv]|sdbd));
            ;
    break;}
case 56:
#line 1367 "asm/as1600.y"
{
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                pevalexpr(1, yyvsp[-2].intv);
                evalr[2].value = yyvsp[0].intv;
                evalr[3].seg   = SSG_ABS;
                evalr[3].value = romw;
                locctr += geninstr(findgen(yyvsp[-3].intv, ST_EXPREG, reg_type[yyvsp[0].intv]|sdbd));
            ;
    break;}
case 57:
#line 1382 "asm/as1600.y"
{
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                pevalexpr(1, yyvsp[-2].intv);
                evalr[2].value = yyvsp[0].intv;

                evalr[3].seg   = SSG_ABS;
                evalr[3].value = romw;

                if (sdbd == 0 && romw != 16)
                {
                    if (evalr[1].seg == SSG_ABS && 
                        (0xFFFF & evalr[1].value & ~romm) != 0)
                    {
                        /*frawarn("Constant is wider than ROM width.  "
                                "Inserting SDBD.");*/
                        locctr += geninstr("0001x");
                        sdbd = SDBD;
                    }

                    if (evalr[1].seg != SSG_ABS && fwd_sdbd)
                    {   
                        frawarn("Inserting SDBD due to forward reference.");
                        locctr += geninstr("0001x");
                        sdbd = SDBD;
                    }
                }

                locctr += geninstr(findgen(yyvsp[-4].intv, ST_CEXREG, reg_type[yyvsp[0].intv]|sdbd));
            ;
    break;}
case 58:
#line 1417 "asm/as1600.y"
{
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                evalr[1].value = yyvsp[0].intv;
                locctr += geninstr(findgen(yyvsp[-1].intv, ST_REG, reg_type[yyvsp[0].intv]|sdbd));
            ;
    break;}
case 59:
#line 1429 "asm/as1600.y"
{
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                evalr[1].value = yyvsp[-2].intv;
                evalr[2].value = yyvsp[0].intv;
                locctr += geninstr(findgen(yyvsp[-3].intv, ST_REGREG, reg_type[yyvsp[-2].intv]|sdbd));
            ;
    break;}
case 60:
#line 1442 "asm/as1600.y"
{
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                evalr[1].value = yyvsp[-2].intv;
                evalr[2].value = yyvsp[0].intv;
                locctr += geninstr(findgen(yyvsp[-3].intv, ST_REGREG, reg_type[yyvsp[-2].intv]|sdbd));
            ;
    break;}
case 61:
#line 1455 "asm/as1600.y"
{
                yyval.intv = yyvsp[0].intv;
            ;
    break;}
case 62:
#line 1459 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_UN,yyvsp[0].intv,IFC_NEG,0,0L, SYMNULL);
            ;
    break;}
case 63:
#line 1463 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_UN,yyvsp[0].intv,IFC_NOT,0,0L, SYMNULL);
            ;
    break;}
case 64:
#line 1467 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_UN,yyvsp[0].intv,IFC_HIGH,0,0L, SYMNULL);
            ;
    break;}
case 65:
#line 1471 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_UN,yyvsp[0].intv,IFC_LOW,0,0L, SYMNULL);
            ;
    break;}
case 66:
#line 1475 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_MUL,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 67:
#line 1479 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_DIV,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 68:
#line 1483 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_ADD,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 69:
#line 1487 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_SUB,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 70:
#line 1491 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_MOD,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 71:
#line 1495 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_SHL,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 72:
#line 1499 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_SHR,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 73:
#line 1503 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_GT,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 74:
#line 1507 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_GE,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 75:
#line 1511 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_LT,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 76:
#line 1515 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_LE,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 77:
#line 1519 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_NE,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 78:
#line 1523 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_EQ,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 79:
#line 1527 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_AND,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 80:
#line 1531 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_OR,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 81:
#line 1535 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_BIN,yyvsp[-2].intv,IFC_XOR,yyvsp[0].intv,0L, SYMNULL);
            ;
    break;}
case 82:
#line 1539 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_DEF,0,IGP_DEFINED,0,0L,yyvsp[0].symb);
            ;
    break;}
case 83:
#line 1543 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_SYMB,0,IFC_SYMB,0,0L,yyvsp[0].symb);
            ;
    break;}
case 84:
#line 1547 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_PROGC,0,IFC_PROGCTR,0,labelloc,SYMNULL);
            ;
    break;}
case 85:
#line 1551 "asm/as1600.y"
{
                yyval.intv = exprnode(PCCASE_CONS,0,IGP_CONSTANT,0,yyvsp[0].longv, SYMNULL);
            ;
    break;}
case 86:
#line 1555 "asm/as1600.y"
{
                yyval.intv = yyvsp[-1].intv;
            ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 542 "/usr/lib/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 1562 "asm/as1600.y"


lexintercept()
/*
    description intercept the call to yylex (the lexical analyzer)
            and filter out all unnecessary tokens when skipping
            the input between a failed IF and its matching ENDI or
            ELSE
    globals     fraifskip   the enable flag
*/
{
#undef yylex

    int rv;

    if(fraifskip)
    {
        for(;;)
        {

            switch(rv = yylex())

            {
            case 0:
            case KOC_END:
            case KOC_IF:
            case KOC_ELSE:
            case KOC_ENDI:
            case EOL:
                return rv;
            default:
                break;
            }
        }
    }
    else
        return yylex();
#define yylex lexintercept
}



setreserved()
{

    reservedsym("and",      KEOP_AND,       0);
    reservedsym("defined",  KEOP_DEFINED,   0);
    reservedsym("ge",       KEOP_GE,        0);
    reservedsym("high",     KEOP_HIGH,      0);
    reservedsym("le",       KEOP_LE,        0);
    reservedsym("low",      KEOP_LOW,       0);
    reservedsym("mod",      KEOP_MOD,       0);
    reservedsym("ne",       KEOP_NE,        0);
    reservedsym("not",      KEOP_NOT,       0);
    reservedsym("or",       KEOP_OR,        0);
    reservedsym("shl",      KEOP_SHL,       0);
    reservedsym("shr",      KEOP_SHR,       0);
    reservedsym("xor",      KEOP_XOR,       0);
    reservedsym("AND",      KEOP_AND,       0);
    reservedsym("DEFINED",  KEOP_DEFINED,   0);
    reservedsym("GE",       KEOP_GE,        0);
    reservedsym("HIGH",     KEOP_HIGH,      0);
    reservedsym("LE",       KEOP_LE,        0);
    reservedsym("LOW",      KEOP_LOW,       0);
    reservedsym("MOD",      KEOP_MOD,       0);
    reservedsym("NE",       KEOP_NE,        0);
    reservedsym("NOT",      KEOP_NOT,       0);
    reservedsym("OR",       KEOP_OR,        0);
    reservedsym("SHL",      KEOP_SHL,       0);
    reservedsym("SHR",      KEOP_SHR,       0);
    reservedsym("XOR",      KEOP_XOR,       0);

    /* machine specific token definitions */
    reservedsym("r0",       REGISTER,       0);
    reservedsym("r1",       REGISTER,       1);
    reservedsym("r2",       REGISTER,       2);
    reservedsym("r3",       REGISTER,       3);
    reservedsym("r4",       REGISTER,       4);
    reservedsym("r5",       REGISTER,       5);
    reservedsym("r6",       REGISTER,       6);
    reservedsym("r7",       REGISTER,       7);
    reservedsym("sp",       REGISTER,       6);
    reservedsym("pc",       REGISTER,       7);
    reservedsym("R0",       REGISTER,       0);
    reservedsym("R1",       REGISTER,       1);
    reservedsym("R2",       REGISTER,       2);
    reservedsym("R3",       REGISTER,       3);
    reservedsym("R4",       REGISTER,       4);
    reservedsym("R5",       REGISTER,       5);
    reservedsym("R6",       REGISTER,       6);
    reservedsym("R7",       REGISTER,       7);
    reservedsym("SP",       REGISTER,       6);
    reservedsym("PC",       REGISTER,       7);

}

cpumatch(str)
    char *str;
{
    return TRUE;
}


/* ======================================================================== */
/*  Opcode and Instruction Generation Tables                                */
/*                                                                          */
/*  These tables are used by the assembler framework to generate            */
/*  instructions from the parsed input.                                     */
/*                                                                          */
/*  OPTAB    -- OPcode TABle.  Contains the set of supported mnemonics.     */
/*  OSTAB    -- Opcode Syntax TABle.  Syntax definition sets for instrs.    */
/*  IGTAB    -- Instruction Generation TABle.  Contains RPN code for        */
/*              generating the instructions.                                */
/* ======================================================================== */



/* ======================================================================== */
/*  OPTAB    -- OPcode TABle.  Contains the set of supported mnemonics.     */
/* ======================================================================== */
struct opsym optab[] =
{
    {   "invalid",  KOC_opcode,     2,  0   },

    {   "MVO",      KOC_opcode,     1,  2   },
    {   "MVI",      KOC_opcode,     1,  3   },
    {   "ADD",      KOC_opcode,     1,  4   },
    {   "SUB",      KOC_opcode,     1,  5   },
    {   "CMP",      KOC_opcode,     1,  6   },
    {   "AND",      KOC_opcode,     1,  7   },
    {   "XOR",      KOC_opcode,     1,  8   },

    {   "MVO@",     KOC_opcode,     1,  9   },
    {   "MVI@",     KOC_opcode_i,   1,  10  },
    {   "ADD@",     KOC_opcode_i,   1,  11  },
    {   "SUB@",     KOC_opcode_i,   1,  12  },
    {   "CMP@",     KOC_opcode_i,   1,  13  },
    {   "AND@",     KOC_opcode_i,   1,  14  },
    {   "XOR@",     KOC_opcode_i,   1,  15  },

    {   "MVOI",     KOC_opcode,     1,  16  },
    {   "MVII",     KOC_opcode,     1,  17  },
    {   "ADDI",     KOC_opcode,     1,  18  },
    {   "SUBI",     KOC_opcode,     1,  19  },
    {   "CMPI",     KOC_opcode,     1,  20  },
    {   "ANDI",     KOC_opcode,     1,  21  },
    {   "XORI",     KOC_opcode,     1,  22  },

    {   "MOVR",     KOC_opcode,     1,  24  },
    {   "ADDR",     KOC_opcode,     1,  25  },
    {   "SUBR",     KOC_opcode,     1,  26  },
    {   "CMPR",     KOC_opcode,     1,  27  },
    {   "ANDR",     KOC_opcode,     1,  28  },
    {   "XORR",     KOC_opcode,     1,  29  },

    {   "B",        KOC_relbr,      1,  30  },
    {   "BC",       KOC_relbr,      1,  31  },
    {   "BOV",      KOC_relbr,      1,  32  },
    {   "BPL",      KOC_relbr,      1,  33  },
    {   "BZE",      KOC_relbr,      1,  34  },
    {   "BEQ",      KOC_relbr,      1,  34  },
    {   "BLT",      KOC_relbr,      1,  35  },
    {   "BNGE",     KOC_relbr,      1,  35  },
    {   "BLE",      KOC_relbr,      1,  36  },
    {   "BNGT",     KOC_relbr,      1,  36  },
    {   "BUSC",     KOC_relbr,      1,  37  },

    {   "NOPP",     KOC_opcode,     2,  92  },
    {   "BNC",      KOC_relbr,      1,  39  },
    {   "BNOV",     KOC_relbr,      1,  40  },
    {   "BMI",      KOC_relbr,      1,  41  },
    {   "BNZE",     KOC_relbr,      1,  42  },
    {   "BNZ",      KOC_relbr,      1,  42  },
    {   "BNEQ",     KOC_relbr,      1,  42  },
    {   "BNE",      KOC_relbr,      1,  42  },
    {   "BGE",      KOC_relbr,      1,  43  },
    {   "BNLT",     KOC_relbr,      1,  43  },
    {   "BGT",      KOC_relbr,      1,  44  },
    {   "BNLE",     KOC_relbr,      1,  44  },
    {   "BESC",     KOC_relbr,      1,  45  },

    {   "BEXT",     KOC_relbr_x,    1,  96  },

    {   "SWAP",     KOC_opcode,     2,  46  },
    {   "SLL",      KOC_opcode,     2,  48  },
    {   "RLC",      KOC_opcode,     2,  50  },
    {   "SLLC",     KOC_opcode,     2,  52  },
    {   "SLR",      KOC_opcode,     2,  54  },
    {   "SAR",      KOC_opcode,     2,  56  },
    {   "RRC",      KOC_opcode,     2,  58  },
    {   "SARC",     KOC_opcode,     2,  60  },

    {   "NOP",      KOC_opcode,     1,  62  },
    {   "NOP2",     KOC_opcode,     1,  94  },
    {   "SIN",      KOC_opcode,     1,  63  },
    {   "SIN2",     KOC_opcode,     1,  95  },

    {   "J",        KOC_opcode,     1,  64  },
    {   "JE",       KOC_opcode,     1,  65  },
    {   "JD",       KOC_opcode,     1,  66  },
    {   "JSR",      KOC_opcode,     1,  67  },
    {   "JSRE",     KOC_opcode,     1,  68  },
    {   "JSRD",     KOC_opcode,     1,  69  },

    {   "INCR",     KOC_opcode,     1,  70  },
    {   "DECR",     KOC_opcode,     1,  71  },
    {   "COMR",     KOC_opcode,     1,  72  },
    {   "NEGR",     KOC_opcode,     1,  73  },
    {   "ADCR",     KOC_opcode,     1,  74  },
    {   "GSWD",     KOC_opcode,     1,  75  },
    {   "RSWD",     KOC_opcode,     1,  76  },

    {   "HLT",      KOC_opcode,     1,  77  },
    {   "SDBD",     KOC_SDBD,       1,  78  },
    {   "EIS",      KOC_opcode,     1,  79  },
    {   "DIS",      KOC_opcode,     1,  80  },
    {   "TCI",      KOC_opcode,     1,  81  },
    {   "CLRC",     KOC_opcode,     1,  82  },
    {   "SETC",     KOC_opcode,     1,  83  },

    {   "TSTR",     KOC_opcode,     1,  84  },  /*  MOVR  Rx, Rx    */
    {   "CLRR",     KOC_opcode,     1,  85  },  /*  XORR  Rx, Rx    */
    {   "PSHR",     KOC_opcode,     1,  86  },  /*  MVO@  Rx, SP    */
    {   "PULR",     KOC_opcode,     1,  87  },  /*  MVI@  SP, Rx    */
    {   "JR",       KOC_opcode,     1,  88  },  /*  MOVR  Rx, PC    */
    {   "CALL",     KOC_opcode,     1,  89  },  /*  JSR   R5, addr  */
    {   "BEGIN",    KOC_opcode,     1,  90  },  /*  MVO@  R5, SP    */
    {   "RETURN",   KOC_opcode,     1,  91  },  /*  MVI@  SP, PC    */

    {   "DECLE",    KOC_DDEF,       0,  0   },  /* Generates ROMW values  */
    {   "DCW",      KOC_DDEF,       0,  0   },  /* Generates ROMW values  */
    {   "BIDECLE",  KOC_WDEF,       0,  0   },  /* Generates SDBD values  */
    {   "ROMWIDTH", KOC_ROMW,       0,  0   },
    {   "ROMW",     KOC_ROMW,       0,  0   },
    {   "PROC",     KOC_PROC,       0,  0   },
    {   "ENDP",     KOC_ENDP,       0,  0   },

    {   "BYTE",     KOC_BDEF,       0,  0   },  /* Generates 8-bit values */
    {   "CHARDEF",  KOC_CHDEF,      0,  0   },
    {   "CHARSET",  KOC_CHSET,      0,  0   },
    {   "CHARUSE",  KOC_CHUSE,      0,  0   },
    {   "CHD",      KOC_CHDEF,      0,  0   },
    {   "DATA",     KOC_DDEF,       0,  0   },  /* Generates ROMW values  */
    {   "DB",       KOC_BDEF,       0,  0   },  /* Generates 8-bit values */
    {   "DW",       KOC_WDEF,       0,  0   },  /* Generates SDBD values  */
    {   "ELSE",     KOC_ELSE,       0,  0   },
    {   "END",      KOC_END,        0,  0   },
    {   "ENDI",     KOC_ENDI,       0,  0   },
    {   "EQU",      KOC_EQU,        0,  0   },
    {   "FCB",      KOC_BDEF,       0,  0   },  /* Generates 8-bit values */
    {   "FCC",      KOC_SDEF,       0,  0   },
    {   "FDB",      KOC_WDEF,       0,  0   },  /* Generates SDBD values  */
    {   "IF",       KOC_IF,         0,  0   },
    {   "INCL",     KOC_INCLUDE,    0,  0   },
    {   "INCLUDE",  KOC_INCLUDE,    0,  0   },
    {   "ORG",      KOC_ORG,        0,  0   },
    {   "RES",      KOC_RESM,       0,  0   },
    {   "RESERVE",  KOC_RESM,       0,  0   },
    {   "RMB",      KOC_RESM,       0,  0   },
    {   "SET",      KOC_SET,        0,  0   },
    {   "STRING",   KOC_SDEF,       0,  0   },
    {   "WORD",     KOC_WDEF,       0,  0   },  /* Generates SDBD values  */

    {   "STRUCT",   KOC_STRUCT,     0,  0   },  /* Opens a struct def'n */
    {   "ENDS",     KOC_ENDS,       0,  0   },  /* Closes a struct def'n */

    {   "MEMATTR",  KOC_MEMATTR,    0,  0   },  /* Set memory attributes */

    {   "",         0,              0,  0   }
};


/* ======================================================================== */
/*  OSTAB    -- Opcode Syntax TABle.  Syntax definition sets for instrs.    */
/*                                                                          */
/*  Legend:                                                                 */
/*      REG      Register.                                                  */
/*      EXP      EXPression                                                 */
/*      CEX      Constant EXpression (eg. exp. prefixed w/ #).              */
/*      IMP      Implied operand.                                           */
/* ======================================================================== */
struct opsynt ostab[] = 
{
    /*  invalid 0   */  {   0,          1,  0   },
    /*  invalid 1   */  {   0xFFFF,     1,  1   },

    /*  MVO     2   */  {   ST_REGEXP,  1,  2   },
    /*  MVI     3   */  {   ST_EXPREG,  1,  3   },
    /*  ADD     4   */  {   ST_EXPREG,  1,  4   },
    /*  SUB     5   */  {   ST_EXPREG,  1,  5   },
    /*  CMP     6   */  {   ST_EXPREG,  1,  6   },
    /*  AND     7   */  {   ST_EXPREG,  1,  7   },
    /*  XOR     8   */  {   ST_EXPREG,  1,  8   },

    /*  MVO@    9   */  {   ST_REGREG,  1,  9   },
    /*  MVI@    10  */  {   ST_REGREG,  1,  10  },
    /*  ADD@    11  */  {   ST_REGREG,  1,  11  },
    /*  SUB@    12  */  {   ST_REGREG,  1,  12  },
    /*  CMP@    13  */  {   ST_REGREG,  1,  13  },
    /*  AND@    14  */  {   ST_REGREG,  1,  14  },
    /*  XOR@    15  */  {   ST_REGREG,  1,  15  },

    /*  MVOI    16  */  {   ST_REGCEX,  1,  16  },
    /*  MVII    17  */  {   ST_CEXREG,  2,  17  },
    /*  ADDI    18  */  {   ST_CEXREG,  2,  19  },
    /*  SUBI    19  */  {   ST_CEXREG,  2,  21  },
    /*  CMPI    20  */  {   ST_CEXREG,  2,  23  },
    /*  ANDI    21  */  {   ST_CEXREG,  2,  25  },
    /*  XORI    22  */  {   ST_CEXREG,  2,  27  },

    /*  unused  23  */  {   0,          1,  0   },  /* oops */
    /*  MOVR    24  */  {   ST_REGREG,  1,  29  },
    /*  ADDR    25  */  {   ST_REGREG,  1,  30  },
    /*  SUBR    26  */  {   ST_REGREG,  1,  31  },
    /*  CMPR    27  */  {   ST_REGREG,  1,  32  },
    /*  ANDR    28  */  {   ST_REGREG,  1,  33  },
    /*  XORR    29  */  {   ST_REGREG,  1,  34  },

    /*  B       30  */  {   ST_EXP,     1,  35  },
    /*  BC      31  */  {   ST_EXP,     1,  36  },
    /*  BOV     32  */  {   ST_EXP,     1,  37  },
    /*  BPL     33  */  {   ST_EXP,     1,  38  },
    /*  BEQ     34  */  {   ST_EXP,     1,  39  },
    /*  BLT     35  */  {   ST_EXP,     1,  40  },
    /*  BLE     36  */  {   ST_EXP,     1,  41  },
    /*  BUSC    37  */  {   ST_EXP,     1,  42  },

    /*  unused  38  */  {   0,          1,  0   },  /* oops */
    /*  BNC     39  */  {   ST_EXP,     1,  44  },
    /*  BNOV    40  */  {   ST_EXP,     1,  45  },
    /*  BMI     41  */  {   ST_EXP,     1,  46  },
    /*  BNEQ    42  */  {   ST_EXP,     1,  47  },
    /*  BGE     43  */  {   ST_EXP,     1,  48  },
    /*  BGT     44  */  {   ST_EXP,     1,  49  },
    /*  BESC    45  */  {   ST_EXP,     1,  50  },

    /*  SWAP    46  */  {   ST_REG,     1,  51  },
    /*  SWAP    47  */  {   ST_REGEXP,  1,  52  },
    /*  SLL     48  */  {   ST_REG,     1,  53  },
    /*  SLL     49  */  {   ST_REGEXP,  1,  54  },
    /*  RLC     50  */  {   ST_REG,     1,  55  },
    /*  RLC     51  */  {   ST_REGEXP,  1,  56  },
    /*  SLLC    52  */  {   ST_REG,     1,  57  },
    /*  SLLC    53  */  {   ST_REGEXP,  1,  58  },
    /*  SLR     54  */  {   ST_REG,     1,  59  },
    /*  SLR     55  */  {   ST_REGEXP,  1,  60  },
    /*  SAR     56  */  {   ST_REG,     1,  61  },
    /*  SAR     57  */  {   ST_REGEXP,  1,  62  },
    /*  RRC     58  */  {   ST_REG,     1,  63  },
    /*  RRC     59  */  {   ST_REGEXP,  1,  64  },
    /*  SARC    60  */  {   ST_REG,     1,  65  },
    /*  SARC    61  */  {   ST_REGEXP,  1,  66  },

    /*  NOP     62  */  {   ST_IMP,     1,  67  },
    /*  SIN     63  */  {   ST_IMP,     1,  68  },

    /*  J       64  */  {   ST_EXP,     1,  69  },
    /*  JE      65  */  {   ST_EXP,     1,  70  },
    /*  JD      66  */  {   ST_EXP,     1,  71  },
    /*  JSR     67  */  {   ST_REGEXP,  1,  72  },
    /*  JSRE    68  */  {   ST_REGEXP,  1,  73  },
    /*  JSRD    69  */  {   ST_REGEXP,  1,  74  },

    /*  INCR    70  */  {   ST_REG,     1,  75  },
    /*  DECR    71  */  {   ST_REG,     1,  76  },
    /*  COMR    72  */  {   ST_REG,     1,  77  },
    /*  NEGR    73  */  {   ST_REG,     1,  78  },
    /*  ADCR    74  */  {   ST_REG,     1,  79  },
    /*  GSWD    75  */  {   ST_REG,     1,  80  },
    /*  RSWD    76  */  {   ST_REG,     1,  81  },

    /*  HLT     77  */  {   ST_IMP,     1,  82  },
    /*  SDBD    78  */  {   ST_IMP,     1,  83  },
    /*  EIS     79  */  {   ST_IMP,     1,  84  },
    /*  DIS     80  */  {   ST_IMP,     1,  85  },
    /*  TCI     81  */  {   ST_IMP,     1,  86  },
    /*  CLRC    82  */  {   ST_IMP,     1,  87  },
    /*  SETC    83  */  {   ST_IMP,     1,  88  },

    /*  TSTR    84  */  {   ST_REG,     1,  89  },
    /*  CLRR    85  */  {   ST_REG,     1,  90  },
    /*  PSHR    86  */  {   ST_REG,     1,  91  },
    /*  PULR    87  */  {   ST_REG,     1,  92  },
    /*  JR      88  */  {   ST_REG,     1,  93  },
    /*  CALL    89  */  {   ST_EXP,     1,  94  },
    /*  BEGIN   90  */  {   ST_IMP,     1,  95  },
    /*  RETURN  91  */  {   ST_IMP,     1,  96  },

    /*  NOPP    92  */  {   ST_EXP,     1,  43  },
    /*  NOPP    93  */  {   ST_IMP,     1,  97  },

    /*  NOP2    94  */  {   ST_IMP,     1,  98  },
    /*  SIN2    95  */  {   ST_IMP,     1,  99  },

    /*  BEXT    95  */  {   ST_EXPEXP,  1,  100 },

    /*  end         */  {   0,          0,  0   }
};


/* ======================================================================== */
/*  Helper macros.                                                          */
/*  MVO_OK  Tests arg 2 to make sure it's R1 .. R6.                         */
/*  SH_OK   Tests if shift amount is ok.                                    */
/*  CST_OK  Tests if constant is ok (within field width).                   */
/*  DBD     Generate double-byte-data.                                      */
/*  RR      Register/Register generator. Reused for Direct, Immediate.      */
/*  BR      Branch Relative generator.                                      */
/*  SH      Shift generator.                                                */
/*  SR      Single-register generator                                       */
/*  JSR     Jump/JSR generator                                              */
/*  CST     Constant arg generator (eg. immediate argument)                 */
/* ======================================================================== */
#define MVO_OK      "[2#].0=.[2#].7=+T$"
#define SH_OK(n)    #n ".2>." #n ".<0.+T$"
#define CST_OK(w,c) #c "." #w"I$"
#define DBD(x)      #x ".FF&x" #x ".8}.FF&x"
#define RR(o,x,y)   #o "." #x ".3{|." #y "|x"
#define BRDIR(a)    "P.2+." #a ">."
#define BROFS(a)    #a ".P.2+-." BRDIR(a) "!_^"
#define BR(c,a,w)   "0200." #c "|." BRDIR(a) "5{|x" BROFS(a) "~x" #w "I$"
#define BX(c,a,w)   #c ".4I$" \
                    "0210." #c "|." BRDIR(a) "5{|x" BROFS(a) "~x" #w "I$"
/*#define BR(c,a,m)   "0200." #c "|." BRDIR(a) "5{|x" #a "x"*/
#define CST(c,m)    CST_OK(m,c) #c "x"
#define SH(o,n,r)   SH_OK(n) "0040." #o ".3{|." #n ".1&.2{|." #r "|x"
#define SR(o,r)     "0000." #o ".3{|." #r "|x"
#define JSR(r,e,a)  "0004x" #r ".3&.8{." #a ".8}.FC&|." #e "|x" #a ".3FF&x"


/* ======================================================================== */
/*  IGTAB    -- Instruction Generator Table.                                */
/* ======================================================================== */
struct igel igtab[] = 
{
    /* inv  0   */  {   SDBD,       0,      "[Xnullentry"                   },
    /* inv  1   */  {   SDBD,       0,      "[Xinvalid opcode"              },

    /* MVO  2   */  {   SDBD,       0,      RR(0240,0,[1#]) CST([2=],[3#])  },
    /* MVI  3   */  {   SDBD,       0,      RR(0280,0,[2#]) CST([1=],[3#])  },
    /* ADD  4   */  {   SDBD,       0,      RR(02C0,0,[2#]) CST([1=],[3#])  },
    /* SUB  5   */  {   SDBD,       0,      RR(0300,0,[2#]) CST([1=],[3#])  },
    /* CMP  6   */  {   SDBD,       0,      RR(0340,0,[2#]) CST([1=],[3#])  },
    /* AND  7   */  {   SDBD,       0,      RR(0380,0,[2#]) CST([1=],[3#])  },
    /* XOR  8   */  {   SDBD,       0,      RR(03C0,0,[2#]) CST([1=],[3#])  },
    
    /* MVO@ 9   */  {   SDBD,       0,      MVO_OK
                                            RR(0240,[2#],[1#])              },
    /* MVI@ 10  */  {   IND_RG,     IND_RG, RR(0280,[1#],[2#])              },
    /* ADD@ 11  */  {   IND_RG,     IND_RG, RR(02C0,[1#],[2#])              },
    /* SUB@ 12  */  {   IND_RG,     IND_RG, RR(0300,[1#],[2#])              },
    /* CMP@ 13  */  {   IND_RG,     IND_RG, RR(0340,[1#],[2#])              },
    /* AND@ 14  */  {   IND_RG,     IND_RG, RR(0380,[1#],[2#])              },
    /* XOR@ 15  */  {   IND_RG,     IND_RG, RR(03C0,[1#],[2#])              },

    /* MVOI 16  */  {   SDBD,       0,      RR(0240,7,[1#]) CST([2=],[3#])  },
    /* MVII 17  */  {   SDBD,       0,      RR(0280,7,[2#]) CST([1=],[3#])  },
    /* MVII 18  */  {   SDBD,       SDBD,   RR(0280,7,[2#]) DBD([1=])       },
    /* ADDI 19  */  {   SDBD,       0,      RR(02C0,7,[2#]) CST([1=],[3#])  },
    /* ADDI 20  */  {   SDBD,       SDBD,   RR(02C0,7,[2#]) DBD([1=])       },
    /* SUBI 21  */  {   SDBD,       0,      RR(0300,7,[2#]) CST([1=],[3#])  },
    /* SUBI 22  */  {   SDBD,       SDBD,   RR(0300,7,[2#]) DBD([1=])       },
    /* CMPI 23  */  {   SDBD,       0,      RR(0340,7,[2#]) CST([1=],[3#])  },
    /* CMPI 24  */  {   SDBD,       SDBD,   RR(0340,7,[2#]) DBD([1=])       },
    /* ANDI 25  */  {   SDBD,       0,      RR(0380,7,[2#]) CST([1=],[3#])  },
    /* ANDI 26  */  {   SDBD,       SDBD,   RR(0380,7,[2#]) DBD([1=])       },
    /* XORI 27  */  {   SDBD,       0,      RR(03C0,7,[2#]) CST([1=],[3#])  },
    /* XORI 28  */  {   SDBD,       SDBD,   RR(03C0,7,[2#]) DBD([1=])       },

    /* MOVR 29  */  {   SDBD,       0,      RR(0080,[1#],[2#])              },
    /* ADDR 30  */  {   SDBD,       0,      RR(00C0,[1#],[2#])              },
    /* SUBR 31  */  {   SDBD,       0,      RR(0100,[1#],[2#])              },
    /* CMPR 32  */  {   SDBD,       0,      RR(0140,[1#],[2#])              },
    /* ANDR 33  */  {   SDBD,       0,      RR(0180,[1#],[2#])              },
    /* XORR 34  */  {   SDBD,       0,      RR(01C0,[1#],[2#])              },

    /* B    35  */  {   SDBD,       0,      BR(0,[1=],[3#])                 },
    /* BC   36  */  {   SDBD,       0,      BR(1,[1=],[3#])                 },
    /* BOV  37  */  {   SDBD,       0,      BR(2,[1=],[3#])                 },
    /* BPL  38  */  {   SDBD,       0,      BR(3,[1=],[3#])                 },
    /* BEQ  39  */  {   SDBD,       0,      BR(4,[1=],[3#])                 },
    /* BLT  40  */  {   SDBD,       0,      BR(5,[1=],[3#])                 },
    /* BLE  41  */  {   SDBD,       0,      BR(6,[1=],[3#])                 },
    /* BUSC 42  */  {   SDBD,       0,      BR(7,[1=],[3#])                 },
    /* NOPP 43  */  {   SDBD,       0,      BR(8,[1=],[3#])                 },
    /* BNC  44  */  {   SDBD,       0,      BR(9,[1=],[3#])                 },
    /* BNOV 45  */  {   SDBD,       0,      BR(A,[1=],[3#])                 },
    /* BMI  46  */  {   SDBD,       0,      BR(B,[1=],[3#])                 },
    /* BNEQ 47  */  {   SDBD,       0,      BR(C,[1=],[3#])                 },
    /* BGE  48  */  {   SDBD,       0,      BR(D,[1=],[3#])                 },
    /* BGT  49  */  {   SDBD,       0,      BR(E,[1=],[3#])                 },
    /* BESC 50  */  {   SDBD,       0,      BR(F,[1=],[3#])                 },

    /* SWAP 51  */  {   SDBD|SHF_RG,SHF_RG, SH(0,0,[1#])                    },
    /* SWAP 52  */  {   SDBD|SHF_RG,SHF_RG, SH(0,[2=].1-,[1#])              },
    /* SLL  53  */  {   SDBD|SHF_RG,SHF_RG, SH(1,0,[1#])                    },
    /* SLL  54  */  {   SDBD|SHF_RG,SHF_RG, SH(1,[2=].1-,[1#])              },
    /* RLC  55  */  {   SDBD|SHF_RG,SHF_RG, SH(2,0,[1#])                    },
    /* RLC  56  */  {   SDBD|SHF_RG,SHF_RG, SH(2,[2=].1-,[1#])              },
    /* SLLC 57  */  {   SDBD|SHF_RG,SHF_RG, SH(3,0,[1#])                    },
    /* SLLC 58  */  {   SDBD|SHF_RG,SHF_RG, SH(3,[2=].1-,[1#])              },
    /* SLR  59  */  {   SDBD|SHF_RG,SHF_RG, SH(4,0,[1#])                    },
    /* SLR  60  */  {   SDBD|SHF_RG,SHF_RG, SH(4,[2=].1-,[1#])              },
    /* SAR  61  */  {   SDBD|SHF_RG,SHF_RG, SH(5,0,[1#])                    },
    /* SAR  62  */  {   SDBD|SHF_RG,SHF_RG, SH(5,[2=].1-,[1#])              },
    /* RRC  63  */  {   SDBD|SHF_RG,SHF_RG, SH(6,0,[1#])                    },
    /* RRC  64  */  {   SDBD|SHF_RG,SHF_RG, SH(6,[2=].1-,[1#])              },
    /* SARC 65  */  {   SDBD|SHF_RG,SHF_RG, SH(7,0,[1#])                    },
    /* SARC 66  */  {   SDBD|SHF_RG,SHF_RG, SH(7,[2=].1-,[1#])              },

    /* NOP  67  */  {   SDBD,       0,      "0034x"                         },
    /* SIN  68  */  {   SDBD,       0,      "0036x"                         },

    /* J    69  */  {   SDBD,       0,      JSR(3,0,[1=])                   },
    /* JE   70  */  {   SDBD,       0,      JSR(3,1,[1=])                   },
    /* JD   71  */  {   SDBD,       0,      JSR(3,2,[1=])                   },
    /* JSR  72  */  {   SDBD|JSR_RG,JSR_RG, JSR([1#],0,[2=])                },
    /* JSRE 73  */  {   SDBD|JSR_RG,JSR_RG, JSR([1#],1,[2=])                },
    /* JSRD 74  */  {   SDBD|JSR_RG,JSR_RG, JSR([1#],2,[2=])                },

    /* INCR 75  */  {   SDBD,       0,      SR(1,[1#])                      },
    /* DECR 76  */  {   SDBD,       0,      SR(2,[1#])                      },
    /* COMR 77  */  {   SDBD,       0,      SR(3,[1#])                      },
    /* NEGR 78  */  {   SDBD,       0,      SR(4,[1#])                      },
    /* ADCR 79  */  {   SDBD,       0,      SR(5,[1#])                      },
    /* GSWD 80  */  {   SDBD|SHF_RG,SHF_RG, SR(6,[1#])                      },
    /* RSWD 81  */  {   SDBD,       0,      SR(7,[1#])                      },

    /* HLT  82  */  {   SDBD,       0,      "0000x"                         },
    /* SDBD 83  */  {   SDBD,       0,      "0001x"                         },
    /* EIS  84  */  {   SDBD,       0,      "0002x"                         },
    /* DIS  85  */  {   SDBD,       0,      "0003x"                         },
    /* TCI  86  */  {   SDBD,       0,      "0005x"                         },
    /* CLRC 87  */  {   SDBD,       0,      "0006x"                         },
    /* SETC 88  */  {   SDBD,       0,      "0007x"                         },

    /* TSTR 89  */  {   SDBD,       0,      RR(0080,[1#],[1#])              },
    /* CLRR 90  */  {   SDBD,       0,      RR(01C0,[1#],[1#])              },
    /* PSHR 91  */  {   SDBD,       0,      RR(0240,6,[1#])                 },
    /* PULR 92  */  {   0,          0,      RR(0280,6,[1#])                 },
    /* JR   93  */  {   SDBD,       0,      RR(0080,[1#],7)                 },
    /* CALL 94  */  {   SDBD,       0,      JSR(5,0,[1=])                   },
    /* BEGIN 95 */  {   SDBD,       0,      RR(0240,6,5)                    },
    /* RETURN 96*/  {   SDBD,       0,      RR(0280,6,7)                    },

    /* NOPP 97  */  {   SDBD,       0,      "0208x0000x"                    },
    /* NOP2 98  */  {   SDBD,       0,      "0035x"                         },
    /* SIN2 99  */  {   SDBD,       0,      "0037x"                         },

    /* BESC 100 */  {   SDBD,       0,      BX([4#],[1=],[3#])              },

    /* end      */  {   0,          0,      "[Xinvalid opcode"              },
};

#define NUMOPCODE (sizeof(optab)/sizeof(struct opsym))

int gnumopcode = NUMOPCODE;
int ophashlnk[NUMOPCODE];

/* ======================================================================== */
/*  End of file:  fraptabdef.c                                              */
/* ======================================================================== */
