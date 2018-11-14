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
	vram.scrollleft 1,22
	xrndn.mb vram.height
	staa	[posy]
	vram.fromxy vram.width-1,[posy]
	ldaa	'<'
	staa	[x]
	ldaa	7
	staa	[x+1]
	vram.refresh
	jmp	loop

	.wseg
posx:	.ds	1
posy:	.ds	1

	.include "oputil.inc"
	.include "xrnd.inc"
	.include "vram32x24x3.inc"
