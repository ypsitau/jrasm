;;;=============================================================================
;;; bios-puthex.asm
;;;=============================================================================
	.org	0x1000

	bios.cls

	bios.color 0,7
	bios.locate 0,0
	bios.puts "bios.puthex/putdec\0"
	bios.color 7,0

	.scope
posx:	.equ	0
posy:	.equ	2
	bios.locate posx,posy
	bios.puthex.mb	0x00
	bios.locate posx,posy+1
	bios.puthex.mb	0x01
	bios.locate posx,posy+2
	bios.puthex.mb	0xab
	bios.locate posx,posy+3
	bios.puthex.mb	0xff
	.end

	.scope
posx:	.equ	4
posy:	.equ	2
	bios.locate posx,posy
	bios.puthex.mw	0x0000
	bios.locate posx,posy+1
	bios.puthex.mw	0x0123
	bios.locate posx,posy+2
	bios.puthex.mw	0xabcd
	bios.locate posx,posy+3
	bios.puthex.mw	0xffff
	.end

	.scope
posx:	.equ	10
posy:	.equ	2
	bios.locate posx,posy
	bios.putdec.mb	0x00
	bios.locate posx,posy+1
	bios.putdec.mb	0x01
	bios.locate posx,posy+2
	bios.putdec.mb	0xab
	bios.locate posx,posy+3
	bios.putdec.mb	0xff
	.end

	.scope
posx:	.equ	14
posy:	.equ	2
	bios.locate posx,posy
	bios.putdec.mw	0x0000
	bios.locate posx,posy+1
	bios.putdec.mw	0x0123
	bios.locate posx,posy+2
	bios.putdec.mw	0xabcd
	bios.locate posx,posy+3
	bios.putdec.mw	0xffff
	.end

	bios.color 0,7
	bios.locate 0,7
	bios.puts "bios.tohex/todec\0"
	bios.color 7,0

	.scope
posx:	.equ	0
posy:	.equ	9
	bios.locate posx,posy
	bios.tohex.mb	0x00
	bios.putn.x_b
	bios.locate posx,posy+1
	bios.tohex.mb	0x01
	bios.putn.x_b
	bios.locate posx,posy+2
	bios.tohex.mb	0xab
	bios.putn.x_b
	bios.locate posx,posy+3
	bios.tohex.mb	0xff
	bios.putn.x_b
	.end

	.scope
posx:	.equ	4
posy:	.equ	9
	bios.locate posx,posy
	bios.tohex.mw	0x0000
	bios.putn.x_b
	bios.locate posx,posy+1
	bios.tohex.mw	0x0123
	bios.putn.x_b
	bios.locate posx,posy+2
	bios.tohex.mw	0xabcd
	bios.putn.x_b
	bios.locate posx,posy+3
	bios.tohex.mw	0xffff
	bios.putn.x_b
	.end

	.scope
posx:	.equ	10
posy:	.equ	9
	bios.locate posx,posy
	bios.todec.mb	0x00
	bios.putn.x_b
	bios.locate posx,posy+1
	bios.todec.mb	0x01
	bios.putn.x_b
	bios.locate posx,posy+2
	bios.todec.mb	0xab
	bios.putn.x_b
	bios.locate posx,posy+3
	bios.todec.mb	0xff
	bios.putn.x_b
	.end

	.scope
posx:	.equ	14
posy:	.equ	9
	bios.locate posx,posy
	bios.todec.mw	0x0000
	bios.putn.x_b
	bios.locate posx,posy+1
	bios.todec.mw	0x0123
	bios.putn.x_b
	bios.locate posx,posy+2
	bios.todec.mw	0xabcd
	bios.putn.x_b
	bios.locate posx,posy+3
	bios.todec.mw	0xffff
	bios.putn.x_b
	.end

infinite:
	bra	infinite

	.include "bios.inc"
