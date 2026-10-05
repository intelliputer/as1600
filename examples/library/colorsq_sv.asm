;;==========================================================================;;
;;  Colored squares pixel-manipulation routines.                            ;;
;;  Copyright 1999-2002, Joe Zbiciak.                                       ;;
;;                                                                          ;;
;;  Contained in this file:                                                 ;;
;;                                                                          ;;
;;      PUTPIXELS   -- Put a pixel.  Saves and restores X, Y, and C         ;;
;;      GETPIXELS   -- Get a pixel.  Saves and restores X, Y, and C         ;;
;;                                                                          ;;
;;  This file requires the routines in colorsq.asm.                         ;;
;;--------------------------------------------------------------------------;;
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

;; ======================================================================== ;;
;;  Note on X/Y/CSAVE:  The storage used for these only needs to be as      ;;
;;  wide as the values that you're passing into PUTPIXELS and GETPIXELS.    ;;
;; ======================================================================== ;;

                        ; Used by         Req'd Width   Description
                        ;-----------------------------------------------------
;CSAVE  EQU     $110    ; PUT/GETPIXELS   8 or 16-bit   Temp. storage
;XSAVE  EQU     $111    ; PUT/GETPIXELS   8 or 16-bit   Temp. storage
;YSAVE  EQU     $112    ; PUT/GETPIXELS   8 or 16-bit   Temp. storage
                        ;-----------------------------------------------------


;; ======================================================================== ;;
;;  GETPIXELS -- Calls GETPIXEL, but saves/restores X, Y                    ;;
;;                                                                          ;;
;;  INPUTS:                                                                 ;;
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
GETPIXELS       PROC
        PSHR    R5
@@chain:
        MVO     R1,     XSAVE
        MVO     R2,     YSAVE
        CALL    GETPIXEL
        MVI     XSAVE,  R1
        MVI     YSAVE,  R2
        PULR    PC
        ENDP


;; ======================================================================== ;;
;;  PUTPIXELS -- Calls PUTPIXEL, but saves/restores X, Y                    ;;
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
PUTPIXELS       PROC
        PSHR    R5
@@chain:
        MVO     R1,     XSAVE
        MVO     R2,     YSAVE
        MVO     R0,     CSAVE
        CALL    PUTPIXEL
        MVI     CSAVE,  R0
        MVI     XSAVE,  R1
        MVI     YSAVE,  R2
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
;*                   Copyright (c) 2000, Joseph Zbiciak                     *;
;* ======================================================================== *;
