		.FILENAMEJR "LINE"
; ************************************
; * LINE命令
; *
; * 機能：指定された2点間に直線を引く
; *
; * 引数：開始座標(X) - IXH(IXの上位バイト)
; *       開始座標(Y) - IXL(IXの下位バイト)
; *       終了座標(X) - B
; *       終了座標(Y) - A
; ************************************
		.ORG	0x7EC0
; 初期処理
		STX		[DATA01]	; 開始座標(X,Y)
		STX		[DATA10]	; 開始座標(X,Y)
		STAB	[DATA11]	; 終了座標(X)
		STAA	[DATA12]	; 終了座標(Y)
		LDX		DATA01
		CLR		[X+0x06]
		CLR		[X+0x07]
		CLR		[X+0x08]
		CLR		[X+0x09]
; X座標の差を求める
		SUBB	[X+0x0A]	; 終了座標(X) - 開始座標(X)
		BEQ		L3			; Z = 0   （終了＝ 開始）の場合、L3へ
		BCC		L1			; C = 0   （終了＞ 開始）の場合、L1へ
							; 上記以外（終了＜ 開始）の場合
		DEC		[X+0x08]
		EORB	0xFF
		INCB
		BRA		L2
L1:		INC		[X+0x08]
L2:		STAB	[X+0x06]
		EORB	0xFF
		INCB
L3:		STAB	[X+0x04]
; Y座標の差を求める
		SUBA	[X+0x0B]	; 終了座標(Y) - 開始座標(Y)
		BEQ		L6			; Z = 0   （終了＝ 開始）の場合、L6へ
		BCC		L4			; C = 0   （終了＞ 開始）の場合、L4へ
							; 上記以外（終了＜ 開始）の場合
		DEC		[X+0x09]
		EORA	0xFF
		INCA
		BRA		L5
L4:		INC		[X+0x09]
L5:		STAA	[X+0x07]
		EORA	0xFF
		INCA
L6:		STAA	[X+0x05]
; * 
		LDAA	[X+0x07]
		CMPA	[X+0x06]
		BCC		L8
		LDAA	[X+0x04]
		STAA	[X+0x03]
		CLR		[X+0x02]
LOOP1:	LDAA	[X]
		LDAB	[X+0x01]
		JSR		[SUB1]
		LDX		DATA01
		LDAA	[X]
		ADDA	[X+0x08]
		STAA	[X]
		LDAA	[X+0x03]
		LDAB	[X+0x07]
		ASLB
		ABA
		STAA	[X+0x03]
		BITA	0x80
		BNE		L7
		LDAB	[X+0x06]
		ASLB
		SBA
		STAA	[X+0x03]
		LDAA	[X+0x01]
		ADDA	[X+0x09]
		STAA	[X+0x01]
L7:		LDAA	[X+0x02]
		INCA
		STAA	[X+0x02]
		CMPA	[X+0x06]
		BNE		LOOP1
		RTS
; * 
L8:		LDAA	[X+0x05]
		STAA	[X+0x03]
		CLR		[X+0x02]
LOOP2:	LDAA	[X]
		LDAB	[X+0x01]
		JSR		[SUB1]
		LDX		DATA01
		LDAA	[X+0x01]
		ADDA	[X+0x09]
		STAA	[X+0x01]
		LDAA	[X+0x03]
		LDAB	[X+0x06]
		ASLB
		ABA
		STAA	[X+0x03]
		BITA	0x80
		BNE		L9
		LDAB	[X+0x07]
		ASLB
		SBA
		STAA	[X+0x03]
		LDAA	[X]
		ADDA	[X+0x08]
		STAA	[X]
L9:		LDAA	[X+0x02]
		INCA
		STAA	[X+0x02]
		CMPA	[X+0x07]
		BNE		LOOP2
		RTS
; ワーク領域
DATA01:	.DW		0x0000
DATA02:	.DB		0x00
DATA03:	.DB		0x00
DATA04:	.DB		0x00
DATA05:	.DB		0x00
DATA06:	.DB		0x00
DATA07:	.DB		0x00
DATA08:	.DB		0x00
DATA09:	.DB		0x00
DATA10:	.DW		0x0000
DATA11:	.DB		0x00
DATA12:	.DB		0x00
; * サブルーチン
SUB1:	CMPA	0x3F
		BHI		L15
		CMPB	0x2F
		BHI		L15
		PSHA
		PSHB
		LSRA
		PSHA
		LSRB
		CLRA
		ASLB
		ADCA	0x00
		ASLB
		ADCA	0x00
		ASLB
		ADCA	0x00
		ASLB
		ADCA	0x00
		ASLA
		ASLB
		ADCA	0xC1
		STAA	[SDATA3]
		ADDA	0x04
		STAA	[SDATA1]
		PULA
		ABA
		STAA	[SDATA4]
		STAA	[SDATA2]
		LDX		[SDATA1]
		LDAA	[X]
		BITA	0x80
		BNE		L10
		TAB
		LSRA
		LSRA
		LSRA
		ORAA	0x80
		ANDB	0xB8
		ABA
		STAA	[X]
		LDX		[SDATA3]
		STAA	[X]
L10:	LDX		[SDATA3]
		PULB
		PULA
		LSRA
		BCS		L12
		LSRB
		BCC		L11
		LDX		[SDATA1]
L11:	LDAA	[X]
		ANDA	0xB8
		LDAB	{0x0E}
		ANDB	0x07
		ABA
		BRA		L14
L12:	LSRB
		BCC		L13
		LDX		[SDATA1]
L13:	LDAA	[X]
		ANDA	0x87
		LDAB	{0x0E}
		ANDB	0x07
		ASLB
		ASLB
		ASLB
		ABA
L14:	STAA	[X]
L15:	RTS
;サブルーチン ワーク領域
SDATA1:	.DB		0x00
SDATA2:	.DB		0x00
SDATA3:	.DB		0x00
SDATA4:	.DB		0x00
