	.org	0x1000
	
loop:
	bios.pick
	cmpa	0x1c
	bne	rel1
	bios.locate [posx],0
	bios.putc '\0'
	inc	[posx]
	bra	rel2
rel1:
	cmpa	0x1d
	bne	rel2
	bios.locate [posx],0
	bios.putc '\0'
	dec	[posx]
rel2:
	bios.locate [posx],0
	bios.putc 'X'
	bios.scrolldown 0xc100
	bra	loop

posx:	.db	0
	.include "bios.inc"
