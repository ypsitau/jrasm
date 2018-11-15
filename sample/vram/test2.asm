	.org	0x1000

	pcgpage.mainpage.store
	vram.clear
	ldmb	[0xca00],1

	vram.fromxy 0,0
	vram.puts "0123456789\0"

	vram.fromxy 10,10
	pcg.chkcircle2x2.put 0
	pcg.chkcircle2x2.putattr 1

	vram.refresh
loop:	bra	loop
	//vram.scrollleft 1,22
	vram.scrollright 1,22
	xrndn.mb vram.height
	staa	[posy]
	//vram.fromxy vram.width-1,[posy]
	vram.fromxy 0,[posy]
	ldaa	'>'
	staa	[x]
	ldaa	7
	staa	[x+1]
	vram.refresh
	jmp	loop

	.wseg
posx:	.ds	1
posy:	.ds	1

	.dseg
	.pcgpage mainpage,cram:0x80

	.pcg	chkcircle2x2, 2,2, 1,32*2, 2:0
	.db	b".....######....."
	.db	b"...##...#####..."
	.db	b"..#.....######.."
	.db	b".#......#######."
	.db	b".#......#######."
	.db	b"#.......########"
	.db	b"#.......########"
	.db	b"#.......########"
	.db	b"########.......#"
	.db	b"########.......#"
	.db	b"########.......#"
	.db	b".#######......#."
	.db	b".#######......#."
	.db	b"..######.....#.."
	.db	b"...#####...##..."
	.db	b".....######....."
	.end

	.end

	.include "bios.inc"
	.include "oputil.inc"
	.include "xrnd.inc"
	.include "vram32x24x3.inc"
