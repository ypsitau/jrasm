	.org	0x1000
	bios.cls
	
	bios.color 0,7
	ldaa	'A'
	bios.putc
	bios.color 1,0
	ldaa	'A'
	bios.putc
	bios.color 2,0
	ldaa	'B'
	bios.putc
	bios.color 3,0
	ldaa	'C'
	bios.putc
	bios.color 4,0
	ldaa	'D'
	bios.putc
	bios.color 5,0
	ldaa	'E'
	bios.putc
	bios.color 6,0
	ldaa	'F'
	bios.putc
	bios.color 7,0
	ldaa	'F'
	bios.putc

	bios.color 0,7
	ldaa	'A'
	bios.putc
	bios.color 1,7
	ldaa	'A'
	bios.putc
	bios.color 2,7
	ldaa	'B'
	bios.putc
	bios.color 3,7
	ldaa	'C'
	bios.putc
	bios.color 4,7
	ldaa	'D'
	bios.putc
	bios.color 5,7
	ldaa	'E'
	bios.putc
	bios.color 6,7
	ldaa	'F'
	bios.putc
	bios.color 7,1
	ldaa	'F'
	bios.putc

	rts
	
	.include "bios.inc"
	