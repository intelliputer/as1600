;; ======================================================================== ;;
;;  GMSLOADER -- This is where we start.                                    ;;
;; ======================================================================== ;;
                GMSLOADER   PROC

0x02B8, 0xC80A,         MVII    #GMSMAIN,   R0  ; \
0x0240, 0x0100,         MVO     R0,     $100    ;  |__ Initialize via an
0x0040,             SWAP    R0              ;  |   interrupt handler
0x0240, 0x0101,         MVO     R0,     $101    ; /
0x0002,             EIS                     ;
0x0017,             DECR    PC              ; Spin till intr happens.

                            ENDP

0x00af,         STUB        JR      R5

;; ======================================================================== ;;
;;  GMSMAIN   -- This is where the actual action begins.                    ;;
;; ======================================================================== ;;
                GMSMAIN     PROC

0x0003,                     DIS

0x02BE, 0x0600,             MVII    #STACK, R6      ; Put our stack in scratch memory.

0x02B8, 0xC809,             MVII    #STUB,  R0      ; \
0x0240, 0x0100,             MVO     R0,     $100    ;  |   Stub out interrupt handler for now.
0x0040,                     SWAP    R0              ;  |-- We need to allow interrupts to 
0x0240, 0x0101,             MVO     R0,     $101    ;  |   happen otherwise STIC gets unhappy.
0x0002,                     EIS                     ; /

                            ;; Copy over most of the important ranges:
0x0004, 0x01C8, 0x0085,     CALL    COPYRAM
0x0020, 0x0000,             DECLE   $0020,  $0000   ; $0000 - $001F
0x000B, 0x0028,             DECLE   $000B,  $0028   ; $0028 - $0032
0x0010, 0x00F0,             DECLE   $0010,  $00F0   ; $00F0 - $00FF
0x025E, 0x0102,             DECLE   $025E,  $0102   ; $0102 - $035F
0x0200, 0x3800,             DECLE   $0200,  $3800   ; $3800 - $39FF
0x0800, 0x4000,             DECLE   $0800,  $4000   ; $4000 - $47FF
0x0000,                     DECLE   0

                            ;; Copy back the 3 words that go at $7000:
0x02B9, 0x0008,             MVII    #WBANK, R1      ; \
0x02BA, 0x0010,             MVII    #BBANK, R2      ;  |-- Point window at save area
0x0004, 0x01C8, 0x00AF,     CALL    IC_SETBANK      ; /

0x0280, 0x0800,             MVI     WINDOW+0, R0    ; \
0x0283, 0x0801,             MVI     WINDOW+1, R3    ;  |-- Read three bytes in.
0x0284, 0x0802,             MVI     WINDOW+2, R4    ; / 

0x02B9, 0x0008,             MVII    #WBANK, R1      ; \
0x02BA, 0x0070,             MVII    #$70,   R2      ;  |-- Point window at $7000
0x0004, 0x01C8, 0x00AF,     CALL    IC_SETBANK      ; /

0x0240, 0x0800,             MVO     R0, WINDOW+0    ; \
0x0243, 0x0801,             MVO     R3, WINDOW+1    ;  |-- Write three bytes out.
0x0244, 0x0802,             MVO     R4, WINDOW+2    ; /

                            ;; Point the window at our register save area.
0x02B9, 0x0008,             MVII    #WBANK, R1      ; \
0x02BA, 0x0000,             MVII    #CBANK, R2      ;  |-- Point window at CPUREGS
0x0004, 0x01C8, 0x00AF,     CALL    IC_SETBANK      ; /

                            ;; Prepare for final launch.  We do that from an ISR.
0x02B9, 0xC84E,             MVII    #GMSLAUNCH, R1
0x0241, 0x0100,             MVO     R1,     $100
0x0041,                     SWAP    R1
0x0241, 0x0101,             MVO     R1,     $101
0x0017,                     DECR    PC              ; spin

                            ENDP

;; ======================================================================== ;;
;;  GMSLAUNCH -- Actually launch the game.                                  ;;
;; ======================================================================== ;;
                GMSLAUNCH   PROC
0x0003,                     DIS

                            ;; Is display visible?
0x0280, 0x084F,             MVI     REGS.visb,  R0
0x0080,                     TSTR    R0
0x0204, 0x0002,             BEQ     @@notvis
0x0240, 0x0020,             MVO     R0,     $20
                   @@notvis:
                               
                            ;; Color stack or FG/BG
0x0280, 0x0021,             MVI     $21,        R0  ; Assume color-stack
0x0280, 0x084E,             MVI     REGS.mode,  R0
0x0080,                     TSTR    R0
0x0204, 0x0002,             BEQ     @@colstk
0x0240, 0x0021,             MVO     R0,         $21 ; FG/BG
                   @@colstk:

                            ;; Restore ISR vector
0x0280, 0x084D,             MVI     REGS.isrv,  R0
0x0240, 0x0100,             MVO     R0,         $100
0x0040,                     SWAP    R0
0x0240, 0x0101,             MVO     R0,         $101

                            ;; Restore registers R1 - R6
0x02BD, 0x0841,             MVII    #REGS.1,    R5
0x02A9,                     MVI@    R5,         R1
0x02AA,                     MVI@    R5,         R2
0x02AB,                     MVI@    R5,         R3
0x02AC,                     MVI@    R5,         R4
0x000D,                     INCR    R5
0x02AE,                     MVI@    R5,         R6
0x0285, 0x0845,             MVI     REGS.5,     R5

                            ;; Separate return paths based on EIS or DIS:
0x0280, 0x0849,             MVI     REGS.intr,  R0
0x03B8, 0x0001,             ANDI    #1,         R0
0x0204, 0x0007,             BEQ     @@do_eis

                            ;; Launch w/out EIS.
0x0280, 0x0848,             MVI     REGS.stat,  R0
0x0038,                     RSWD    R0
0x0280, 0x0840,             MVI     REGS.0,     R0
0x0287, 0x0847,             MVI     REGS.7,     R7

                            ;; Launch with EIS.
0x0280, 0x0848,   @@do_eis: MVI     REGS.stat,  R0
0x0038,                     RSWD    R0
0x0280, 0x0840,             MVI     REGS.0,     R0
0x0002,                     EIS
0x0287, 0x0847,             MVI     REGS.7,     R7

                            ENDP


;;==========================================================================;;
;;  COPYRAM  -- Copies data from Intellicart space to CPU space.            ;;
;;                                                                          ;;
;;  This performs a copy from the Intellicart's private memory space to     ;;
;;  the Intellivision's memory space.  The copy occurs via the bankswitch   ;;
;;  window in chunks no larger than 2048 words.                             ;;
;;                                                                          ;;
;;  Inputs:                                                                 ;;
;;      R5 -- List of ranges to copy, stored as "length, addr" pairs.       ;;
;;            Length == 0 terminates.  Function will return immediately     ;;
;;            after the 0 word.                                             ;;
;;                                                                          ;;
;;  Outputs:                                                                ;;
;;      R0 -- Untouched.                                                    ;;
;;      R1, R2, R3, R4, R5 -- Trashed.                                      ;;
;;==========================================================================;;
                COPYRAM     PROC

0x0200, 0x0023,             B       @@first 

0x02AC,           @@o_loop: MVI@    R5,     R4      ; Get address.
0x0275,                     PSHR    R5              ; Save our table ptr.

                  @@m_loop:
0x02B9, 0x0008,             MVII    #WBANK, R1      ; \
0x00A2,                     MOVR    R4,     R2      ;  |   Point window to start of range
0x0042,                     SWAP    R2              ;  |-- (within 256 word granularity).
0x03BA, 0x00FF,             ANDI    #$FF,   R2      ;  |
0x0004, 0x01C8, 0x00AF,     CALL    IC_SETBANK      ; /

0x00A5,                     MOVR    R4,     R5      ; \
0x03BD, 0x00FF,             ANDI    #$FF,   R5      ;  |-- Point source pointer into window.
0x02FD, 0x0800,             ADDI    #WINDOW,R5      ; /

0x00AA,                     MOVR    R5,     R2          ; \
0x00DA,                     ADDR    R3,     R2          ;  |   Make sure copy loop stays 
0x037A, 0x1000,             CMPI    #WINDOW + 2048, R2  ;  |__ inside the window.  If 
0x0209, 0x0002,             BNC     @@cnt_ok            ;  |   "start+count > end_of_window",
0x02BA, 0x0FFF,             MVII    #WINDOW+2048-1, R2  ;  |   count = end_of_window - start.
0x012A,           @@cnt_ok: SUBR    R5,     R2          ; /

0x0113,                     SUBR    R2,     R3      ; Subtract this pass' count from total

0x02A9,           @@i_loop: MVI@    R5,     R1      ; \
0x0261,                     MVO@    R1,     R4      ;  |__ Copy as much data as we can.
0x0012,                     DECR    R2              ;  |
0x022C, 0x0004,             BNEQ    @@i_loop        ; /

0x009B,                     TSTR    R3              ;
0x022C, 0x001F,             BNEQ    @@m_loop        ; Keep going until it's all copied.
                            
0x02B5,                     PULR    R5              ; Restore table ptr.
0x02AB,         @@first:    MVI@    R5,     R3      ; Get length byte
0x009B,                     TSTR    R3              ; Is it 0?
0x022C, 0x0026,             BNEQ    @@o_loop        ; No:   Lets do this range.
0x00AF,                     JR      R5              ; Yes:  Return

                            ENDP


;;==========================================================================;;
;; IC_SETBANK    -- Sets a bank for a given 2K page of memory.              ;;
;;                                                                          ;;
;;  Inputs:                                                                 ;;
;;    R1 -- 5 MSBs of bank address in bits 7..3                             ;;
;;    R2 -- New bank address to point to in 8 LSBs.                         ;;
;;    R5 -- Return address                                                  ;;
;;                                                                          ;;
;;  Outputs:                                                                ;;
;;    R1 -- Address of Intellicart base control register.                   ;;
;;    R2 -- Trashed.                                                        ;;
;;    R5 -- Trashed.                                                        ;;
;;                                                                          ;;
;;  Example:                                                                ;;
;;    To remap 0x7800-0x7FFF in the Inty's address space to point to        ;;
;;    0x4000-0x47FF in the cart's address space, pass in the following      ;;
;;    parameters:  R1 = 0x0078, R2 = 0x0040.                                ;;
;;                                                                          ;;
;;==========================================================================;;
                IC_SETBANK  PROC
0x0275,                     PSHR    R5              ; Save return address.
0x008D,                     MOVR    R1,     R5      ; Get Hi/Lo 2K bit from address in R5
0x00ED,                     ADDR    R5,     R5
0x03BD, 0x0010,             ANDI    #$10,   R5

0x0065,                     SLR     R1,     2       ; Put 4 MSBs of page in 4 LSBs of R0
0x0065,                     SLR     R1,     2
0x03B9, 0x000F,             ANDI    #$0F,   R1      ; Keep only the four LSBs.
0x00E9,                     ADDR    R5,     R1      ; Merge bits: 76543210 -> xxxx37654
0x02F9, 0x0040,             ADDI    #$40,   R1      ; Point it at Intellicart ctrl regs.
0x024A,                     MVO@    R2,     R1      ; Write to control register.
0x02B7,                     PULR    PC              ; Return
                            ENDP

