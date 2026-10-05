;;==========================================================================;;
;;  Useful Routines                                                         ;;
;;  Copyright 1999-2002, Joe Zbiciak.                                       ;;
;;                                                                          ;;
;;  This file contains a number of useful routines that you're welcome      ;;
;;  to use in your own software.  Please keep in mind that these routines   ;;
;;  are licensed under the GNU General Public License, and so if you plan   ;;
;;  to distribute a program which incorporates these routines, it too must  ;;
;;  be distributed under the GNU General Public License.                    ;;
;;==========================================================================;;

;; ======================================================================== ;;
;;  GLOBAL VARIABLES USED BY THESE ROUTINES                                 ;;
;;                                                                          ;;
;;  Note that some of these routines may use one or more global variables.  ;;
;;  If you use these routines, you will need to allocate the appropriate    ;;
;;  space in either 16-bit or 8-bit memory as appropriate.  Each global     ;;
;;  variable is listed with the routines which use it and the required      ;;
;;  memory width.                                                           ;;
;;                                                                          ;;
;;  Example declarations for these routines are shown below, commented out. ;;
;;  You should uncomment these and add them to your program to make use of  ;;
;;  the routine that needs them.  Make sure to assign these variables to    ;;
;;  locations that aren't used for anything else.                           ;;
;; ======================================================================== ;;

                        ; Used by       Req'd Width     Description
                        ;-----------------------------------------------------
;RNDLO  EQU     $320    ; RAND          16-bit          Random number state
;RNDHI  EQU     $321    ; RAND          16-bit          Random number state

;; ======================================================================== ;;
;;  RAND                                                                    ;;
;;      Returns random bits in R0.                                          ;;
;;                                                                          ;;
;;  INPUTS:                                                                 ;;
;;      R0 -- Number of bits desired                                        ;;
;;      R5 -- Return address                                                ;;
;;      Random state in RNDLO, RNDHI                                        ;;
;;                                                                          ;;
;;  OUTPUTS:                                                                ;;
;;      R0 -- N random bits.                                                ;;
;;      R1, R2, R3, R4 -- Saved and restored                                ;;
;;      R5 -- trashed.                                                      ;;
;;                                                                          ;;
;;  NOTES:                                                                  ;;
;;      You are encouraged to add additional "randomness" by adding or      ;;
;;      XORing other values into RNDLO or RNDHI.  Also, to initialize       ;;
;;      the random number generator, ensure that RNDLO and RNDHI are        ;;
;;      non-zero.                                                           ;;
;; ======================================================================== ;;
RAND    PROC
        PSHR    R5              ; Save return address and R1..R4
        PSHR    R4
        PSHR    R3
        PSHR    R2
        PSHR    R1

        MVII    #1,     R1      ; Our initial mask word
        TSTR    R0              ; Is R0 > 0?
        BEQ     @@nobits

        MVII    #$04C1, R5      ; period==(2**32 - 1) polynomial 
        MVII    #$1DB7, R4      ; (this is the CRC-32 polynomial)
        MVI     RNDHI,  R3      ; Read in our 32-bit random number state
        MVI     RNDLO,  R2

        TSTR    R3              ; If our random number generator is zero
        BNEQ    @@loop          ; jumpstart the process by forcing an XOR
        TSTR    R2              ; of our generator polynomal into R2/R3
        SETC                    ; up front.  Otherwise, we won't generate
        BEQ     @@forceit       ; any random numbers!
        
@@loop:
        SLLC    R2,     1       ; Shift our 32-bit random number left by 1
        RLC     R3,     1       ; ... by using the carry and an RLC.
@@forceit:
        SLL     R1,     1       ; Shift our mask bit left by 1
        BNC     @@nocarry
        XORR    R4,     R2      ; If the carry was set, XOR in our generator
        XORR    R5,     R3      ; polynomial.  
@@nocarry:
        DECR    R0              ; Keep generating bits.
        BNEQ    @@loop

        MVO     R3,     RNDHI   ; Store our new random number state.
        MVO     R2,     RNDLO

@@nobits:
        DECR    R1              ; Turn our mask bit into a mask word
        ANDR    R1,     R2      ; Mask the bits we actually want.
        MOVR    R2,     R0      ; Return our result in R0

        PULR    R1              ; Retstore our registers and return.
        PULR    R2
        PULR    R3
        PULR    R4
        PULR    PC

        ENDP

;* ======================================================================== *;
;*  This program is free software; you can redistribute it and/or modify    *;
;*  it under the terms of the GNU General Public License as published by    *;
;*  the Free Software Foundation; either version 2 of the License, or       *;
;*  (at your option) any later version.                                     *;
;*                                                                          *;
;*  This program is distributed in the hope that it will be useful,         *;
;*  but WITHOUT ANY WARRANTY; without even the implied warranty of          *;
;*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       *;
;*  General Public License for more details.                                *;
;*                                                                          *;
;*  You should have received a copy of the GNU General Public License       *;
;*  along with this program; if not, write to the Free Software             *;
;*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               *;
;* ======================================================================== *;
;*                Copyright (c) 1999-2002, Joseph Zbiciak                   *;
;* ======================================================================== *;
