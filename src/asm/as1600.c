/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
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


#line 505 "asm/as1600.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "as1600.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_REGISTER = 3,                   /* REGISTER  */
  YYSYMBOL_KOC_BDEF = 4,                   /* KOC_BDEF  */
  YYSYMBOL_KOC_ELSE = 5,                   /* KOC_ELSE  */
  YYSYMBOL_KOC_END = 6,                    /* KOC_END  */
  YYSYMBOL_KOC_ENDI = 7,                   /* KOC_ENDI  */
  YYSYMBOL_KOC_EQU = 8,                    /* KOC_EQU  */
  YYSYMBOL_KOC_IF = 9,                     /* KOC_IF  */
  YYSYMBOL_KOC_INCLUDE = 10,               /* KOC_INCLUDE  */
  YYSYMBOL_KOC_ORG = 11,                   /* KOC_ORG  */
  YYSYMBOL_KOC_RESM = 12,                  /* KOC_RESM  */
  YYSYMBOL_KOC_SDEF = 13,                  /* KOC_SDEF  */
  YYSYMBOL_KOC_SET = 14,                   /* KOC_SET  */
  YYSYMBOL_KOC_WDEF = 15,                  /* KOC_WDEF  */
  YYSYMBOL_KOC_CHSET = 16,                 /* KOC_CHSET  */
  YYSYMBOL_KOC_CHDEF = 17,                 /* KOC_CHDEF  */
  YYSYMBOL_KOC_CHUSE = 18,                 /* KOC_CHUSE  */
  YYSYMBOL_KOC_opcode = 19,                /* KOC_opcode  */
  YYSYMBOL_KOC_opcode_i = 20,              /* KOC_opcode_i  */
  YYSYMBOL_KOC_relbr = 21,                 /* KOC_relbr  */
  YYSYMBOL_KOC_relbr_x = 22,               /* KOC_relbr_x  */
  YYSYMBOL_KOC_SDBD = 23,                  /* KOC_SDBD  */
  YYSYMBOL_KOC_ROMW = 24,                  /* KOC_ROMW  */
  YYSYMBOL_KOC_PROC = 25,                  /* KOC_PROC  */
  YYSYMBOL_KOC_ENDP = 26,                  /* KOC_ENDP  */
  YYSYMBOL_KOC_STRUCT = 27,                /* KOC_STRUCT  */
  YYSYMBOL_KOC_ENDS = 28,                  /* KOC_ENDS  */
  YYSYMBOL_KOC_MEMATTR = 29,               /* KOC_MEMATTR  */
  YYSYMBOL_KOC_DDEF = 30,                  /* KOC_DDEF  */
  YYSYMBOL_CONSTANT = 31,                  /* CONSTANT  */
  YYSYMBOL_EOL = 32,                       /* EOL  */
  YYSYMBOL_KEOP_AND = 33,                  /* KEOP_AND  */
  YYSYMBOL_KEOP_DEFINED = 34,              /* KEOP_DEFINED  */
  YYSYMBOL_KEOP_EQ = 35,                   /* KEOP_EQ  */
  YYSYMBOL_KEOP_GE = 36,                   /* KEOP_GE  */
  YYSYMBOL_KEOP_GT = 37,                   /* KEOP_GT  */
  YYSYMBOL_KEOP_HIGH = 38,                 /* KEOP_HIGH  */
  YYSYMBOL_KEOP_LE = 39,                   /* KEOP_LE  */
  YYSYMBOL_KEOP_LOW = 40,                  /* KEOP_LOW  */
  YYSYMBOL_KEOP_LT = 41,                   /* KEOP_LT  */
  YYSYMBOL_KEOP_MOD = 42,                  /* KEOP_MOD  */
  YYSYMBOL_KEOP_MUN = 43,                  /* KEOP_MUN  */
  YYSYMBOL_KEOP_NE = 44,                   /* KEOP_NE  */
  YYSYMBOL_KEOP_NOT = 45,                  /* KEOP_NOT  */
  YYSYMBOL_KEOP_OR = 46,                   /* KEOP_OR  */
  YYSYMBOL_KEOP_SHL = 47,                  /* KEOP_SHL  */
  YYSYMBOL_KEOP_SHR = 48,                  /* KEOP_SHR  */
  YYSYMBOL_KEOP_XOR = 49,                  /* KEOP_XOR  */
  YYSYMBOL_KEOP_locctr = 50,               /* KEOP_locctr  */
  YYSYMBOL_LABEL = 51,                     /* LABEL  */
  YYSYMBOL_STRING = 52,                    /* STRING  */
  YYSYMBOL_SYMBOL = 53,                    /* SYMBOL  */
  YYSYMBOL_KTK_invalid = 54,               /* KTK_invalid  */
  YYSYMBOL_55_ = 55,                       /* '+'  */
  YYSYMBOL_56_ = 56,                       /* '-'  */
  YYSYMBOL_57_ = 57,                       /* '*'  */
  YYSYMBOL_58_ = 58,                       /* '/'  */
  YYSYMBOL_59_ = 59,                       /* ','  */
  YYSYMBOL_60_ = 60,                       /* ':'  */
  YYSYMBOL_61_ = 61,                       /* '#'  */
  YYSYMBOL_62_ = 62,                       /* '$'  */
  YYSYMBOL_63_ = 63,                       /* '('  */
  YYSYMBOL_64_ = 64,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 65,                  /* $accept  */
  YYSYMBOL_file = 66,                      /* file  */
  YYSYMBOL_allline = 67,                   /* allline  */
  YYSYMBOL_line = 68,                      /* line  */
  YYSYMBOL_labeledline = 69,               /* labeledline  */
  YYSYMBOL_labelcolon = 70,                /* labelcolon  */
  YYSYMBOL_genline = 71,                   /* genline  */
  YYSYMBOL_exprlist = 72,                  /* exprlist  */
  YYSYMBOL_expr = 73                       /* expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  64
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   717

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  9
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  153

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   309


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    61,    62,     2,     2,     2,
      63,    64,    57,    55,    59,    56,     2,    58,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    60,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   513,   513,   514,   517,   521,   522,   529,   534,   538,
     558,   579,   601,   633,   654,   672,   691,   714,   730,   754,
     771,   800,   821,   840,   856,   861,   886,   953,   965,   968,
     989,  1000,  1001,  1003,  1014,  1026,  1037,  1046,  1063,  1068,
    1081,  1087,  1119,  1138,  1151,  1180,  1195,  1220,  1257,  1271,
    1287,  1312,  1323,  1336,  1351,  1366,  1381,  1416,  1428,  1441,
    1454,  1458,  1462,  1466,  1470,  1474,  1478,  1482,  1486,  1490,
    1494,  1498,  1502,  1506,  1510,  1514,  1518,  1522,  1526,  1530,
    1534,  1538,  1542,  1546,  1550,  1554
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "REGISTER", "KOC_BDEF",
  "KOC_ELSE", "KOC_END", "KOC_ENDI", "KOC_EQU", "KOC_IF", "KOC_INCLUDE",
  "KOC_ORG", "KOC_RESM", "KOC_SDEF", "KOC_SET", "KOC_WDEF", "KOC_CHSET",
  "KOC_CHDEF", "KOC_CHUSE", "KOC_opcode", "KOC_opcode_i", "KOC_relbr",
  "KOC_relbr_x", "KOC_SDBD", "KOC_ROMW", "KOC_PROC", "KOC_ENDP",
  "KOC_STRUCT", "KOC_ENDS", "KOC_MEMATTR", "KOC_DDEF", "CONSTANT", "EOL",
  "KEOP_AND", "KEOP_DEFINED", "KEOP_EQ", "KEOP_GE", "KEOP_GT", "KEOP_HIGH",
  "KEOP_LE", "KEOP_LOW", "KEOP_LT", "KEOP_MOD", "KEOP_MUN", "KEOP_NE",
  "KEOP_NOT", "KEOP_OR", "KEOP_SHL", "KEOP_SHR", "KEOP_XOR", "KEOP_locctr",
  "LABEL", "STRING", "SYMBOL", "KTK_invalid", "'+'", "'-'", "'*'", "'/'",
  "','", "':'", "'#'", "'$'", "'('", "')'", "$accept", "file", "allline",
  "line", "labeledline", "labelcolon", "genline", "exprlist", "expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-48)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     204,   -31,   -12,   -48,   -48,   -48,   258,   -47,   258,   258,
     -12,   -12,   -43,   258,    72,     9,   258,   258,   -48,   258,
     -48,   -48,   258,   -12,   -48,   -42,   169,   -48,   -15,   -48,
     237,   -48,   -48,   -48,   -33,   258,   258,   258,   -48,   -48,
     258,   258,   -48,   258,   -38,   585,   585,   -48,   315,   585,
     -38,   -38,   -36,   585,   -35,   258,   342,   -34,   585,   369,
     396,   423,   -38,   -48,   -48,   -48,   -48,   -48,   258,   258,
     258,   -48,   -48,   258,   -21,   -48,   -48,   585,   585,   635,
     -48,   -48,   289,   238,   258,   258,   258,   258,   258,   258,
     258,   258,   258,   258,   258,   258,   258,   258,   258,   258,
     258,   -12,   105,   450,    29,    33,   258,   258,   258,   585,
     477,   585,   585,   -48,   -48,   585,   635,   659,   659,   659,
     659,   659,   -48,   659,   611,   -48,   -48,   611,     0,     0,
     -48,   -48,   504,   -38,   -48,   258,   585,    35,   -48,   -48,
     585,   585,   531,   258,   -13,   585,   -48,    -7,   558,   -48,
     -48,     1,   -48
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,    14,     8,    15,    13,     0,     0,     0,
       0,     0,     0,    24,    51,     0,     0,     0,    48,     0,
      43,    45,     0,     0,     5,    31,     0,     3,     0,    28,
      27,    30,     6,    84,     0,     0,     0,     0,    41,    82,
       0,     0,    83,     0,    33,    40,    12,     9,    17,    37,
      35,    36,     0,    25,    57,     0,    52,     0,    49,     0,
      46,     0,    34,    32,     1,     2,     4,     7,     0,     0,
       0,    23,    42,     0,     0,    29,    81,    63,    64,    62,
      60,    61,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    10,
      16,    11,    44,    85,    39,    38,    78,    77,    73,    72,
      75,    74,    69,    76,    79,    70,    71,    80,    67,    68,
      65,    66,    19,    26,    58,     0,    53,     0,    55,    59,
      50,    47,     0,     0,     0,    54,    56,     0,    18,    21,
      22,     0,    20
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -48,   -48,    20,   -48,   -48,    22,    24,     4,    -6
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    26,    27,    28,    29,    30,    31,    44,    45
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      46,    32,    48,    49,    72,    47,    73,    53,    56,    52,
      58,    59,    57,    60,    50,    51,    61,    66,    63,    33,
      76,    83,    34,   101,   102,   105,    35,    62,    36,    77,
      78,    79,   138,    37,    80,    81,   139,    82,   146,   149,
      38,    39,    90,    40,    41,   150,    65,    93,    94,   103,
      42,    43,    74,   152,    75,     0,     0,    98,    99,     0,
       0,     0,   109,   110,   111,     0,     0,   112,     0,     0,
       0,     0,     0,     0,     0,    54,     0,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,     0,   136,     0,     0,     0,
     140,   141,   142,    33,     0,   133,    34,     0,   134,     0,
      35,     0,    36,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,     0,    39,     0,    40,    41,   145,
       0,     0,     0,    55,    42,    43,    33,   148,     0,    34,
       0,     0,     0,    35,     0,    36,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,     0,    39,     0,
      40,    41,     0,     0,     0,     0,   135,    42,    43,    64,
       1,     0,     0,     2,     3,     4,     5,     0,     6,     7,
       8,     9,    10,     0,    11,     0,    12,    13,    14,    15,
      16,    17,    18,    19,     0,    20,     0,    21,    22,    23,
       0,    24,     0,     0,     0,     1,     0,     0,     2,     3,
       4,     5,     0,     6,     7,     8,     9,    10,     0,    11,
      25,    12,    13,    14,    15,    16,    17,    18,    19,     0,
      20,     0,    21,    22,    23,     0,    24,     0,     0,     0,
       0,     2,     0,    67,     0,    68,     0,     0,    69,     9,
      10,    70,    11,    71,     0,    25,    14,    15,    16,    17,
      18,    19,    72,    20,    73,    21,     0,    23,     0,    33,
       0,     0,    34,     0,     0,     0,    35,     0,    36,     0,
       0,     0,     0,    37,     0,     0,     0,     0,    25,    33,
     114,    39,    34,    40,    41,     0,    35,     0,    36,     0,
      42,    43,     0,    37,     0,     0,     0,     0,     0,     0,
       0,    39,     0,    40,    41,     0,     0,     0,     0,     0,
      42,    43,    84,     0,    85,    86,    87,     0,    88,     0,
      89,    90,     0,    91,     0,    92,    93,    94,    95,     0,
       0,     0,     0,     0,    96,    97,    98,    99,    84,     0,
      85,    86,    87,   113,    88,     0,    89,    90,     0,    91,
       0,    92,    93,    94,    95,     0,     0,     0,     0,     0,
      96,    97,    98,    99,   100,    84,     0,    85,    86,    87,
       0,    88,     0,    89,    90,     0,    91,     0,    92,    93,
      94,    95,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   104,    84,     0,    85,    86,    87,     0,    88,     0,
      89,    90,     0,    91,     0,    92,    93,    94,    95,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   106,    84,
       0,    85,    86,    87,     0,    88,     0,    89,    90,     0,
      91,     0,    92,    93,    94,    95,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   107,    84,     0,    85,    86,
      87,     0,    88,     0,    89,    90,     0,    91,     0,    92,
      93,    94,    95,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   108,    84,     0,    85,    86,    87,     0,    88,
       0,    89,    90,     0,    91,     0,    92,    93,    94,    95,
       0,     0,     0,     0,     0,    96,    97,    98,    99,   137,
      84,     0,    85,    86,    87,     0,    88,     0,    89,    90,
       0,    91,     0,    92,    93,    94,    95,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   143,    84,     0,    85,
      86,    87,     0,    88,     0,    89,    90,     0,    91,     0,
      92,    93,    94,    95,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   144,    84,     0,    85,    86,    87,     0,
      88,     0,    89,    90,     0,    91,     0,    92,    93,    94,
      95,     0,     0,     0,     0,     0,    96,    97,    98,    99,
     147,    84,     0,    85,    86,    87,     0,    88,     0,    89,
      90,     0,    91,     0,    92,    93,    94,    95,     0,     0,
       0,     0,     0,    96,    97,    98,    99,   151,    84,     0,
      85,    86,    87,     0,    88,     0,    89,    90,     0,    91,
       0,    92,    93,    94,    95,     0,     0,     0,     0,     0,
      96,    97,    98,    99,    84,     0,    85,    86,    87,     0,
      88,     0,    89,    90,     0,    91,     0,     0,    93,    94,
       0,     0,     0,     0,     0,     0,    96,    97,    98,    99,
      85,    86,    87,     0,    88,     0,    89,    90,     0,    91,
       0,     0,    93,    94,     0,     0,     0,     0,     0,     0,
      96,    97,    98,    99,    -1,    -1,    -1,     0,    -1,     0,
      -1,    90,     0,    -1,     0,     0,    93,    94,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99
};

static const yytype_int16 yycheck[] =
{
       6,    32,     8,     9,    25,    52,    27,    13,    14,    52,
      16,    17,     3,    19,    10,    11,    22,    32,    60,    31,
      53,    59,    34,    59,    59,    59,    38,    23,    40,    35,
      36,    37,     3,    45,    40,    41,     3,    43,     3,    52,
      52,    53,    42,    55,    56,    52,    26,    47,    48,    55,
      62,    63,    30,    52,    30,    -1,    -1,    57,    58,    -1,
      -1,    -1,    68,    69,    70,    -1,    -1,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,    -1,
     106,   107,   108,    31,    -1,   101,    34,    -1,     3,    -1,
      38,    -1,    40,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    55,    56,   135,
      -1,    -1,    -1,    61,    62,    63,    31,   143,    -1,    34,
      -1,    -1,    -1,    38,    -1,    40,    -1,    -1,    -1,    -1,
      45,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      55,    56,    -1,    -1,    -1,    -1,    61,    62,    63,     0,
       1,    -1,    -1,     4,     5,     6,     7,    -1,     9,    10,
      11,    12,    13,    -1,    15,    -1,    17,    18,    19,    20,
      21,    22,    23,    24,    -1,    26,    -1,    28,    29,    30,
      -1,    32,    -1,    -1,    -1,     1,    -1,    -1,     4,     5,
       6,     7,    -1,     9,    10,    11,    12,    13,    -1,    15,
      51,    17,    18,    19,    20,    21,    22,    23,    24,    -1,
      26,    -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,
      -1,     4,    -1,     6,    -1,     8,    -1,    -1,    11,    12,
      13,    14,    15,    16,    -1,    51,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    -1,    30,    -1,    31,
      -1,    -1,    34,    -1,    -1,    -1,    38,    -1,    40,    -1,
      -1,    -1,    -1,    45,    -1,    -1,    -1,    -1,    51,    31,
      52,    53,    34,    55,    56,    -1,    38,    -1,    40,    -1,
      62,    63,    -1,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    -1,    55,    56,    -1,    -1,    -1,    -1,    -1,
      62,    63,    33,    -1,    35,    36,    37,    -1,    39,    -1,
      41,    42,    -1,    44,    -1,    46,    47,    48,    49,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    58,    33,    -1,
      35,    36,    37,    64,    39,    -1,    41,    42,    -1,    44,
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
      55,    56,    57,    58,    33,    -1,    35,    36,    37,    -1,
      39,    -1,    41,    42,    -1,    44,    -1,    -1,    47,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    56,    57,    58,
      35,    36,    37,    -1,    39,    -1,    41,    42,    -1,    44,
      -1,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    56,    57,    58,    35,    36,    37,    -1,    39,    -1,
      41,    42,    -1,    44,    -1,    -1,    47,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    55,    56,    57,    58
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     4,     5,     6,     7,     9,    10,    11,    12,
      13,    15,    17,    18,    19,    20,    21,    22,    23,    24,
      26,    28,    29,    30,    32,    51,    66,    67,    68,    69,
      70,    71,    32,    31,    34,    38,    40,    45,    52,    53,
      55,    56,    62,    63,    72,    73,    73,    52,    73,    73,
      72,    72,    52,    73,     3,    61,    73,     3,    73,    73,
      73,    73,    72,    60,     0,    67,    32,     6,     8,    11,
      14,    16,    25,    27,    70,    71,    53,    73,    73,    73,
      73,    73,    73,    59,    33,    35,    36,    37,    39,    41,
      42,    44,    46,    47,    48,    49,    55,    56,    57,    58,
      59,    59,    59,    73,    59,    59,    59,    59,    59,    73,
      73,    73,    73,    64,    52,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    72,     3,    61,    73,    59,     3,     3,
      73,    73,    73,    59,    59,    73,     3,    59,    73,    52,
      52,    59,    52
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    65,    66,    66,    67,    67,    67,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    69,
      69,    70,    70,    71,    71,    71,    71,    71,    72,    72,
      72,    72,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     2,     1,     2,     2,     1,     2,
       3,     3,     2,     1,     1,     1,     3,     2,     5,     4,
       7,     6,     6,     2,     1,     2,     4,     1,     1,     2,
       1,     1,     2,     2,     2,     2,     2,     2,     3,     3,
       1,     1,     2,     1,     3,     1,     2,     4,     1,     2,
       4,     1,     2,     4,     5,     4,     5,     2,     4,     4,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4: /* allline: line EOL  */
#line 518 "asm/as1600.y"
            {
                clrexpr();
            }
#line 1779 "asm/as1600.c"
    break;

  case 6: /* allline: error EOL  */
#line 523 "asm/as1600.y"
            {
                clrexpr();
                yyerrok;
            }
#line 1788 "asm/as1600.c"
    break;

  case 7: /* line: labelcolon KOC_END  */
#line 530 "asm/as1600.y"
            {
                endsymbol = (yyvsp[-1].symb);
                nextreadact = Nra_end;
            }
#line 1797 "asm/as1600.c"
    break;

  case 8: /* line: KOC_END  */
#line 535 "asm/as1600.y"
            {
                nextreadact = Nra_end;
            }
#line 1805 "asm/as1600.c"
    break;

  case 9: /* line: KOC_INCLUDE STRING  */
#line 539 "asm/as1600.y"
            {
                if(nextfstk >= FILESTKDPTH)
                {
                    fraerror("include file nesting limit exceeded");
                }
                else
                {
                    infilestk[nextfstk].fnm = savestring((yyvsp[0].strng),strlen((yyvsp[0].strng)));
                    if( (infilestk[nextfstk].fpt = path_fopen((yyvsp[0].strng),"r"))
                        ==(FILE *)NULL )
                    {
                        fraerror("cannot open include file");
                    }
                    else
                    {
                        nextreadact = Nra_new;
                    }
                }
            }
#line 1829 "asm/as1600.c"
    break;

  case 10: /* line: labelcolon KOC_EQU expr  */
#line 559 "asm/as1600.y"
            {
                if((yyvsp[-2].symb)->seg == SSG_UNDEF)
                {
                    pevalexpr(0, (yyvsp[0].intv));
                    if(evalr[0].seg == SSG_ABS)
                    {
                        (yyvsp[-2].symb)->seg = SSG_EQU;
                        (yyvsp[-2].symb)->value = evalr[0].value;
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
            }
#line 1854 "asm/as1600.c"
    break;

  case 11: /* line: labelcolon KOC_SET expr  */
#line 580 "asm/as1600.y"
            {
                if((yyvsp[-2].symb)->seg == SSG_UNDEF
                   || (yyvsp[-2].symb)->seg == SSG_SET)
                {
                    pevalexpr(0, (yyvsp[0].intv));
                    if(evalr[0].seg == SSG_ABS)
                    {
                        (yyvsp[-2].symb)->seg = SSG_SET;
                        (yyvsp[-2].symb)->value = evalr[0].value;
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
            }
#line 1880 "asm/as1600.c"
    break;

  case 12: /* line: KOC_IF expr  */
#line 602 "asm/as1600.y"
            {
                if((++ifstkpt) < IFSTKDEPTH)
                {
                    pevalexpr(0, (yyvsp[0].intv));
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
            }
#line 1915 "asm/as1600.c"
    break;

  case 13: /* line: KOC_IF  */
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
            }
#line 1939 "asm/as1600.c"
    break;

  case 14: /* line: KOC_ELSE  */
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
            }
#line 1960 "asm/as1600.c"
    break;

  case 15: /* line: KOC_ENDI  */
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
            }
#line 1983 "asm/as1600.c"
    break;

  case 16: /* line: labelcolon KOC_ORG expr  */
#line 692 "asm/as1600.y"
            {
                pevalexpr(0, (yyvsp[0].intv));
                if(evalr[0].seg == SSG_ABS)
                {
                    locctr   = 2 * (labelloc = evalr[0].value);
                    currseg  = 0;
                    strcpy(currmode,"+R");
                    if((yyvsp[-2].symb)->seg == SSG_UNDEF)
                    {
                        (yyvsp[-2].symb)->seg = SSG_ABS;
                        (yyvsp[-2].symb)->value = labelloc;
                    }
                    else
                        fraerror( "multiple definition of label");

                    prtequvalue("C: 0x%lx\n", evalr[0].value);
                }
                else
                {
                    fraerror( "noncomputable expression for ORG");
                }
            }
#line 2010 "asm/as1600.c"
    break;

  case 17: /* line: KOC_ORG expr  */
#line 715 "asm/as1600.y"
            {
                pevalexpr(0, (yyvsp[0].intv));
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
            }
#line 2030 "asm/as1600.c"
    break;

  case 18: /* line: labelcolon KOC_ORG expr ',' expr  */
#line 731 "asm/as1600.y"
            {
                pevalexpr(0, (yyvsp[-2].intv));
                pevalexpr(1, (yyvsp[0].intv));
                if(evalr[0].seg == SSG_ABS && evalr[1].seg == SSG_ABS)
                {
                    locctr   = 2 * (labelloc = evalr[0].value);
                    currseg  = (evalr[1].value - labelloc);
                    strcpy(currmode, currseg ? "" : "+R");
                    if((yyvsp[-4].symb)->seg == SSG_UNDEF)
                    {
                        (yyvsp[-4].symb)->seg = SSG_ABS;
                        (yyvsp[-4].symb)->value = labelloc;
                    }
                    else
                        fraerror( "multiple definition of label");

                    prtequvalue("C: 0x%lx\n", evalr[0].value);
                }
                else
                {
                    fraerror( "noncomputable expression for ORG");
                }
            }
#line 2058 "asm/as1600.c"
    break;

  case 19: /* line: KOC_ORG expr ',' expr  */
#line 755 "asm/as1600.y"
            {
                pevalexpr(0, (yyvsp[-2].intv));
                pevalexpr(1, (yyvsp[0].intv));
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
            }
#line 2079 "asm/as1600.c"
    break;

  case 20: /* line: labelcolon KOC_ORG expr ',' expr ',' STRING  */
#line 772 "asm/as1600.y"
            {
                pevalexpr(0, (yyvsp[-4].intv));
                pevalexpr(1, (yyvsp[-2].intv));
                if(evalr[0].seg == SSG_ABS && evalr[1].seg == SSG_ABS)
                {
                    char *s = (yyvsp[0].strng);
                    if (strlen(s) > 30)
                        fraerror("Mode string is too long (max 30 chars)\n");
                    strcpy(currmode, s);

                    locctr   = 2 * (labelloc = evalr[0].value);
                    currseg  = (evalr[1].value - labelloc);

                    if((yyvsp[-6].symb)->seg == SSG_UNDEF)
                    {
                        (yyvsp[-6].symb)->seg = SSG_ABS;
                        (yyvsp[-6].symb)->value = labelloc;
                    }
                    else
                        fraerror( "multiple definition of label");

                    prtequvalue("C: 0x%lx\n", evalr[0].value);
                }
                else
                {
                    fraerror( "noncomputable expression for ORG");
                }
            }
#line 2112 "asm/as1600.c"
    break;

  case 21: /* line: KOC_ORG expr ',' expr ',' STRING  */
#line 801 "asm/as1600.y"
            {
                pevalexpr(0, (yyvsp[-4].intv));
                pevalexpr(1, (yyvsp[-2].intv));
                if(evalr[0].seg == SSG_ABS && evalr[1].seg == SSG_ABS)
                {
                    char *s = (yyvsp[0].strng);
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
            }
#line 2137 "asm/as1600.c"
    break;

  case 22: /* line: KOC_MEMATTR expr ',' expr ',' STRING  */
#line 822 "asm/as1600.y"
            {
                pevalexpr(0, (yyvsp[-4].intv));
                pevalexpr(1, (yyvsp[-2].intv));
                if(evalr[0].seg == SSG_ABS && evalr[1].seg == SSG_ABS)
                {
                    char *s = (yyvsp[0].strng);
                    if (strlen(s) > 30)
                        fraerror("Mode string is too long (max 30 chars)\n");

                    genmarec(evalr[0].value, evalr[1].value, s);
                }
                else
                {
                    fraerror(
                     "noncomputable expression for MEMATTR");
                }
            }
#line 2159 "asm/as1600.c"
    break;

  case 23: /* line: labelcolon KOC_CHSET  */
#line 841 "asm/as1600.y"
            {
                if((yyvsp[-1].symb)->seg == SSG_UNDEF)
                {
                    (yyvsp[-1].symb)->seg = SSG_EQU;
                    if( ((yyvsp[-1].symb)->value = chtcreate()) <= 0)
                    {
                        fraerror("cannot create character translation table");
                    }
                    prtequvalue("C: 0x%lx\n", (yyvsp[-1].symb)->value);
                }
                else
                {
                    fraerror("multiple definition of label");
                }
            }
#line 2179 "asm/as1600.c"
    break;

  case 24: /* line: KOC_CHUSE  */
#line 857 "asm/as1600.y"
            {
                chtcpoint = (int *) NULL;
                prtequvalue("C: 0x%lx\n", 0L);
            }
#line 2188 "asm/as1600.c"
    break;

  case 25: /* line: KOC_CHUSE expr  */
#line 862 "asm/as1600.y"
            {
                pevalexpr(0, (yyvsp[0].intv));
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
            }
#line 2217 "asm/as1600.c"
    break;

  case 26: /* line: KOC_CHDEF STRING ',' exprlist  */
#line 887 "asm/as1600.y"
            {
                int findrv, numret, *charaddr;
                char *sourcestr = (yyvsp[-2].strng), *before;

                if(chtnpoint != (int *)NULL)
                {
                    for(satsub = 0; satsub < (yyvsp[0].intv); satsub++)
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
            }
#line 2288 "asm/as1600.c"
    break;

  case 27: /* line: labelcolon  */
#line 954 "asm/as1600.y"
            {
                if((yyvsp[0].symb)->seg == SSG_UNDEF)
                {
                    (yyvsp[0].symb)->seg = SSG_ABS;
                    (yyvsp[0].symb)->value = labelloc;
                    prtequvalue("C: 0x%lx\n", labelloc);

                }
                else
                    fraerror("multiple definition of label");
            }
#line 2304 "asm/as1600.c"
    break;

  case 29: /* labeledline: labelcolon genline  */
#line 969 "asm/as1600.y"
            {
                if (sdbd)
                    frawarn("label between SDBD and instruction");

                if((yyvsp[-1].symb)->seg == SSG_UNDEF)
                {
                    (yyvsp[-1].symb)->seg   = SSG_ABS;
                    (yyvsp[-1].symb)->value = labelloc;
                }
                else
                    fraerror("multiple definition of label");

                if (locctr & 1) fraerror("internal error: PC misaligned.");

                labelloc = locctr >> 1;

                sdbd    = is_sdbd;
                is_sdbd = 0;
                first   = 0;
            }
#line 2329 "asm/as1600.c"
    break;

  case 30: /* labeledline: genline  */
#line 990 "asm/as1600.y"
            {
                if (locctr & 1) fraerror("internal error: PC misaligned.");
                labelloc = locctr >> 1;

                sdbd    = is_sdbd;
                is_sdbd = 0;
                first   = 0;
            }
#line 2342 "asm/as1600.c"
    break;

  case 33: /* genline: KOC_BDEF exprlist  */
#line 1004 "asm/as1600.y"
            {
                genlocrec(currseg, labelloc, TYPE_DATA, currmode);
                evalr[2].seg   = SSG_ABS;
                evalr[2].value = 8;
                for( satsub = 0; satsub < (yyvsp[0].intv); satsub++)
                {
                    pevalexpr(1, exprlist[satsub]);
                    locctr += geninstr(genbdef);
                }
            }
#line 2357 "asm/as1600.c"
    break;

  case 34: /* genline: KOC_DDEF exprlist  */
#line 1015 "asm/as1600.y"
            {
                genlocrec(currseg, labelloc, TYPE_DATA, currmode);
                evalr[2].seg   = SSG_ABS;
                evalr[2].value = romw;
                for( satsub = 0; satsub < (yyvsp[0].intv); satsub++)
                {
                    pevalexpr(1, exprlist[satsub]);
                    locctr += geninstr(genbdef);
                }
            }
#line 2372 "asm/as1600.c"
    break;

  case 35: /* genline: KOC_SDEF exprlist  */
#line 1027 "asm/as1600.y"
            {
                genlocrec(currseg, labelloc, TYPE_STRING, currmode);
                evalr[2].seg   = SSG_ABS;
                evalr[2].value = romw;
                for( satsub = 0; satsub < (yyvsp[0].intv); satsub++)
                {
                    pevalexpr(1, exprlist[satsub]);
                    locctr += geninstr(genbdef);
                }
            }
#line 2387 "asm/as1600.c"
    break;

  case 36: /* genline: KOC_WDEF exprlist  */
#line 1038 "asm/as1600.y"
            {
                genlocrec(currseg, labelloc, TYPE_DBDATA|TYPE_DATA, currmode);
                for( satsub = 0; satsub < (yyvsp[0].intv); satsub++)
                {
                    pevalexpr(1, exprlist[satsub]);
                    locctr += geninstr(genwdef);
                }
            }
#line 2400 "asm/as1600.c"
    break;

  case 37: /* genline: KOC_RESM expr  */
#line 1047 "asm/as1600.y"
            {
                pevalexpr(0, (yyvsp[0].intv));
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
            }
#line 2419 "asm/as1600.c"
    break;

  case 38: /* exprlist: exprlist ',' expr  */
#line 1064 "asm/as1600.y"
            {
                exprlist[nextexprs++] = (yyvsp[0].intv);
                (yyval.intv) = nextexprs;
            }
#line 2428 "asm/as1600.c"
    break;

  case 39: /* exprlist: exprlist ',' STRING  */
#line 1069 "asm/as1600.y"
            {
                char *s = (yyvsp[0].strng);
                long accval = 0;

                while (*s)
                {
                    accval = chtran(&s);
                    exprlist[nextexprs++] = 
                        exprnode(PCCASE_CONS,0,IGP_CONSTANT,0,accval,SYMNULL);
                }
                (yyval.intv) = nextexprs;
            }
#line 2445 "asm/as1600.c"
    break;

  case 40: /* exprlist: expr  */
#line 1082 "asm/as1600.y"
            {
                nextexprs = 0;
                exprlist[nextexprs++] = (yyvsp[0].intv);
                (yyval.intv) = nextexprs;
            }
#line 2455 "asm/as1600.c"
    break;

  case 41: /* exprlist: STRING  */
#line 1088 "asm/as1600.y"
            {
                char *s = (yyvsp[0].strng);
                long accval = 0;

                while (*s)
                {
                    accval = chtran(&s);
                    exprlist[nextexprs++] = 
                        exprnode(PCCASE_CONS,0,IGP_CONSTANT,0,accval,SYMNULL);
                }
                (yyval.intv) = nextexprs;
            }
#line 2472 "asm/as1600.c"
    break;

  case 42: /* genline: labelcolon KOC_PROC  */
#line 1120 "asm/as1600.y"
            {
                if (proc)
                    fraerror("Nested procedures/structures are not allowed.");

                proc     = strdup((yyvsp[-1].symb)->symstr);
                proc_len = strlen(proc);

                if((yyvsp[-1].symb)->seg == SSG_UNDEF)
                {
                    (yyvsp[-1].symb)->seg   = SSG_ABS;
                    (yyvsp[-1].symb)->value = labelloc;
                    prtequvalue("C: 0x%lx\n", labelloc);

                }
                else
                    fraerror("multiple definition of label");
            }
#line 2494 "asm/as1600.c"
    break;

  case 43: /* genline: KOC_ENDP  */
#line 1139 "asm/as1600.y"
            {
                if (!proc || struct_locctr != -1)
                    fraerror("ENDP w/out PROC.");

                free(proc);
                proc     = NULL;
                proc_len = 0;
            }
#line 2507 "asm/as1600.c"
    break;

  case 44: /* genline: labelcolon KOC_STRUCT expr  */
#line 1152 "asm/as1600.y"
            {
                if (proc)
                    fraerror("Nested procedures/structures are not allowed.");

                proc     = strdup((yyvsp[-2].symb)->symstr);
                proc_len = strlen(proc);
                struct_locctr = locctr;

                pevalexpr(0, (yyvsp[0].intv));
                if(evalr[0].seg == SSG_ABS)
                {
                    locctr = 2 * (labelloc = evalr[0].value);
                    if((yyvsp[-2].symb)->seg == SSG_UNDEF)
                    {
                        (yyvsp[-2].symb)->seg = SSG_ABS;
                        (yyvsp[-2].symb)->value = labelloc;
                    }
                    else
                        fraerror( "multiple definition of label");

                    prtequvalue("C: 0x%lx\n", evalr[0].value);
                }
                else
                {
                    fraerror( "noncomputable expression for ORG");
                }
            }
#line 2539 "asm/as1600.c"
    break;

  case 45: /* genline: KOC_ENDS  */
#line 1181 "asm/as1600.y"
            {
                if (!proc || struct_locctr == -1)
                    fraerror("ENDS w/out STRUCT.");

                free(proc);
                proc     = NULL;
                proc_len = 0;
                locctr = struct_locctr;
                struct_locctr = -1;
            }
#line 2554 "asm/as1600.c"
    break;

  case 46: /* genline: KOC_ROMW expr  */
#line 1196 "asm/as1600.y"
            {
                genlocrec(currseg, labelloc, TYPE_HOLE, currmode);
                pevalexpr(0, (yyvsp[0].intv));
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
            }
#line 2583 "asm/as1600.c"
    break;

  case 47: /* genline: KOC_ROMW expr ',' expr  */
#line 1221 "asm/as1600.y"
            {
                genlocrec(currseg, labelloc, TYPE_HOLE, currmode);
                pevalexpr(0, (yyvsp[-2].intv));
                pevalexpr(1, (yyvsp[0].intv));
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

            }
#line 2619 "asm/as1600.c"
    break;

  case 48: /* genline: KOC_SDBD  */
#line 1258 "asm/as1600.y"
            {
                if (sdbd)
                    frawarn("Two SDBDs in a row.");

                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                locctr += geninstr(findgen((yyvsp[0].intv), ST_IMP, 0));
                is_sdbd = SDBD;
            }
#line 2632 "asm/as1600.c"
    break;

  case 49: /* genline: KOC_relbr expr  */
#line 1272 "asm/as1600.y"
            {
                unsigned rel_addr = labelloc + 2;
                int dir;

                SDBD_CHK

                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                pevalexpr(1, (yyvsp[0].intv));

                evalr[3].seg   = SSG_ABS;
                evalr[3].value = romw;

                locctr += geninstr(findgen((yyvsp[-1].intv), ST_EXP, sdbd));
            }
#line 2651 "asm/as1600.c"
    break;

  case 50: /* genline: KOC_relbr_x expr ',' expr  */
#line 1288 "asm/as1600.y"
            {
                unsigned rel_addr = labelloc + 2;
                int dir;

                SDBD_CHK

                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                pevalexpr(1, (yyvsp[-2].intv));
                pevalexpr(4, (yyvsp[0].intv));

                if (evalr[4].seg != SSG_ABS)
                    fraerror("Must have constant expr for BEXT condition");

                evalr[3].seg   = SSG_ABS;
                evalr[3].value = romw;

                locctr += geninstr(findgen((yyvsp[-3].intv), ST_EXPEXP, sdbd));
            }
#line 2674 "asm/as1600.c"
    break;

  case 51: /* genline: KOC_opcode  */
#line 1313 "asm/as1600.y"
            {
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                locctr += geninstr(findgen((yyvsp[0].intv), ST_IMP, sdbd));
            }
#line 2684 "asm/as1600.c"
    break;

  case 52: /* genline: KOC_opcode expr  */
#line 1324 "asm/as1600.y"
            {
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                pevalexpr(1, (yyvsp[0].intv));
                locctr += geninstr(findgen((yyvsp[-1].intv), ST_EXP, sdbd));
            }
#line 2695 "asm/as1600.c"
    break;

  case 53: /* genline: KOC_opcode REGISTER ',' expr  */
#line 1337 "asm/as1600.y"
            {
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                evalr[1].value = (yyvsp[-2].intv);
                pevalexpr(2, (yyvsp[0].intv));
                evalr[3].seg    = SSG_ABS;
                evalr[3].value  = romw;
                locctr += geninstr(findgen((yyvsp[-3].intv), ST_REGEXP, reg_type[(yyvsp[-2].intv)]|sdbd));
            }
#line 2709 "asm/as1600.c"
    break;

  case 54: /* genline: KOC_opcode REGISTER ',' '#' expr  */
#line 1352 "asm/as1600.y"
            {
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                evalr[1].value  = (yyvsp[-3].intv);
                evalr[3].seg    = SSG_ABS;
                evalr[3].value  = romw;
                pevalexpr(2, (yyvsp[0].intv));
                locctr += geninstr(findgen((yyvsp[-4].intv), ST_REGCEX, reg_type[(yyvsp[-3].intv)]|sdbd));
            }
#line 2723 "asm/as1600.c"
    break;

  case 55: /* genline: KOC_opcode expr ',' REGISTER  */
#line 1367 "asm/as1600.y"
            {
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                pevalexpr(1, (yyvsp[-2].intv));
                evalr[2].value = (yyvsp[0].intv);
                evalr[3].seg   = SSG_ABS;
                evalr[3].value = romw;
                locctr += geninstr(findgen((yyvsp[-3].intv), ST_EXPREG, reg_type[(yyvsp[0].intv)]|sdbd));
            }
#line 2737 "asm/as1600.c"
    break;

  case 56: /* genline: KOC_opcode '#' expr ',' REGISTER  */
#line 1382 "asm/as1600.y"
            {
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                pevalexpr(1, (yyvsp[-2].intv));
                evalr[2].value = (yyvsp[0].intv);

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

                locctr += geninstr(findgen((yyvsp[-4].intv), ST_CEXREG, reg_type[(yyvsp[0].intv)]|sdbd));
            }
#line 2771 "asm/as1600.c"
    break;

  case 57: /* genline: KOC_opcode REGISTER  */
#line 1417 "asm/as1600.y"
            {
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                evalr[1].value = (yyvsp[0].intv);
                locctr += geninstr(findgen((yyvsp[-1].intv), ST_REG, reg_type[(yyvsp[0].intv)]|sdbd));
            }
#line 2782 "asm/as1600.c"
    break;

  case 58: /* genline: KOC_opcode REGISTER ',' REGISTER  */
#line 1429 "asm/as1600.y"
            {
                SDBD_CHK
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                evalr[1].value = (yyvsp[-2].intv);
                evalr[2].value = (yyvsp[0].intv);
                locctr += geninstr(findgen((yyvsp[-3].intv), ST_REGREG, reg_type[(yyvsp[-2].intv)]|sdbd));
            }
#line 2794 "asm/as1600.c"
    break;

  case 59: /* genline: KOC_opcode_i REGISTER ',' REGISTER  */
#line 1442 "asm/as1600.y"
            {
                genlocrec(currseg, labelloc, TYPE_CODE, currmode);
                evalr[1].value = (yyvsp[-2].intv);
                evalr[2].value = (yyvsp[0].intv);
                locctr += geninstr(findgen((yyvsp[-3].intv), ST_REGREG, reg_type[(yyvsp[-2].intv)]|sdbd));
            }
#line 2805 "asm/as1600.c"
    break;

  case 60: /* expr: '+' expr  */
#line 1455 "asm/as1600.y"
            {
                (yyval.intv) = (yyvsp[0].intv);
            }
#line 2813 "asm/as1600.c"
    break;

  case 61: /* expr: '-' expr  */
#line 1459 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_UN,(yyvsp[0].intv),IFC_NEG,0,0L, SYMNULL);
            }
#line 2821 "asm/as1600.c"
    break;

  case 62: /* expr: KEOP_NOT expr  */
#line 1463 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_UN,(yyvsp[0].intv),IFC_NOT,0,0L, SYMNULL);
            }
#line 2829 "asm/as1600.c"
    break;

  case 63: /* expr: KEOP_HIGH expr  */
#line 1467 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_UN,(yyvsp[0].intv),IFC_HIGH,0,0L, SYMNULL);
            }
#line 2837 "asm/as1600.c"
    break;

  case 64: /* expr: KEOP_LOW expr  */
#line 1471 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_UN,(yyvsp[0].intv),IFC_LOW,0,0L, SYMNULL);
            }
#line 2845 "asm/as1600.c"
    break;

  case 65: /* expr: expr '*' expr  */
#line 1475 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_MUL,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2853 "asm/as1600.c"
    break;

  case 66: /* expr: expr '/' expr  */
#line 1479 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_DIV,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2861 "asm/as1600.c"
    break;

  case 67: /* expr: expr '+' expr  */
#line 1483 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_ADD,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2869 "asm/as1600.c"
    break;

  case 68: /* expr: expr '-' expr  */
#line 1487 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_SUB,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2877 "asm/as1600.c"
    break;

  case 69: /* expr: expr KEOP_MOD expr  */
#line 1491 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_MOD,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2885 "asm/as1600.c"
    break;

  case 70: /* expr: expr KEOP_SHL expr  */
#line 1495 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_SHL,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2893 "asm/as1600.c"
    break;

  case 71: /* expr: expr KEOP_SHR expr  */
#line 1499 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_SHR,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2901 "asm/as1600.c"
    break;

  case 72: /* expr: expr KEOP_GT expr  */
#line 1503 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_GT,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2909 "asm/as1600.c"
    break;

  case 73: /* expr: expr KEOP_GE expr  */
#line 1507 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_GE,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2917 "asm/as1600.c"
    break;

  case 74: /* expr: expr KEOP_LT expr  */
#line 1511 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_LT,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2925 "asm/as1600.c"
    break;

  case 75: /* expr: expr KEOP_LE expr  */
#line 1515 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_LE,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2933 "asm/as1600.c"
    break;

  case 76: /* expr: expr KEOP_NE expr  */
#line 1519 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_NE,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2941 "asm/as1600.c"
    break;

  case 77: /* expr: expr KEOP_EQ expr  */
#line 1523 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_EQ,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2949 "asm/as1600.c"
    break;

  case 78: /* expr: expr KEOP_AND expr  */
#line 1527 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_AND,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2957 "asm/as1600.c"
    break;

  case 79: /* expr: expr KEOP_OR expr  */
#line 1531 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_OR,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2965 "asm/as1600.c"
    break;

  case 80: /* expr: expr KEOP_XOR expr  */
#line 1535 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_BIN,(yyvsp[-2].intv),IFC_XOR,(yyvsp[0].intv),0L, SYMNULL);
            }
#line 2973 "asm/as1600.c"
    break;

  case 81: /* expr: KEOP_DEFINED SYMBOL  */
#line 1539 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_DEF,0,IGP_DEFINED,0,0L,(yyvsp[0].symb));
            }
#line 2981 "asm/as1600.c"
    break;

  case 82: /* expr: SYMBOL  */
#line 1543 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_SYMB,0,IFC_SYMB,0,0L,(yyvsp[0].symb));
            }
#line 2989 "asm/as1600.c"
    break;

  case 83: /* expr: '$'  */
#line 1547 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_PROGC,0,IFC_PROGCTR,0,labelloc,SYMNULL);
            }
#line 2997 "asm/as1600.c"
    break;

  case 84: /* expr: CONSTANT  */
#line 1551 "asm/as1600.y"
            {
                (yyval.intv) = exprnode(PCCASE_CONS,0,IGP_CONSTANT,0,(yyvsp[0].longv), SYMNULL);
            }
#line 3005 "asm/as1600.c"
    break;

  case 85: /* expr: '(' expr ')'  */
#line 1555 "asm/as1600.y"
            {
                (yyval.intv) = (yyvsp[-1].intv);
            }
#line 3013 "asm/as1600.c"
    break;


#line 3017 "asm/as1600.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
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
