	.org	0x1000
	ldx	0x1080

loop:
	xrndn	0x10
	staa	[x]
	inx
	cpx	0x1080+128
	bne	loop
	rts

	.include "xorshift.inc"
