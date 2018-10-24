	.org	0x1000
	ldx	0x1080

	xrndseed 1
loop:
	xrndn8	0x10
	staa	[x]
	inx
	cpx	0x1080+128
	bne	loop
	rts

	.include "xorshift.inc"
