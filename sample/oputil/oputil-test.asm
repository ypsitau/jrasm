;;;=============================================================================
;;; oputil-test.inc
;;;=============================================================================
	.cseg
	.org	0x1000
	;;-----------------------
	;; addmb
	;;-----------------------
	jsr	init
	bios.puts "addmb.a       \0"
	bios.puthex.mb [byte1]
	bios.putc '+'
	bios.puthex.mb [byte2]
	ldaa	[byte2]
	addmb.a [byte1]
	bios.putc '='
	bios.puthex.mb [byte1]
	bios.putc '\r'

	jsr	init
	bios.puts "addmb.mb(EXT) \0"
	bios.puthex.mb [byte1]
	bios.putc '+'
	bios.puthex.mb [byte2]
	addmb.mb [byte1],[byte2]
	bios.putc '='
	bios.puthex.mb [byte1]
	bios.putc '\r'
	
	jsr	init
	bios.puts "addmb.mb(IMM) \0"
	bios.puthex.mb [byte1]
	bios.putc '+'
	bios.puthex.mb 0x34
	addmb.mb [byte1],0x34
	bios.putc '='
	bios.puthex.mb [byte1]
	bios.putc '\r'
	
	;;-----------------------
	;; addmw
	;;-----------------------
	jsr	init
	bios.puts "addmw.mb(EXT) \0"
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mb [byte2]
	addmw.mb [word1],[byte2]
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	jsr	init
	bios.puts "addmw.mb(EXT) \0"
	bios.puthex.mw [word2]
	bios.putc '+'
	bios.puthex.mb [byte2]
	addmw.mb [word2],[byte2]
	bios.putc '='
	bios.puthex.mw [word2]
	bios.putc '\r'

	jsr	init
	bios.puts "addmw.mw(EXT) \0"
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mw [word2]
	addmw.mw [word1],[word2]
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	jsr	init
	bios.puts "addmw.mb(IMM) \0"
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mb 0x34
	addmw.mb [word1],0x34
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	jsr	init
	bios.puts "addmw.mb(IMM) \0"
	bios.puthex.mw [word2]
	bios.putc '+'
	bios.puthex.mb 0x34
	addmw.mb [word2],0x34
	bios.putc '='
	bios.puthex.mw [word2]
	bios.putc '\r'

	jsr	init
	bios.puts "addmw.mw(EXT) \0"
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mw 0x12f4
	addmw.mw [word1],0x12f4
	bios.putc '='
	bios.puthex.mw [word1]
	bios.putc '\r'

	;;-----------------------
	;; addx
	;;-----------------------
	jsr	init
	bios.puts "addx.mb(EXT)  \0"
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mb [byte2]
	ldx	[word1]
	addx.mb [byte2]
	bios.putc '='
	bios.puthex.x
	bios.putc '\r'

	jsr	init
	bios.puts "addx.mb(EXT)  \0"
	bios.puthex.mw [word2]
	bios.putc '+'
	bios.puthex.mb [byte2]
	ldx	[word2]
	addx.mb [byte2]
	bios.putc '='
	bios.puthex.x
	bios.putc '\r'

	jsr	init
	bios.puts "addx.mw(EXT)  \0"
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mw [word2]
	ldx	[word1]
	addx.mw [word2]
	bios.putc '='
	bios.puthex.x
	bios.putc '\r'

	jsr	init
	bios.puts "addx.mb(IMM)  \0"
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mb 0x34
	ldx	[word1]
	addx.mb 0x34
	bios.putc '='
	bios.puthex.x
	bios.putc '\r'

	jsr	init
	bios.puts "addx.mb(IMM)  \0"
	bios.puthex.mw [word2]
	bios.putc '+'
	bios.puthex.mb 0x34
	ldx	[word2]
	addx.mb 0x34
	bios.putc '='
	bios.puthex.x
	bios.putc '\r'

	jsr	init
	bios.puts "addx.mw(IMM)  \0"
	bios.puthex.mw [word1]
	bios.putc '+'
	bios.puthex.mw 0x12f4
	ldx	[word1]
	addx.mw 0x12f4
	bios.putc '='
	bios.puthex.x
	bios.putc '\r'


	rts

	
	addx.a
	addx.mb	0x12
	addx.mb	[byte1]
	addx.mw	0x1234
	addx.mw	[word1]
	rts

init:	
	ldmb	[byte1],0xc3
	ldmb	[byte2],0x34
	ldmw	[word1],0x5612
	ldmw	[word2],0x12f4
	rts

	.wseg
byte1:	.ds	@byte
byte2:	.ds	@byte
word1:	.ds	@word
word2:	.ds	@word

	.include "bios.inc"
	.include "oputil.inc"
