	.org	0x1000

	bios.puts "bgt:\0"
	bios.puts "(4,3)\0"
	ldaa	4
	cmpa	3
	bgt	rel1
	ldaa	'f'
	bios.putc
	bra	rel2
rel1:
	ldaa	't'
	bios.putc
rel2:	
	bios.puts ";(4,4)\0"
	ldaa	4
	cmpa	4
	bgt	rel3
	ldaa	'f'
	bios.putc
	bra	rel4
rel3:	
	ldaa	't'
	bios.putc
rel4:	
	bios.puts ";(4,5)\0"
	ldaa	4
	cmpa	5
	bgt	rel5
	ldaa	'f'
	bios.putc
	bra	rel6
rel5:	
	ldaa	't'
	bios.putc
rel6:	
	ldaa	'\r'
	bios.putc
	
	rts
	
	.include "jump.inc"
	.include "bios.inc"
