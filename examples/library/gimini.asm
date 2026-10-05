;;==========================================================================;;
;; GIMINI structure definitions.  Defines the memory map and peripherals.   ;;
;;==========================================================================;;

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



;; ======================================================================== ;;
;;  Overall Memory Map                                                      ;;
;;  MEMMAP -- Structure containing overall system memory map.               ;;
;; ======================================================================== ;;
MEMMAP          STRUCT  0
@@stic          EQU     $0000 ;..$003F  ; Standard Television Interface Circuit
@@voice         EQU     $0080 ;..$0081  ; Voice Synthesizer
@@psg1          EQU     $00F0 ;..$00FF  ; Secondary Programmable Sound Gen.
@@dataram       EQU     $0100 ;..$01EF  ; 8-bit Data RAM
@@psg0          EQU     $01F0 ;..$01FF  ; Primary Programmable Sound Generator
@@backtab       EQU     $0200 ;..$02EF  ; 16-bit Display RAM
@@sysram        EQU     $02F0 ;..$035F  ; 16-bit System RAM (incl. Stack)
@@exec2         EQU     $0400 ;..$04FF  ; Executive ROM expansion
@@exec          EQU     $1000 ;..$1FFF  ; Executive ROM
@@grom          EQU     $3000 ;..$37FF  ; Graphics ROM
@@gram          EQU     $3800 ;..$39FF  ; Graphics RAM
@@gram_alias1   EQU     $3A00 ;..$3BFF  ; Graphics RAM alias 
@@gram_alias2   EQU     $3C00 ;..$3DFF  ; Graphics RAM alias
@@gram_alias3   EQU     $3E00 ;..$3FFF  ; Graphics RAM alias
@@stic_alias1   EQU     $4000 ;..$403F  ; STIC alias (incomplete decode)
@@stic_alias2   EQU     $8000 ;..$803F  ; STIC alias (incomplete decode)
                ENDS



;;==========================================================================;;
;; AY8914 Programmable Sound Generator Register Definitions.                ;;
;;                                                                          ;;
;; PSG0 -- Primary PSG in the Gimini Console.                               ;;
;; PSG1 -- Secondary PSG, available when expansion is attached.             ;;
;; PSG  -- Constants and bitfields common to both PSG's.                    ;;
;;                                                                          ;;
;; Each PSG contains the following set of registers:                        ;;
;;                                                                          ;;
;;     Register pairs:                                                      ;;
;;                                                                          ;;
;;       7   6   5   4   3   2   1   0 | 7   6   5   4   3   2   1   0      ;;
;;     +---------------+---------------|-------------------------------+    ;;
;;  R4 |    unused     |                Channel A Period               | R0 ;;
;;     +---------------+---------------|-------------------------------+    ;;
;;  R5 |    unused     |                Channel B Period               | R1 ;;
;;     +---------------+---------------|-------------------------------+    ;;
;;  R6 |    unused     |                Channel C Period               | R2 ;;
;;     +---------------+---------------|-------------------------------+    ;;
;;  R7 |                        Envelope Period                        | R3 ;;
;;     +-------------------------------|-------+-----------------------+    ;;
;;                                                                          ;;
;;     Single registers:                                                    ;;
;;                                                                          ;;
;;         7       6       5       4       3       2       1       0        ;;
;;     +---------------+-----------------------+-----------------------+    ;;
;;     | I/O Port Dir  |    Noise Enables      |     Tone Enables      |    ;;
;;  R8 |   0   |   0   |   C   |   B   |   A   |   C   |   B   |   A   |    ;;
;;     +-------+-------+-------+-------+-------+-------+-------+-------+    ;;
;;  R9 |            unused     |              Noise Period             |    ;;
;;     +-----------------------+-------+-------+-------+-------+-------+    ;;
;;     |                               |   Envelope Characteristics    |    ;;
;; R10 |            unused             | CONT  |ATTACK | ALTER | HOLD  |    ;;
;;     +---------------+---------------+-------+-------+-------+-------+    ;;
;; R11 |    unused     | A Envl Select |    Channel A Volume Level     |    ;;
;;     +---------------+---------------+-------------------------------+    ;;
;; R12 |    unused     | B Envl Select |    Channel B Volume Level     |    ;;
;;     +---------------+---------------+-------------------------------+    ;;
;; R13 |    unused     | C Envl Select |    Channel C Volume Level     |    ;;
;;     +---------------+---------------+-------------------------------+    ;;
;;                                                                          ;;
;;==========================================================================;;

                ;;==========================================================;;
                ;; Primary PSG in the main console.                         ;;
                ;;==========================================================;;
PSG0            STRUCT  $01F0

                ;;----------------------------------------------------------;;
                ;; Register address definitions                             ;;
                ;;----------------------------------------------------------;;
@@chn_a_lo      EQU     $ + 0   ; Channel A period, lower 8 bits of 12
@@chn_b_lo      EQU     $ + 1   ; Channel B period, lower 8 bits of 12
@@chn_c_lo      EQU     $ + 2   ; Channel C period, lower 8 bits of 12
@@envlp_lo      EQU     $ + 3   ; Envelope period,  lower 8 bits of 16

@@chn_a_hi      EQU     $ + 4   ; Channel A period, upper 4 bits of 12
@@chn_b_hi      EQU     $ + 5   ; Channel B period, upper 4 bits of 12
@@chn_c_hi      EQU     $ + 6   ; Channel C period, upper 4 bits of 12
@@envlp_hi      EQU     $ + 7   ; Envelope period,  upper 8 bits of 16

@@chan_enable   EQU     $ + 8   ; Channel enables (bits 3-5 noise, 0-2 tone)
@@noise         EQU     $ + 9   ; Noise period (5 bits)
@@envelope      EQU     $ + 10  ; Envelope type/trigger (4 bits)

@@chn_a_vol     EQU     $ + 11  ; Channel A volume / Envelope select (6 bits)
@@chn_b_vol     EQU     $ + 12  ; Channel B volume / Envelope select (6 bits)
@@chn_c_vol     EQU     $ + 13  ; Channel C volume / Envelope select (6 bits)

@@io_port0      EQU     $ + 14  ; I/O port 0 (8 bits)
@@io_port1      EQU     $ + 15  ; I/O port 1 (8 bits)

                ENDS

                ;;==========================================================;;
                ;; Secondary PSG in the expansion unit.                     ;;
                ;;==========================================================;;
PSG1            STRUCT  $00F0

                ;;----------------------------------------------------------;;
                ;; Register address definitions                             ;;
                ;;----------------------------------------------------------;;
@@chn_a_lo      EQU     $ + 0   ; Channel A period, lower 8 bits of 12
@@chn_b_lo      EQU     $ + 1   ; Channel B period, lower 8 bits of 12
@@chn_c_lo      EQU     $ + 2   ; Channel C period, lower 8 bits of 12
@@envlp_lo      EQU     $ + 3   ; Envelope period,  lower 8 bits of 16

@@chn_a_hi      EQU     $ + 4   ; Channel A period, upper 4 bits of 12
@@chn_b_hi      EQU     $ + 5   ; Channel B period, upper 4 bits of 12
@@chn_c_hi      EQU     $ + 6   ; Channel C period, upper 4 bits of 12
@@envlp_hi      EQU     $ + 7   ; Envelope period,  upper 8 bits of 16

@@chan_enable   EQU     $ + 8   ; Channel enables (bits 3-5 noise, 0-2 tone)
@@noise         EQU     $ + 9   ; Noise period (5 bits)
@@envelope      EQU     $ + 10  ; Envelope type/trigger (4 bits)

@@chn_a_vol     EQU     $ + 11  ; Channel A volume / Envelope select (6 bits)
@@chn_b_vol     EQU     $ + 12  ; Channel B volume / Envelope select (6 bits)
@@chn_c_vol     EQU     $ + 13  ; Channel C volume / Envelope select (6 bits)

@@io_port0      EQU     $ + 14  ; I/O port 0 (8 bits)
@@io_port1      EQU     $ + 15  ; I/O port 1 (8 bits)

                ENDS

                ;;==========================================================;;
                ;; Useful Constants / Bit-field definitions.                ;;
                ;;==========================================================;;
PSG             STRUCT  $0000   ; Constants, etc. common to both PSGs.

                ;;----------------------------------------------------------;;
                ;; Bits to OR together for Channel Enable word              ;;
                ;;----------------------------------------------------------;;
@@tone_a_on     EQU     00000000b   
@@tone_b_on     EQU     00000000b
@@tone_c_on     EQU     00000000b
@@noise_a_on    EQU     00000000b
@@noise_b_on    EQU     00000000b
@@noise_c_on    EQU     00000000b

@@tone_a_off    EQU     00000001b
@@tone_b_off    EQU     00000010b
@@tone_c_off    EQU     00000100b
@@noise_a_off   EQU     00001000b
@@noise_b_off   EQU     00010000b
@@noise_c_off   EQU     00100000b

                ;;----------------------------------------------------------;;
                ;; Bits to OR together for Envelope Type                    ;;
                ;;----------------------------------------------------------;;
@@envl_cont     EQU     00001000b   ; Continue
@@envl_attack   EQU     00000100b   ; Attack (vs. Decay)
@@envl_alter    EQU     00000010b   ; Alternate
@@envl_hold     EQU     00000001b   ; Hold

                ;;----------------------------------------------------------;;
                ;; Volume levels                                            ;;
                ;;----------------------------------------------------------;;
@@vol_0         EQU     0
@@vol_1         EQU     1
@@vol_2         EQU     2
@@vol_3         EQU     3
@@vol_4         EQU     4
@@vol_5         EQU     5
@@vol_6         EQU     6
@@vol_7         EQU     7
@@vol_8         EQU     8
@@vol_9         EQU     9
@@vol_10        EQU     10
@@vol_11        EQU     11
@@vol_12        EQU     12
@@vol_13        EQU     13
@@vol_14        EQU     14
@@vol_15        EQU     15
@@vol_envl      EQU     63

                ENDS

;;==========================================================================;;
;; AY8900 Standard Television Interface Circuit                             ;;
;;                                                                          ;;
;; STIC -- Register definitions, constants and bitfields                    ;;
;;                                                                          ;;
;; The Standard Television Interface Circuit provides a method for placing  ;;
;; characters and graphics on the screen.  It provides a 20x12 matrix of    ;;
;; background 'cards'.  Each card contains an 8x8 tile of pixels.  It also  ;;
;; provides 8 movable objects (refered to here as MOBs).                    ;;
;;                                                                          ;;
;; MOB controls: (Note: 'mob' is MOB number (0 - 7))                        ;;
;;  mob + $00   -- X position, and some attribute bits.                     ;;
;;  mob + $08   -- Y position, and some attribute bits.                     ;;
;;  mob + $10   -- Character definition, more attribute bits.               ;;
;;  mob + $18   -- Collision information.                                   ;;
;;                                                                          ;;
;; Display Mode controls:                                                   ;;
;;  $20         -- Display enable (must be written during vert blank.)      ;;
;;  $21         -- Graphics mode (write sets FGBG, read sets Color Stack)   ;;
;;                                                                          ;;
;; Color Stack and Border Color controls:                                   ;;
;;  $28..$2B    -- Color stack entries 0..3                                 ;;
;;  $2C         -- Screen border color                                      ;;
;;                                                                          ;;
;; Display framing controls:                                                ;;
;;  $30         -- Pixel column delay (0..7)                                ;;
;;  $31         -- Pixel row delay (0..7)                                   ;;
;;  $32         -- Edge masking (bit 0 masks left, bit 1 masks top)         ;;
;;                                                                          ;;
;; Color Set                                                                ;;
;;       Primary Color Set                Pastel Color Set                  ;;
;;    ------------------------       -----------------------------          ;;
;;     0 Black   4 Dark Green          8 Grey     12 Pink                   ;;
;;     1 Blue    5 Green               9 Cyan     13 Light Blue             ;;
;;     2 Red     6 Yellow             10 Orange   14 Yellow-Green           ;;
;;     3 Tan     7 White              11 Brown    15 Purple                 ;;
;;                                                                          ;;
;;==========================================================================;;
STIC            STRUCT  $0000

;;--------------------------------------------------------------------------;;
;; MOB Controls                                                             ;;
;;                                                                          ;;
;; X Register layout:                                                       ;;
;;                                                                          ;;
;;    13   12   11   10    9    8    7    6    5    4    3    2    1    0   ;;
;;  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+ ;;
;;  | ?? | ?? | ?? | X  |VISB|INTR|            X Coordinate               | ;;
;;  |    |    |    |SIZE|    |    |             (0 to 255)                | ;;
;;  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+ ;;
;;                                                                          ;;
;; Y Register layout:                                                       ;;
;;                                                                          ;;
;;    13   12   11   10    9    8    7    6    5    4    3    2    1    0   ;;
;;  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+ ;;
;;  | ?? | ?? | Y  | X  | Y  | Y  |YRES|          Y Coordinate            | ;;
;;  |    |    |FLIP|FLIP|SIZ4|SIZ2|    |           (0 to 127)             | ;;
;;  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+ ;;
;;                                                                          ;;
;; A Register layout:                                                       ;;
;;                                                                          ;;
;;    13   12   11   10    9    8    7    6    5    4    3    2    1    0   ;;
;;  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+ ;;
;;  |PRIO| FG |GRAM|      GRAM/GROM Card # (0 to 255)      |   FG Color   | ;;
;;  |    |bit3|GROM|     (bits 9, 10 ignored for GRAM)     |   Bits 0-2   | ;;
;;  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+ ;;
;;                                                                          ;;
;; C Register layout:                                                       ;;
;;                                                                          ;;
;;    13   12   11   10    9    8    7    6    5    4    3    2    1    0   ;;
;;  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+ ;;
;;  | ?? | ?? | ?? | ?? |COLL|COLL|COLL|COLL|COLL|COLL|COLL|COLL|COLL|COLL| ;;
;;  |    |    |    |    |BORD| BG |MOB7|MOB6|MOB5|MOB4|MOB3|MOB2|MOB1|MOB0| ;;
;;  +----+----+----+----+----+----+----+----+----+----+----+----+----+----+ ;;
;;--------------------------------------------------------------------------;;

@@mob0_x        EQU     $00 + 0     ; MOB 0 X position, XSIZE/VIS attributes
@@mob1_x        EQU     $00 + 1     ; MOB 1 X position, XSIZE/VIS attributes
@@mob2_x        EQU     $00 + 2     ; MOB 2 X position, XSIZE/VIS attributes
@@mob3_x        EQU     $00 + 3     ; MOB 3 X position, XSIZE/VIS attributes
@@mob4_x        EQU     $00 + 4     ; MOB 4 X position, XSIZE/VIS attributes
@@mob5_x        EQU     $00 + 5     ; MOB 5 X position, XSIZE/VIS attributes
@@mob6_x        EQU     $00 + 6     ; MOB 6 X position, XSIZE/VIS attributes
@@mob7_x        EQU     $00 + 7     ; MOB 7 X position, XSIZE/VIS attributes

@@mob0_y        EQU     $08 + 0     ; MOB 0 Y pos'n, YRES/YSIZE/XFLIP/YFLIP
@@mob1_y        EQU     $08 + 1     ; MOB 1 Y pos'n, YRES/YSIZE/XFLIP/YFLIP
@@mob2_y        EQU     $08 + 2     ; MOB 2 Y pos'n, YRES/YSIZE/XFLIP/YFLIP
@@mob3_y        EQU     $08 + 3     ; MOB 3 Y pos'n, YRES/YSIZE/XFLIP/YFLIP
@@mob4_y        EQU     $08 + 4     ; MOB 4 Y pos'n, YRES/YSIZE/XFLIP/YFLIP
@@mob5_y        EQU     $08 + 5     ; MOB 5 Y pos'n, YRES/YSIZE/XFLIP/YFLIP
@@mob6_y        EQU     $08 + 6     ; MOB 6 Y pos'n, YRES/YSIZE/XFLIP/YFLIP
@@mob7_y        EQU     $08 + 7     ; MOB 7 Y pos'n, YRES/YSIZE/XFLIP/YFLIP

@@mob0_a        EQU     $10 + 0     ; MOB 0 Color, Card #, Priority
@@mob1_a        EQU     $10 + 1     ; MOB 1 Color, Card #, Priority
@@mob2_a        EQU     $10 + 2     ; MOB 2 Color, Card #, Priority
@@mob3_a        EQU     $10 + 3     ; MOB 3 Color, Card #, Priority
@@mob4_a        EQU     $10 + 4     ; MOB 4 Color, Card #, Priority
@@mob5_a        EQU     $10 + 5     ; MOB 5 Color, Card #, Priority
@@mob6_a        EQU     $10 + 6     ; MOB 6 Color, Card #, Priority
@@mob7_a        EQU     $10 + 7     ; MOB 7 Color, Card #, Priority

@@mob0_c        EQU     $18 + 0     ; MOB 0 Collision detect
@@mob1_c        EQU     $18 + 1     ; MOB 1 Collision detect
@@mob2_c        EQU     $18 + 2     ; MOB 2 Collision detect
@@mob3_c        EQU     $18 + 3     ; MOB 3 Collision detect
@@mob4_c        EQU     $18 + 4     ; MOB 4 Collision detect
@@mob5_c        EQU     $18 + 5     ; MOB 5 Collision detect
@@mob6_c        EQU     $18 + 6     ; MOB 6 Collision detect
@@mob7_c        EQU     $18 + 7     ; MOB 7 Collision detect

                ;;----------------------------------------------------------;;
                ;; Display Mode Controls                                    ;;
                ;;----------------------------------------------------------;;
@@viden         EQU     $20         ; Display Enable (write during vblank)
@@mode          EQU     $21         ; Mode select 

                ;;----------------------------------------------------------;;
                ;; Color Stack and Display Border Color Controls            ;;
                ;;----------------------------------------------------------;;
@@colstack      EQU     $28         ; Base of the color stack
@@cs0           EQU     $28 + 0     ; Color Stack 0
@@cs1           EQU     $28 + 1     ; Color Stack 1
@@cs2           EQU     $28 + 2     ; Color Stack 2
@@cs3           EQU     $28 + 3     ; Color Stack 3
@@bord          EQU     $2C         ; Border color

                ;;----------------------------------------------------------;;
                ;; Display Framing Controls                                 ;;
                ;;----------------------------------------------------------;;
@@h_delay       EQU     $30         ; Horizontal delay (0 - 7 pixels)
@@v_delay       EQU     $31         ; Vertical delay (0 - 7 pixels)
@@edgemask      EQU     $32         ; Edge masking

                ;;----------------------------------------------------------;;
                ;; Useful bitfields/constants/masks for MOBS                ;;
                ;;----------------------------------------------------------;;
@@mobx_xpos     EQU     00000011111111b     ; MOB XREG: X position
@@mobx_intr     EQU     00000100000000b     ; MOB XREG: Interaction
@@mobx_visb     EQU     00001000000000b     ; MOB XREG: Visibility
@@mobx_xsize    EQU     00010000000000b     ; MOB XREG: Horiz 2x magnification

@@moby_ypos     EQU     00000001111111b     ; MOB YREG: Y position
@@moby_yres     EQU     00000010000000b     ; MOB YREG: Y res. (8 or 16 rows)
@@moby_ysize2   EQU     00000100000000b     ; MOB YREG: Vert 2x magnification
@@moby_ysize4   EQU     00001000000000b     ; MOB YREG: Vert 4x magnification
@@moby_ysize8   EQU     00001100000000b     ; MOB YREG: 8x mag (sets 4x & 2x)
@@moby_xflip    EQU     00010000000000b     ; MOB YREG: Flip horizontally
@@moby_yflip    EQU     00100000000000b     ; MOB YREG: Flip vertically

@@moba_fg0      EQU     00000000000000b     ; MOB AREG: Foreground color =  0
@@moba_fg1      EQU     00000000000001b     ; MOB AREG: Foreground color =  1
@@moba_fg2      EQU     00000000000010b     ; MOB AREG: Foreground color =  2
@@moba_fg3      EQU     00000000000011b     ; MOB AREG: Foreground color =  3
@@moba_fg4      EQU     00000000000100b     ; MOB AREG: Foreground color =  4
@@moba_fg5      EQU     00000000000101b     ; MOB AREG: Foreground color =  5
@@moba_fg6      EQU     00000000000110b     ; MOB AREG: Foreground color =  6
@@moba_fg7      EQU     00000000000111b     ; MOB AREG: Foreground color =  7
@@moba_fg8      EQU     01000000000000b     ; MOB AREG: Foreground color =  8
@@moba_fg9      EQU     01000000000001b     ; MOB AREG: Foreground color =  9
@@moba_fgA      EQU     01000000000010b     ; MOB AREG: Foreground color = 10
@@moba_fgB      EQU     01000000000011b     ; MOB AREG: Foreground color = 11
@@moba_fgC      EQU     01000000000100b     ; MOB AREG: Foreground color = 12
@@moba_fgD      EQU     01000000000101b     ; MOB AREG: Foreground color = 13
@@moba_fgE      EQU     01000000000110b     ; MOB AREG: Foreground color = 14
@@moba_fgF      EQU     01000000000111b     ; MOB AREG: Foreground color = 15
@@moba_card     EQU     00000111111000b     ; MOB AREG: Card # mask
@@moba_gram     EQU     00100000000000b     ; MOB AREG: GRAM card select
@@moba_prio     EQU     10000000000000b     ; MOB AREG: Priority 
                                            ;           (above/below bkgnd)

@@mobc_coll0    EQU     00000000000001b     ; MOB CREG: Collision w/ MOB #0
@@mobc_coll1    EQU     00000000000010b     ; MOB CREG: Collision w/ MOB #1
@@mobc_coll2    EQU     00000000000100b     ; MOB CREG: Collision w/ MOB #2
@@mobc_coll3    EQU     00000000001000b     ; MOB CREG: Collision w/ MOB #3
@@mobc_coll4    EQU     00000000010000b     ; MOB CREG: Collision w/ MOB #4
@@mobc_coll5    EQU     00000000100000b     ; MOB CREG: Collision w/ MOB #5
@@mobc_coll6    EQU     00000001000000b     ; MOB CREG: Collision w/ MOB #6
@@mobc_coll7    EQU     00000010000000b     ; MOB CREG: Collision w/ MOB #7
@@mobc_collmob  EQU     00000011111111b     ; MOB CREG: Coll w/ any MOB (mask)
@@mobc_collbg   EQU     00000100000000b     ; MOB CREG; Coll w/ background
@@mobc_collbord EQU     00001000000000b     ; MOB CREG; Coll w/ background

                ;;----------------------------------------------------------;;
                ;; Useful bits for Edge Masking.                            ;;
                ;;----------------------------------------------------------;;
@@mask_left     EQU     00000000000001b     ; Edge mask: Mask leftmost 8 pixels
@@mask_top      EQU     00000000000010b     ; Edge mask: Mask topmost 8 pixels

;;--------------------------------------------------------------------------;;
;; Useful bits for Color Stack Mode                                         ;;
;;                                                                          ;;
;; Display format word layout in Color Stack Mode:                          ;;
;;   13   12    11   10    9    8    7    6    5    4    3    2    1    0   ;;
;; +----+-----+----+----+----+----+----+----+----+----+----+----+----+----+ ;;
;; |Adv.|FG   |GRAM|           GRAM/GROM Card #            |   FG Color   | ;;
;; |col |Bit3/|GROM|    (0-255 for GROM, 0-63 for GRAM)    |   Bits 0-2   | ;;
;; |stck|----------|                                       |              | ;;
;; |    |col. sqr. |                                       |              | ;;
;; |    |mode slct.|                                       |              | ;;
;; +----+-----+----+----+----+----+----+----+----+----+----+----+----+----+ ;;
;;                                                                          ;;
;; Color Stack Notes:                                                       ;;
;;  -- If GRAM card, two MSBs of Card # are ignored, and may be used by     ;;
;;     the program to store other information.                              ;;
;;                                                                          ;;
;;  -- Bit 12 is set and Bit 11 is cleared, this word is treated as a       ;;
;;     colored-square-mode display word, in the format below.               ;;
;;                                                                          ;;
;;  -- The color stack is reset to offset 0 at the start of the display.    ;;
;;     Setting the 'advance' bit advances the color stack by one for that   ;;
;;     card and all cards after it in normal left-to-right scanning order.  ;;
;;                                                                          ;;
;;  -- Bits 14 and 15 of the display format word are ignored and may be     ;;
;;     used by the program to store status bits, etc.                       ;;
;;                                                                          ;;
;; Display format word layout in Colored Squares Mode:                      ;;
;;                                                                          ;;
;;   13   12   11   10    9    8    7    6    5    4    3    2    1    0    ;;
;; +----+----+----+----+----+----+----+----+----+----+----+----+----+----+  ;;
;; |Pix3|  1 |  0 | Pix. 3  | Pix. 2 color | Pix. 1 color | Pix. 0 color |  ;;
;; |Bit2|    |    | bit 0-1 |    (0-7)     |    (0-7)     |    (0-7)     |  ;;
;; +----+----+----+----+----+----+----+----+----+----+----+----+----+----+  ;;
;;                                                                          ;;
;; Pixels are display like so:  +-----+-----+                               ;;
;;                              |Pixel|Pixel|                               ;;
;;                              |  0  |  1  |                               ;;
;;                              +-----+-----+                               ;;
;;                              |Pixel|Pixel|                               ;;
;;                              |  2  |  3  |                               ;;
;;                              +-----+-----+                               ;;
;;                                                                          ;;
;; Colored Square Mode Notes:                                               ;;
;;                                                                          ;;
;;  -- It is not possible to advance the color stack with a card that is    ;;
;;     displayed in color-stack mode.                                       ;;
;;                                                                          ;;
;;  -- Color 7 in colored squares mode instead shows the current color on   ;;
;;     the color stack.                                                     ;;
;;                                                                          ;;
;;  -- Colors 0 through 6 in a colored-square card will interact with       ;;
;;     MOBs, but color 7 will not.                                          ;;
;;                                                                          ;;
;;  -- Bits 14 and 15 of the display format word are ignored and may be     ;;
;;     used by the program to store status bits, etc.                       ;;
;;--------------------------------------------------------------------------;;
@@cs_fg0        EQU     00000000000000b     ; foreground ==  0
@@cs_fg1        EQU     00000000000001b     ; foreground ==  1
@@cs_fg2        EQU     00000000000010b     ; foreground ==  2
@@cs_fg3        EQU     00000000000011b     ; foreground ==  3
@@cs_fg4        EQU     00000000000100b     ; foreground ==  4
@@cs_fg5        EQU     00000000000101b     ; foreground ==  5
@@cs_fg6        EQU     00000000000110b     ; foreground ==  6
@@cs_fg7        EQU     00000000000111b     ; foreground ==  7
@@cs_fg8        EQU     01000000000000b     ; foreground ==  8
@@cs_fg9        EQU     01000000000001b     ; foreground ==  9
@@cs_fgA        EQU     01000000000010b     ; foreground == 10
@@cs_fgB        EQU     01000000000011b     ; foreground == 11
@@cs_fgC        EQU     01000000000100b     ; foreground == 12
@@cs_fgD        EQU     01000000000101b     ; foreground == 13
@@cs_fgE        EQU     01000000000110b     ; foreground == 14
@@cs_fgF        EQU     01000000000111b     ; foreground == 15
@@cs_card       EQU     00011111111000b     ; Card # mask (GRAM/GROM index #)
@@cs_gram       EQU     00100000000000b     ; Selects cards from GRAM if set
@@cs_advance    EQU     10000000000000b     ; Advances color stack.
@@cs_colsqr     EQU     01000000000000b     ; Selects 'colored square mode'

@@cs_pix0       EQU     00000000000111b     ; ColSqr Pixel 0 mask
@@cs_pix1       EQU     00000000111000b     ; ColSqr Pixel 1 mask
@@cs_pix2       EQU     00000111000000b     ; ColSqr Pixel 2 mask
@@cs_pix3       EQU     10011000000000b     ; ColSqr Pixel 3 mask

@@cs_pix0_0     EQU     00000000000000b     ; ColSqr Pixel 0, color == 0
@@cs_pix0_1     EQU     00000000000001b     ; ColSqr Pixel 0, color == 1
@@cs_pix0_2     EQU     00000000000010b     ; ColSqr Pixel 0, color == 2
@@cs_pix0_3     EQU     00000000000011b     ; ColSqr Pixel 0, color == 3
@@cs_pix0_4     EQU     00000000000100b     ; ColSqr Pixel 0, color == 4
@@cs_pix0_5     EQU     00000000000101b     ; ColSqr Pixel 0, color == 5
@@cs_pix0_6     EQU     00000000000110b     ; ColSqr Pixel 0, color == 6
@@cs_pix0_7     EQU     00000000000111b     ; ColSqr Pixel 0, color == 7

@@cs_pix1_0     EQU     00000000000000b     ; ColSqr Pixel 1, color == 0
@@cs_pix1_1     EQU     00000000001000b     ; ColSqr Pixel 1, color == 1
@@cs_pix1_2     EQU     00000000010000b     ; ColSqr Pixel 1, color == 2
@@cs_pix1_3     EQU     00000000011000b     ; ColSqr Pixel 1, color == 3
@@cs_pix1_4     EQU     00000000100000b     ; ColSqr Pixel 1, color == 4
@@cs_pix1_5     EQU     00000000101000b     ; ColSqr Pixel 1, color == 5
@@cs_pix1_6     EQU     00000000110000b     ; ColSqr Pixel 1, color == 6
@@cs_pix1_7     EQU     00000000111000b     ; ColSqr Pixel 1, color == 7

@@cs_pix2_0     EQU     00000000000000b     ; ColSqr Pixel 2, color == 0
@@cs_pix2_1     EQU     00000001000000b     ; ColSqr Pixel 2, color == 1
@@cs_pix2_2     EQU     00000010000000b     ; ColSqr Pixel 2, color == 2
@@cs_pix2_3     EQU     00000011000000b     ; ColSqr Pixel 2, color == 3
@@cs_pix2_4     EQU     00000100000000b     ; ColSqr Pixel 2, color == 4
@@cs_pix2_5     EQU     00000101000000b     ; ColSqr Pixel 2, color == 5
@@cs_pix2_6     EQU     00000110000000b     ; ColSqr Pixel 2, color == 6
@@cs_pix2_7     EQU     00000111000000b     ; ColSqr Pixel 2, color == 7

@@cs_pix3_0     EQU     00000000000000b     ; ColSqr Pixel 3, color == 0
@@cs_pix3_1     EQU     00001000000000b     ; ColSqr Pixel 3, color == 1
@@cs_pix3_2     EQU     00010000000000b     ; ColSqr Pixel 3, color == 2
@@cs_pix3_3     EQU     00011000000000b     ; ColSqr Pixel 3, color == 3
@@cs_pix3_4     EQU     10000000000000b     ; ColSqr Pixel 3, color == 4
@@cs_pix3_5     EQU     10001000000000b     ; ColSqr Pixel 3, color == 5
@@cs_pix3_6     EQU     10010000000000b     ; ColSqr Pixel 3, color == 6
@@cs_pix3_7     EQU     10011000000000b     ; ColSqr Pixel 3, color == 7

;;--------------------------------------------------------------------------;;
;; Useful bits for Foreground/Background Mode                               ;;
;;                                                                          ;;
;; Display format word layout in Color Stack Mode:                          ;;
;;   13   12   11   10    9    8    7    6    5    4    3    2    1    0    ;;
;; +----+----+----+----+----+----+----+----+----+----+----+----+----+----+  ;;
;; |BG  |BG  |GRAM|BG  |BG  |      GRAM/GROM Card #       |   FG Color   |  ;;
;; |Bit2|Bit3|GROM|Bit1|Bit0|          (0 - 63)           |   Bits 0-2   |  ;;
;; +----+----+----+----+----+----+----+----+----+----+----+----+----+----+  ;;
;;--------------------------------------------------------------------------;;

@@fb_fg0        EQU     00000000000000b     ; foreground ==  0
@@fb_fg1        EQU     00000000000001b     ; foreground ==  1
@@fb_fg2        EQU     00000000000010b     ; foreground ==  2
@@fb_fg3        EQU     00000000000011b     ; foreground ==  3
@@fb_fg4        EQU     00000000000100b     ; foreground ==  4
@@fb_fg5        EQU     00000000000101b     ; foreground ==  5
@@fb_fg6        EQU     00000000000110b     ; foreground ==  6
@@fb_fg7        EQU     00000000000111b     ; foreground ==  7

@@fb_bg0        EQU     00000000000000b     ; background ==  0
@@fb_bg1        EQU     00001000000000b     ; background ==  1
@@fb_bg2        EQU     00010000000000b     ; background ==  2
@@fb_bg3        EQU     00011000000000b     ; background ==  3
@@fb_bg4        EQU     10000000000000b     ; background ==  4
@@fb_bg5        EQU     10001000000000b     ; background ==  5
@@fb_bg6        EQU     10010000000000b     ; background ==  6
@@fb_bg7        EQU     10011000000000b     ; background ==  7
@@fb_bg8        EQU     01000000000000b     ; background ==  8
@@fb_bg9        EQU     01001000000000b     ; background ==  9
@@fb_bgA        EQU     01010000000000b     ; background == 10
@@fb_bgB        EQU     01011000000000b     ; background == 11
@@fb_bgC        EQU     11000000000000b     ; background == 12
@@fb_bgD        EQU     11001000000000b     ; background == 13
@@fb_bgE        EQU     11010000000000b     ; background == 14
@@fb_bgF        EQU     11011000000000b     ; background == 15

@@fb_card       EQU     00000111111000b     ; Card # mask (GRAM/GROM index #)
@@fb_gram       EQU     00100000000000b     ; Selects cards from GRAM if set

                ENDS

;;==========================================================================;;
;;  STIC COLOR-NAMES                                                        ;;
;;                                                                          ;;
;;  These are easier to remember short mnemonics for the STIC's colors.     ;;
;;  You can use the C_xxx colors for color-stack registers, border colors   ;;
;;  and so on where the color is stored in a contiguous field.  Use the     ;;
;;  X_xxx color names for the foreground color on display cards in color-   ;;
;;  stack mode, or for MOB attribute words.                                 ;;
;;                                                                          ;;
;;  Note that for the primary color set, C_xxx and X_xxx are identical.     ;;
;;==========================================================================;;

C_BLK   EQU     $0              ; Black
C_BLU   EQU     $1              ; Blue
C_RED   EQU     $2              ; Red
C_TAN   EQU     $3              ; Tan
C_DGR   EQU     $4              ; Dark Green
C_GRN   EQU     $5              ; Green
C_YEL   EQU     $6              ; Yellow
C_WHT   EQU     $7              ; White
C_GRY   EQU     $8              ; Grey
C_CYN   EQU     $9              ; Cyan
C_ORG   EQU     $A              ; Orange
C_BRN   EQU     $B              ; Brown
C_PNK   EQU     $C              ; Pink
C_LBL   EQU     $D              ; Light Blue
C_YGR   EQU     $E              ; Yellow-Green
C_PUR   EQU     $F              ; Purple

X_BLK   EQU     $0              ; Black
X_BLU   EQU     $1              ; Blue
X_RED   EQU     $2              ; Red
X_TAN   EQU     $3              ; Tan
X_DGR   EQU     $4              ; Dark Green
X_GRN   EQU     $5              ; Green
X_YEL   EQU     $6              ; Yellow
X_WHT   EQU     $7              ; White
X_GRY   EQU     $1000           ; Grey
X_CYN   EQU     $1001           ; Cyan
X_ORG   EQU     $1002           ; Orange
X_BRN   EQU     $1003           ; Brown
X_PNK   EQU     $1004           ; Pink
X_LBL   EQU     $1005           ; Light Blue
X_YGR   EQU     $1006           ; Yellow-Green
X_PUR   EQU     $1007           ; Purple


