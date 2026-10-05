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
;*                  Copyright (c) 1999-2000, Joe Zbiciak                    *;
;* ======================================================================== *;

        ORG     $1000
        ROMW    10


        JD      EXEC
DUMMY:  MOVR    R5,     R7

;
; HDWRINT
; Hardware Interrupt Routine
; This routine saves the machine state and vectors to the software
; interrupt vector at $100..$101
;

HDWRINT PROC
        PSHR    R0
        GSWD    R0
        PSHR    R0
        PSHR    R1
        PSHR    R2
        PSHR    R3
        PSHR    R4
        PSHR    R5
        SDBD
        MVII    #@@return, R5
        MVII    #$0100, R4
        SDBD
        MVI@    R4,     R7
@@return:
        PULR    R5
        PULR    R4
        PULR    R3
        PULR    R2
        PULR    R1
        PULR    R0
        RSWD    R0
        PULR    R0
        PULR    R7
        ENDP

MINIISR PROC
        MVO     R0,     $20     ; Enable the display
        JR      R5
        ENDP

EXEC    PROC
        CLRR    R0
        MVII    #$1F0,  R4
        MVII    #$00E,  R1
@@loop:
        MVO@    R0,     R4      ; Clear out PSG. ($1F0..$1FD)
        DECR    R1
        BNEQ    @@loop

        MVII    #DUMMY, R0
        MVO     R0,     $100    ; Hook dummy routine into ISR vector
        SWAP    R0,     1
        MVO     R0,     $101

        MVII    #$2F0,  R6      ; Set up stack pointer.
        MVII    #$5014, R4      ; Rom header address
        PSHR    R4

        MVII    #$1000, R5      ; Set return address to mini-EXEC ROM start.

        MVII    #$3FF,  R1
        COMR    R1              ; 10-bit ROM mask.
        MOVR    R1,     R0      ; need two copies

        MVII    #$48,   R3      
        SWAP    R3              ; R3 == $4800
        AND@    R3,     R1      ; Check for viable ROM at $4800
        BEQ     @@viablerom
        MVII    #$70,   R3      
        SWAP    R3              ; R3 == $7000
        AND@    R3,     R0      ; Check for viable ROM at $7000
        BNEQ    @@notviablerom
@@viablerom:
        EIS
        MOVR    R3,     PC
@@notviablerom:

        ; Jump to the START address in the std ROM header location instead.
        EIS
        SUBI    #$10,   R4      ; R4 = $5004 now
        SDBD
        MVI@    R4,     PC      ; Jump to program start address.

        ENDP


        ; Padding to fill out EXEC image to make it exactly a 4K ROM.
        RMB     $11FF - $
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
        RMB     $FF
	BYTE    0
