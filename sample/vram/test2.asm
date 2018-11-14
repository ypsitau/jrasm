	.org	0x1000

	vram.clear
	ldmb	[0xca00],1
	vram.color 7,0

/*
	vram.fromxy 0,0
	vram.fill '*',32
	vram.color 7,0
	vram.fromxy 2,23
	vram.fill '*',32
	vram.fromxy 2,0
	vram.puts "Hello\0"
	vram.fromxy 2,2
	vram.puts "Hello\0"

	vram.fromxy 0,10
	vram.puts "ABCDEFG\0"

	vram.fromxy 0,22
	vram.puts "ABCDEFG\0"
	//vram.fill '+',20
*/

	vram.fromxy 0,1
	vram.puts "ABCD\0"

	vram.fromxy 32,1
	vram.puts "ABCD\0"

	vram.fromxy 0,2
	vram.puts "ABCD\0"

	vram.fromxy 32,2
	vram.puts "ABCD\0"

	vram.fromxy 0,3
	vram.puts "ABCD\0"

	vram.fromxy 32,3
	vram.puts "ABCD\0"

	vram.refresh
loop:	bra	loop

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
	.include "vram40x24x2.inc"
