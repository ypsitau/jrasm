	.org	0x1000
	ldx	0x0000
	stx	[num]
loop:
	bios.puthex.w [num]
	bios.putc ' '
	bios.todec.w [num]
	.save	x,b
	negb
	addb	6
	ldaa	'*'
	jsr	bios._filln
	.end
	jsr	bios._putn
	bios.putc ' '
	bios.putdec.w [num]
	bios.putc '\r'
	ldx	[num]
	inx
	stx	[num]
	bra	loop
	rts
num:	.dw	0
	.include "bios.inc"
