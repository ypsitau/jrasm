;;;=============================================================================
;;; oputil-test.inc
;;;=============================================================================
	.cseg
	.org	0x1000
	;;-----------------------
	;; addmb
	;;-----------------------
	jsr	init
	bios.puthex.mb [byte1]
	bios.putc '+'
	bios.puthex.mb [byte2]
	addmb.mb [byte1],[byte2]
	bios.putc '='
	bios.puthex.mb [byte1]
	bios.putc '\r'
	
	;;-----------------------
	;; addmw
	;;-----------------------
	jsr	init
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mb [byte2]
	addmw.mb [word1],[byte2]
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	jsr	init
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mw [word2]
	addmw.mw [word1],[word2]
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	jsr	init
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mb 0x34
	addmw.mb [word1],0x34
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	jsr	init
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mw 0x12f4
	addmw.mw [word1],0x12f4
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	;;-----------------------
	;; submb
	;;-----------------------
	jsr	init
	bios.puthex.mb [byte1]
	bios.putc '-'
	bios.puthex.mb [byte2]
	submb.mb [byte1],[byte2]
	bios.putc '='
	bios.puthex.mb [byte1]
	bios.putc '\r'
	
	;;-----------------------
	;; submw
	;;-----------------------
	jsr	init
	bios.puthex.mw [word1]
	bios.putc '-'
	bios.puthex.mb [byte2]
	submw.mb [word1],[byte2]
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	jsr	init
	bios.puthex.mw [word1]
	bios.putc '-'
	bios.puthex.mw [word2]
	submw.mw [word1],[word2]
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	jsr	init
	bios.puthex.mw [word1]
	bios.putc '-'
	bios.puthex.mb 0x34
	submw.mb [word1],0x34
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	jsr	init
	bios.puthex.mw [word1]
	bios.putc '-'
	bios.puthex.mw 0x12f4
	submw.mw [word1],0x12f4
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	rts

	addmb	[byte1],8
	addmb	[byte1],[byte2]
	
	addmw.a	[word1]
	addmw.mb [word1],0x12
	addmw.mb [word1],[byte2]
	addmw.mw [word1],0x1234
	addmw.mw [word1],[word2]
	
	addx.a
	addx.mb	0x12
	addx.mb	[byte1]
	addx.mw	0x1234
	addx.mw	[word1]
	rts

init:	
	ldmb	[byte1],0xc3
	ldmb	[byte2],0x34
	ldmw	[word1],0x5678
	ldmw	[word2],0x12f4
	rts

	.wseg
byte1:	.ds	@byte
byte2:	.ds	@byte
word1:	.ds	@word
word2:	.ds	@word

	.include "bios.inc"
	.include "oputil.inc"
