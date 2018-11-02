	.org	0x1000

	.scope
posx:	.equ	0
posy:	.equ	0
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
posy:	.equ	0
	bios.locate posx,posy
	bios.puthex.mw	0x0000
	bios.locate posx,posy+1
	bios.puthex.mw	0x0101
	bios.locate posx,posy+2
	bios.puthex.mw	0xabab
	bios.locate posx,posy+3
	bios.puthex.mw	0xffff
	.end
	
	.scope
posx:	.equ	8
posy:	.equ	0
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
posx:	.equ	12
posy:	.equ	0
	bios.locate posx,posy
	bios.putdec.mw	0x0000
	bios.locate posx,posy+1
	bios.putdec.mw	0x0101
	bios.locate posx,posy+2
	bios.putdec.mw	0xabab
	bios.locate posx,posy+3
	bios.putdec.mw	0xffff
	.end
	
infinite:
	bra	infinite
	
	.include "bios.inc"
