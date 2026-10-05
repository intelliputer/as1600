;; ======================================================================== ;;
;;  NAME                                                                    ;;
;;      SQRT        Calculate the square root of a fixed-point number       ;;
;;      SQRT.1      Calculate the square root of an integer                 ;;
;;      SQRT.2      Calculate the square root of a fixed-point number       ;;
;;                                                                          ;;
;;  AUTHOR                                                                  ;;
;;      Joseph Zbiciak <im14u2c@primenet.com>                               ;;
;;                                                                          ;;
;;  REVISION HISTORY                                                        ;;
;;      12-Sep-2001 Initial revision . . . . . . . . . . .  J. Zbiciak      ;;
;;                                                                          ;;
;;  INPUTS for SQRT                                                         ;;
;;      R1      Unsigned 16-bit argument to SQRT()                          ;;
;;      R5      Pointer to DECLE containing Qpt, followed by return addr.   ;;
;;                                                                          ;;
;;  INPUTS for SQRT.1                                                       ;;
;;      R1      Unsigned 16-bit argument to SQRT()                          ;;
;;      R5      Return address                                              ;;
;;                                                                          ;;
;;  INPUTS for SQRT.2                                                       ;;
;;      R0      Qpt for fixed-point value                                   ;;
;;      R1      Unsigned 16-bit argument to SQRT()                          ;;
;;      R5      Return address                                              ;;
;;                                                                          ;;
;;  OUTPUTS                                                                 ;;
;;      R0      Zeroed                                                      ;;
;;      R1      Unmodified                                                  ;;
;;      R2      SQRT(R1)                                                    ;;
;;      R3, R4  Unmodified                                                  ;;
;;      R5      Trashed                                                     ;;
;;                                                                          ;;
;;  NOTES                                                                   ;;
;;      The way this code handles odd Q-points on fixed-point numbers is    ;;
;;      by right-shifting the incoming value 1 bit, thus making the         ;;
;;      Q-point even.  This has the negative effect of losing precision     ;;
;;      on odd Q-point numbers.  Rectifying this without losing any         ;;
;;      performance would require significantly larger codesize.            ;;
;;                                                                          ;;
;;  CODESIZE                                                                ;;
;;      44 words                                                            ;;
;;                                                                          ;;
;;  CYCLES                                                                  ;;
;;      cycles = 141 + 71*(8 + Qpt/2) worst case for SQRT                   ;;
;;      cycles = 132 + 61*(8 + Qpt/2) best case for SQRT                    ;;
;;                                                                          ;;
;;      Subtract 4 cycles if Qpt is even.                                   ;;
;;      Subtract 8 cycles if calling SQRT.1.                                ;;
;;      Subtract 14 cycles if calling SQRT.2.                               ;;
;;                                                                          ;;
;;  SOURCE                                                                  ;;
;;      Loosely based on a C code example (mborg_isqrt2) by Paul Hseih and  ;;
;;      Mark Borgerding, found on the web here:                             ;;
;;          http://www.azillionmonkeys.com/qed/sqroot.html                  ;;
;;                                                                          ;;
;;  LICENSE                                                                 ;;
;;      GNU General Public License, Version 2.0:                            ;;
;;                                                                          ;;
;;      This program is free software; you can redistribute it and/or       ;;
;;      modify it under the terms of the GNU General Public License as      ;;
;;      published by the Free Software Foundation; either version 2 of the  ;;
;;      License, or (at your option) any later version.                     ;;
;;                                                                          ;;
;;      This program is distributed in the hope that it will be useful,     ;;
;;      but WITHOUT ANY WARRANTY; without even the implied warranty of      ;;
;;      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU   ;;
;;      General Public License for more details.                            ;;
;;                                                                          ;;
;;      You should have received a copy of the GNU General Public License   ;;
;;      along with this program; if not, write to the Free Software         ;;
;;      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.           ;;
;; ------------------------------------------------------------------------ ;;
;;                   Copyright (c) 2001, Joseph Zbiciak                     ;;
;; ======================================================================== ;;

SQRT        PROC
            MVI@    R5,     R0      ;   8 Get Qpt from after CALL
            INCR    PC              ;   6 (skip CLRR R0)
@@1:        CLRR    R0              ;   6 Set Qpt == 0
@@2:        PSHR    R5              ;   9 Alt entry point w/ all args in regs.
            PSHR    R1              ;   9 save R1
            PSHR    R3              ;   9 save R3
                                    ;----
                                    ;  41 (worst case: SQRT)
                                    ;  33 (if SQRT.1)           
                                    ;  27 (if SQRT.2)          
                                         
            CLRR    R2              ;   6 R2 == Result word
            MVII    #$4000, R3      ;   8 R3 == 1/2 of square of test bit
            MOVR    R3,     R5      ;   6 R5 == bit * (2*guess + bit)
                                         
            INCR    R0              ;   6
            SARC    R0,     1       ;   6 Check to see if Qpt is odd
            BC      @@even_q        ; 7/9
            SLR     R1,     1       ;   6 Note: We lose LSB if odd Q
@@even_q:                                
                                         
            ADDI    #8,     R0      ;   8
            B       @@first         ;   9
                                    ;----
                                    ;  60 (worst case, q is ODD)
                                    ;  56 (q is EVEN)            
                                    ;====
                                    ;  83 (worst case: SQRT, q is ODD)
                                         
@@loop:     SLLC    R1,     1       ;   6 Shift the value left by 1
            BC      @@b1            ; 7/9 MSB was 1, force guessed bit to 1.
                                         
@@first:    CMPR    R5,     R1      ;   6 Is (guess+bit)**2 <= val?
            BNC     @@b0            ; 7/9 C==0 means the bit should be 0
                                         
@@b1:       RLC     R2,     1       ;   6 Yes:  Set bit in result and 
            SUBR    R5,     R1      ;   6       subtract guess from value
            ADDR    R3,     R5      ;   6 \
            SLR     R3,     1       ;   6  |-- Calculate next guess value
            ADDR    R3,     R5      ;   6 /
            DECR    R0              ;   6
            BNEQ    @@loop          ; 9/7 Guess next bit
            B       @@done          ;   9
                                    ;----
                                    ;  71*k + 7 worst case
                                         
@@b0:       SLL     R2,     1       ;   6 No:  Clear bit in guessed result
            SLR     R3,     1       ;   6 \__ Calculate next guess
            SUBR    R3,     R5      ;   6 /
            DECR    R0              ;   6
            BNEQ    @@loop          ; 9/7 Guess next bit
                                    ;----
                                    ;  61*k - 2 best case

@@done:     PULR    R3              ;  11 Restore R3
            PULR    R1              ;  11 Restore R1
            PULR    PC              ;  11 Return
            ENDP

;; ======================================================================== ;;
;;  End of function: SQRT                                                   ;;
;; ======================================================================== ;;
