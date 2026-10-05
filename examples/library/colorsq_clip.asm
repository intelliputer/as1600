;;==========================================================================;;
;;  Colored squares pixel-manipulation routines                             ;;
;;  Copyright 1999-2002, Joe Zbiciak.                                       ;;
;;                                                                          ;;
;;  Contained in this file:                                                 ;;
;;                                                                          ;;
;;      PIXELCLIP   -- Returns if C==1 if pixel is onscreen                 ;;
;;      PUTPIXELSC  -- Put a pixel if it's onscreen. Save/restore X/Y/C     ;;
;;      GETPIXELSC  -- Get a pixel if it's onscreen. Save/restore X/Y/C     ;;
;;                                                                          ;;
;;  This file requires the routines in colorsq.asm and colorsq_sv.asm.      ;;
;;--------------------------------------------------------------------------;;
;;  This file contains a number of useful routines that you're welcome      ;;
;;  to use in your own software.  Please keep in mind that these routines   ;;
;;  are licensed under the GNU General Public License, and so if you plan   ;;
;;  to distribute a program which incorporates these routines, it too must  ;;
;;  be distributed under the GNU General Public License.                    ;;
;;==========================================================================;;


;; ======================================================================== ;;
;;  PIXELCLIP                                                               ;;
;;  Returns with C==1 if pixel is onscreen                                  ;;
;;                                                                          ;;
;;  INPUTS:                                                                 ;;
;;      R1 -- X coordinate                                                  ;;
;;      R2 -- Y coordinate                                                  ;;
;;      R5 -- Return address                                                ;;
;;                                                                          ;;
;;  OUTPUTS:                                                                ;;
;;      Carry == 1 if onscreen, 0 if offscreen.                             ;;
;; ======================================================================== ;;

PIXELCLIP       PROC
        TSTR    R1              ; Off left?
        BMI     @@offscr        ; Yes:  Set carry and exit
        TSTR    R2              ; Off top?
        BMI     @@offscr        ; Yes:  Set carry and exit
        CMPI    #39,    R1      ; Off right?
        BGT     @@offscr        ; Yes:  Set carry and exit
        CMPI    #23,    R2      ; Off bottom?
        BGT     @@offscr        ; Yes:  Set carry and exit
        SETC                    ; No to all: Clear carry and exit
        JR      R5
@@offscr
        CLRC
        JR      R5
        ENDP

;; ======================================================================== ;;
;;  GETPIXELSC                                                              ;;
;;  Calls GETPIXELS or returns R0 unchanged if pixel is off screen          ;;
;;                                                                          ;;
;;  INPUTS:                                                                 ;;
;;      R0 -- Color to return if off-screen                                 ;;
;;      R1 -- X coordinate                                                  ;;
;;      R2 -- Y coordinate                                                  ;;
;;      R5 -- Return addr.                                                  ;;
;;                                                                          ;;
;;  OUTPUTS                                                                 ;;
;;      R0 -- Color                                                         ;;
;;      R1 -- X coordinate                                                  ;;
;;      R2 -- Y coordinate                                                  ;;
;;      R3, R4, R5 -- Clobbered.                                            ;;
;; ======================================================================== ;;
GETPIXELSC      PROC
        PSHR    R5              ; Save return address
        CALL    PIXELCLIP       ; See if pixel is onscreen
        ADCR    PC              ; If it is, then skip the return
        PULR    PC              ; Otherwise return.
        B       GETPIXELS.chain ; If onscreen chain over to GETPIXELS
        ENDP

;; ======================================================================== ;;
;;  PUTPIXELSC                                                              ;;
;;  Calls PUTPIXELS if the pixel is onscreen.                               ;;
;;                                                                          ;;
;;  INPUTS:                                                                 ;;
;;      R0 -- Color                                                         ;;
;;      R1 -- X coordinate                                                  ;;
;;      R2 -- Y coordinate                                                  ;;
;;      R5 -- Return addr.                                                  ;;
;;                                                                          ;;
;;  OUTPUTS                                                                 ;;
;;      R0 -- Color                                                         ;;
;;      R1 -- X coordinate                                                  ;;
;;      R2 -- Y coordinate                                                  ;;
;;      R3, R4, R5 -- Clobbered.                                            ;;
;; ======================================================================== ;;
PUTPIXELSC      PROC
        PSHR    R5              ; Save return address
        CALL    PIXELCLIP       ; See if pixel is onscreen
        ADCR    PC              ; If it is, then skip the return
        PULR    PC              ; Otherwise return.
        B       PUTPIXELS.chain ; If onscreen chain over to GETPIXELS
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
;*                   Copyright (c) 2000, Joseph Zbiciak                     *;
;* ======================================================================== *;
