	.org	0x1000

	bios.keyclick 0

	bios.color 7,0
	bios.cls
	
	bios.color 0,7
	ldaa	'A'
	bios.putc
	bios.color 1,0
	ldaa	'B'
	bios.putc
	bios.color 2,0
	ldaa	'C'
	bios.putc
	bios.color 3,0
	ldaa	'D'
	bios.putc
	bios.color 4,0
	ldaa	'E'
	bios.putc
	bios.color 5,0
	ldaa	'F'
	bios.putc
	bios.color 6,0
	ldaa	'G'
	bios.putc
	bios.color 7,0
	ldaa	'H'
	bios.putc

	bios.locate 10,2

	bios.color 0,7
	ldaa	'A'
	bios.putc
	bios.color 1,7
	ldaa	'B'
	bios.putc
	bios.color 2,7
	ldaa	'C'
	bios.putc
	bios.color 3,7
	ldaa	'D'
	bios.putc
	bios.color 4,7
	ldaa	'E'
	bios.putc
	bios.color 5,7
	ldaa	'F'
	bios.putc
	bios.color 6,7
	ldaa	'G'
	bios.putc
	bios.color 7,1
	ldaa	'H'
	bios.putc
	
	bios.locate 0,3
	bios.color 7,0
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
	
