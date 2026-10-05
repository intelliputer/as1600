/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_ASM_AS1600_H_INCLUDED
# define YY_YY_ASM_AS1600_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    REGISTER = 258,                /* REGISTER  */
    KOC_BDEF = 259,                /* KOC_BDEF  */
    KOC_ELSE = 260,                /* KOC_ELSE  */
    KOC_END = 261,                 /* KOC_END  */
    KOC_ENDI = 262,                /* KOC_ENDI  */
    KOC_EQU = 263,                 /* KOC_EQU  */
    KOC_IF = 264,                  /* KOC_IF  */
    KOC_INCLUDE = 265,             /* KOC_INCLUDE  */
    KOC_ORG = 266,                 /* KOC_ORG  */
    KOC_RESM = 267,                /* KOC_RESM  */
    KOC_SDEF = 268,                /* KOC_SDEF  */
    KOC_SET = 269,                 /* KOC_SET  */
    KOC_WDEF = 270,                /* KOC_WDEF  */
    KOC_CHSET = 271,               /* KOC_CHSET  */
    KOC_CHDEF = 272,               /* KOC_CHDEF  */
    KOC_CHUSE = 273,               /* KOC_CHUSE  */
    KOC_opcode = 274,              /* KOC_opcode  */
    KOC_opcode_i = 275,            /* KOC_opcode_i  */
    KOC_relbr = 276,               /* KOC_relbr  */
    KOC_relbr_x = 277,             /* KOC_relbr_x  */
    KOC_SDBD = 278,                /* KOC_SDBD  */
    KOC_ROMW = 279,                /* KOC_ROMW  */
    KOC_PROC = 280,                /* KOC_PROC  */
    KOC_ENDP = 281,                /* KOC_ENDP  */
    KOC_STRUCT = 282,              /* KOC_STRUCT  */
    KOC_ENDS = 283,                /* KOC_ENDS  */
    KOC_MEMATTR = 284,             /* KOC_MEMATTR  */
    KOC_DDEF = 285,                /* KOC_DDEF  */
    CONSTANT = 286,                /* CONSTANT  */
    EOL = 287,                     /* EOL  */
    KEOP_AND = 288,                /* KEOP_AND  */
    KEOP_DEFINED = 289,            /* KEOP_DEFINED  */
    KEOP_EQ = 290,                 /* KEOP_EQ  */
    KEOP_GE = 291,                 /* KEOP_GE  */
    KEOP_GT = 292,                 /* KEOP_GT  */
    KEOP_HIGH = 293,               /* KEOP_HIGH  */
    KEOP_LE = 294,                 /* KEOP_LE  */
    KEOP_LOW = 295,                /* KEOP_LOW  */
    KEOP_LT = 296,                 /* KEOP_LT  */
    KEOP_MOD = 297,                /* KEOP_MOD  */
    KEOP_MUN = 298,                /* KEOP_MUN  */
    KEOP_NE = 299,                 /* KEOP_NE  */
    KEOP_NOT = 300,                /* KEOP_NOT  */
    KEOP_OR = 301,                 /* KEOP_OR  */
    KEOP_SHL = 302,                /* KEOP_SHL  */
    KEOP_SHR = 303,                /* KEOP_SHR  */
    KEOP_XOR = 304,                /* KEOP_XOR  */
    KEOP_locctr = 305,             /* KEOP_locctr  */
    LABEL = 306,                   /* LABEL  */
    STRING = 307,                  /* STRING  */
    SYMBOL = 308,                  /* SYMBOL  */
    KTK_invalid = 309              /* KTK_invalid  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 434 "asm/as1600.y"

    int intv;
    long    longv;
    char    *strng;
    struct symel *symb;

#line 125 "asm/as1600.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_ASM_AS1600_H_INCLUDED  */
