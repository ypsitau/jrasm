	.org	0x1000
	ldx	0x0000
	stx	[num]
loop:
	bios.puthex.mw [num]
	bios.putc.mb ' '
	bios.todec.mw [num]
	.save	x,b
	negb
	addb	6
	ldaa	'*'
	jsr	bios.filln.a_b
	.end
	bios.putn.x_b
	bios.putc ' '
	bios.putdec.mw [num]
	bios.putc.mb '\r'
	ldx	[num]
	inx
	stx	[num]
	bra	loop
	rts
num:	.dw	0
	.include "bios.inc"
