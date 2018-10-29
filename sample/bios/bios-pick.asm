	.org	0x1000

	bios.keyclick 0
loop:
	bios.pick
	tsta
	beq	loop
	cmpa	0x1c
	bne	rel1
	ldaa	0x89
	bra	rel4
rel1:
	cmpa	0x1d
	bne	rel2
	ldaa	0x86
	bra	rel4
rel2:
	cmpa	0x1e
	bne	rel3
	ldaa	0x88
	bra	rel4
rel3:
	cmpa	0x1f
	bne	rel4
	ldaa	0x87
rel4:
	bios.putc
	bra	loop

	.include "bios.inc"
