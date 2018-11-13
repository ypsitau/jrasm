	.org	0x1000

	vram.clear
	ldmb	[0xca00],1
	vram.color 7,0
	vram.fromxy 0,0
	vram.fill '*',32
	vram.color 7,0
	vram.fromxy 0,23
	vram.fill '*',32
	vram.fromxy 0,0
	vram.puts "Hello\0"
loop:
	vram.scrolldown 1,22
	xrndn.mb 32
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
	.include "vram32x24x3.inc"
