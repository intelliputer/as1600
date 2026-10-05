;; ======================================================================== ;;
;;  MEMCMP    Compare Two Arrays                                            ;;
;;  MEMCMP.1  Alternate entry point                                         ;;
;;                                                                          ;;
;;  AUTHOR                                                                  ;;
;;      Joseph Zbiciak <im14u2c@primenet.com>                               ;;
;;                                                                          ;;
;;  REVISION HISTORY                                                        ;;
;;      08-Sep-2001 Initial Revision                                        ;;
;;                                                                          ;;
;;  INPUTS for MEMCMP                                                       ;;
;;      R5    Pointer to invocation record, followed by return address.     ;;
;;            Pointer to first array       1 DECLE                          ;;
;;            Pointer to second array      1 DECLE                          ;;
;;            Length                       1 DECLE                          ;;
;;                                                                          ;;
;;  INPUTS for MEMCMP.1                                                     ;;
;;      R5    Return address                                                ;;
;;      R4    Pointer to first array                                        ;;
;;      R1    Pointer to second array                                       ;;
;;      R0    Length                                                        ;;
;;                                                                          ;;
;;  OUTPUTS                                                                 ;;
;;      R0    Zero if all elements compared as equal, non-zero otherwise.   ;;
;;      R1    Last element compared against from first array.               ;;
;;      R4    If miscompared, points just after miscompare in 1st array,    ;;
;;            otherwise points to first element beyond end of array.        ;;
;;      R5    If miscompared, points just after miscompare in 2nd array,    ;;
;;            otherwise points to first element beyond end of array.        ;;
;;      FLAGS Result of comparison between the arrays.                      ;;
;;                                                                          ;;
;;  TECHNIQUES                                                              ;;
;;      Unrolled 2x for speed.                                              ;;
;;                                                                          ;;
;;  CODESIZE                                                                ;;
;;      23 words                                                            ;;
;;                                                                          ;;
;;  CYCLES                                                                  ;;
;;      If length is even:  cycles =  74 + 61*floor(length / 2)             ;;
;;      If length is odd:   cycles = 107 + 61*floor(length / 2)             ;;
;;                                                                          ;;
;;      If calling MEMCMP.1, subtract 24 cycles from the above formulae.    ;;
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
;;                   Copyright (c) 2002, Joseph Zbiciak                     ;;
;; ======================================================================== ;;

MEMCMP      PROC

            MVI@    R5,     R4      ;   8   First array
            MVI@    R5,     R1      ;   8   Second array
            MVI@    R5,     R0      ;   8   Length

@@1:        PSHR    R5              ;   9   Alternate entry point

            MOVR    R1,     R5      ;   6   Use auto-incr pointer

            INCR    R0              ;   6   \    Loop is unrolled 2x, so 
            SARC    R0,     1       ;   6    |-- handle the odd iteration.
            BNC     @@odd           ;  7/9  /   
            BEQ     @@done          ;  7/9  Abort if length == 0.
                                    ;---- 
                                    ;  65   Fallthru case
                                    ;  60   Branch to odd

@@loop:     MVI@    R5,     R1      ;   8   \
            CMP@    R4,     R1      ;   8    |-- Compare one pair of words
            BNEQ    @@done          ;  7/9  /
            
@@odd:      MVI@    R5,     R1      ;   8   \
            CMP@    R4,     R1      ;   8    |-- Compare one pair of words
            BNEQ    @@done          ;  7/9  /
            
            DECR    R0              ;   6   
            BNEQ    @@loop          ;  9/7  Iterate
                                    ;----
                                    ;  61*k - 2

@@done:     PULR    PC              ;  11   Return, w/ result in FLAGS.
            ENDP    


;; ======================================================================== ;;
;;  End of File:  memcmp.asm                                                ;;
;; ======================================================================== ;;
