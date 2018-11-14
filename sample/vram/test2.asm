	.org	0x1000

	vram.clear
	ldmb	[0xca00],1
	vram.color 7,0
	vram.fromxy 0,0
	vram.fill '*',vram.width
	vram.color 7,0
	vram.fromxy 0,23
	vram.fill '*',vram.width
	//vram.refresh
loop:	//bra	loop
	vram.scrolldown 1,22
	xrndn.mb vram.width
	staa	[posx]
	vram.fromxy [posx],1
	ldaa	'A'
	staa	[x]
	ldaa	7
	staa	[x+1]
	vram.refresh
	jmp	loop

	.wseg
posx:	.ds	1

	.include "oputil.inc"
	.include "xrnd.inc"
	.include "vram40x24x3.inc"
