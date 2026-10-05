;; ======================================================================== ;;
;;  DIST_FAST Approximate the distance between two points to 4% error.      ;;
;;  DIST_FAST.1 Alternate entry point                                       ;;
;;                                                                          ;;
;;  AUTHOR                                                                  ;;
;;      Joseph Zbiciak <im14u2c@primenet.com>                               ;;
;;                                                                          ;;
;;  REVISION HISTORY                                                        ;;
;;      22-Apr-2002 Initial Revision                                        ;;
;;                                                                          ;;
;;  INPUTS for DIST_FAST                                                    ;;
;;      R0    Coordinate x0                                                 ;;
;;      R1    Coordinate y0                                                 ;;
;;      R2    Coordinate x1                                                 ;;
;;      R3    Coordinate y1                                                 ;;
;;      R5    Return address                                                ;;
;;                                                                          ;;
;;  INPUTS for DIST_FAST.1                                                  ;;
;;      R2    Absolute value of delta-X (eg. abs(x0 - x1))                  ;;
;;      R3    Absolute value of delta-Y (eg. abs(y0 - y1))                  ;;
;;      R5    Return address                                                ;;
;;                                                                          ;;
;;  OUTPUTS                                                                 ;;
;;      R0    Unchanged                                                     ;;
;;      R1    Trashed                                                       ;;
;;      R2    Trashed                                                       ;;
;;      R3    Approximate Euclidean distance from (x0, y0) to (x1, y1).     ;;
;;                                                                          ;;
;;  NOTES                                                                   ;;
;;      This returns a value which is approximately sqrt(dx**2 + dy**2),    ;;
;;      within 4%.  This should be sufficient for most applications.        ;;
;;                                                                          ;;
;;      The code accepts unsigned 16-bit values for the incoming            ;;
;;      coordinates, and should correctly calculate abs(x0 - x1)            ;;
;;      and abs(y0 - y1) for the full 16-bit range.                         ;;
;;                                                                          ;;
;;      The output should have the same dynamic range as the inputs.        ;;
;;      Thus, if the inputs are in an 8-bit range, so will be the           ;;
;;      output.  If the inputs are in a 16-bit range, so is the output.     ;;
;;                                                                          ;;
;;      Although the arguments list R0 and R2 as the X coordinates          ;;
;;      and R1 and R3 as the Y coordinates, these assignments may be        ;;
;;      swapped.  (eg. R1 and R3 are X coordinates, R0 and R2 are Y         ;;
;;      coordinates.)  Euclidean distance doesn't change when you rotate    ;;
;;      the page 90 degrees or flip it over.  :-)                           ;;
;;                                                                          ;;
;;  SOURCE                                                                  ;;
;;      Graphics Gems IV (see C code below)                                 ;;
;;                                                                          ;;
;;      C code from the article                                             ;;
;;      "Fast Linear Approximations of Euclidean Distance in                ;;
;;      Higher Dimensions" by Yoshikazu Ohashi, yoshi@cognex.com            ;;
;;      in "Graphics Gems IV", Academic Press, 1994                         ;;
;;                                                                          ;;
;;      /* 2-D Euclidean distance approximation          */                 ;;
;;      /* c1 = 123/128, c2 = 51/128 and max(e) = 4.0%   */                 ;;
;;      int veclen2 (int ix, int iy)                                        ;;
;;      {                                                                   ;;
;;          int  t;                                                         ;;
;;                                                                          ;;
;;          ix= (ix<0 ? -ix : ix);   /* absolute values */                  ;;
;;          iy= (iy<0 ? -iy : iy);                                          ;;
;;                                                                          ;;
;;          if(ix<iy)        /* swap ix and iy if (ix < iy) */              ;;
;;          {                /* See Wyvill (G1, 436)        */              ;;
;;              ix=ix^iy;                                                   ;;
;;              iy=ix^iy;                                                   ;;
;;              ix=ix^iy;                                                   ;;
;;          }                                                               ;;
;;                                                                          ;;
;;          t = iy + (iy>>1);                                               ;;
;;                                                                          ;;
;;          /* return (123 * ix + 51 * iy) / 128; */                        ;;
;;          return (ix - (ix>>5) - (ix>>7)  + (t>>2) + (t>>6));             ;;
;;      }                                                                   ;;
;;                                                                          ;;
;;  CODESIZE                                                                ;;
;;      28 words                                                            ;;
;;                                                                          ;;
;;  CYCLES                                                                  ;;
;;      Approx 180 cycles.  (Not completely characterized.)                 ;;
;;                                                                          ;;
;;  LICENSE                                                                 ;;
;;      This implementatation is governed by the GNU GPL.  The C code       ;;
;;      above comes from Graphics Gems IV, and is bound by the terms of     ;;
;;      that book.                                                          ;;
;;                                                                          ;;
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

DIST_FAST   PROC
            SUBR    R0,     R2
            ADCR    PC          ; Skip negr if R2 was greater than R0
            NEGR    R2

            SUBR    R1,     R3
            ADCR    PC          ; Skip negr if R3 was greater than R1
            NEGR    R3

@@1:        CMPR    R2,     R3  ; Put larger value in R3
            BC      @@no_swap
            XORR    R2,     R3  ;\
            XORR    R3,     R2  ; |-- Swap R2 and R3 if necessary.
            XORR    R2,     R3  ;/
@@no_swap:
            MOVR    R2,     R1
            SLR     R1,     1
            ADDR    R1,     R2  ; t = iy + (iy >> 1)

            MOVR    R3,     R1  ;
            SLR     R1,     2   ;
            SLR     R1,     2   ;
            SLR     R1,     1   ; R1 = ix >> 5
            SUBR    R1,     R3  ; R3 = ix - (ix>>5)
            SLR     R1,     2   ; R1 = ix >> 7
            SUBR    R1,     R3  ; R3 = ix - (ix>>5) - (ix>>7)
            SLR     R2,     2   ; R2 = t >> 2
            ADDR    R2,     R3  ; R3 = ix - (ix>>5) - (ix>>7) + (t>>2)
            SLR     R2,     2
            SLR     R2,     2   ; R2 = t >> 6
            ADDR    R2,     R3  ; R3 = ix - (ix>>5) - (ix>>7) + (t>>2) + (t>>6)

            JR      R5
            ENDP

;; ======================================================================== ;;
;;  End of File:  dist_fast.asm                                             ;;
;; ======================================================================== ;;
