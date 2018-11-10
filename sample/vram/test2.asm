	.org	0x1000

	vram.clear
loop:
	vram.scrollup
	xrndn.mb 31
	staa	[posx]
	vram.fromxy [posx],22
	ldaa	'A'
	staa	[x]
	ldaa	7
	staa	[x+1]
	vram.refresh
	jmp	loop

	.wseg
posx:	.ds	1

	.include "xrnd.inc"
	.include "vram32x24x2.inc"
