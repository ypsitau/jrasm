		;; http://yorumomiji.sakura.ne.jp/contents/documents/6802-assembler-users-guide
VIA:    .EQU    0xc800
T1CL:   .EQU    0x04
T1CH:   .EQU    0x05
T1LL:   .EQU    0x06
T1LH:   .EQU    0x07
ACR:    .EQU    0x0b

        .ORG    0x3000
START:
        LDX     VIA
        LDAA    [X+ACR]
        ORAA    0xd0        ; T1 free-run mode
        STAA    [X+ACR]
        LDX     SCORE
        STX     [CUR]
LOOP:   LDAA    [X]         ; A: 音程
        CMPA    0xFF
        BEQ     END
        LDX     [TEMP]
        ASLA
        ADDA    [TEMP+1]
        ANDA    0x00        ; Aにゼロをセット(Cフラグ変化無し)
        ADCB    [TEMP]

        LDX     VIA
        STAA    [X+T1CL]
        STAB    [X+T1CH]    ; この時点から新しい音程の音を発生する。
        
        BSR     WAIT
        BSR     WAIT
        LDX     CUR
        INX
        STX     [CUR]
        BRA     LOOP

END:    LDX     VIA
        LDAA    [X+ACR]
        ANDA    0x3F
        STAA    [X+ACR]
        RTS

; 約0.4秒待つ
WAIT:   PSHA
        CLRA
        CLRB
LOOP2:  DECB
        BNE LOOP2
        DECA
        BNE LOOP2
        PULA
        RTS
        
        .ORG    0x3080  ; 
TABLE:  .DW     0x0473, 0x0433, 0x03F6, 0x03BD, 0x0387, 0x0355, 0x0325, 0x02F7
        .DW     0x02CD, 0x02A4, 0x027E, 0x025A, 0x0238, 0x0218, 0x01FA, 0x01DD
        .DW     0x01C2, 0x01A9, 0x0191, 0x017A, 0x0165, 0x0151, 0x013E, 0x012D
        .DW     0x011B, 0x010B, 0x00FC
        
        .ORG    0x3100
SCORE:  .DB     0, 8, 10, 12, 10, 8, 7, 8, 10, 12, 10, 0, 99

        .ORG    0x3180
TEMP:   .DW     0x3080      ; 音程データのアドレス計算用
CUR:    .DW     0x0000      ; 現在の音符位置退避用
