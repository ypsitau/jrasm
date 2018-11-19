;;;=============================================================================
;;; test2.asm
;;;=============================================================================
	.org	0x1000

STAT_neutral:	.equ	0
STAT_jumpup:	.equ	1
STAT_jumpdown:	.equ	2
JumpHeight:	.equ	6

player_t: .struct
posx:	.ds	1
posy:	.ds	1
stat:	.ds	1
count:	.ds	1
	.end

	pcgpage.mainpage.store
	vram.clear
	ldmb	[0xca00],1

	vram.fromxy 0,0
	vram.puts "0123456789\0"

	ldx	player
	ldmb	[x+player_t.posx],vram.width/2
	ldmb	[x+player_t.posy],vram.height-2
	ldmb	[x+player_t.stat],STAT_neutral
	ldmb	[x+player_t.count],0

loop:
	ldx	player
	vram.fromxy [x+player_t.posx],[x+player_t.posy]
	pcg.chkcircle2x2.erase 0
	pcg.chkcircle2x2.eraseattr 7,0,1
	vram.scrollleft

	ldx	player
	ldaa	[x+player_t.stat]
	cmpa	STAT_neutral
	bne	stat_not_neutral
	bios.pick
	ldx	player
	cmpa	32
	bne	stat_done
	ldmb	[x+player_t.stat],STAT_jumpup
	ldmb	[x+player_t.count],JumpHeight
	bra	stat_done
stat_not_neutral:
	cmpa	STAT_jumpup
	bne	stat_not_jumpup
	ldx	player
	submb	[x+player_t.posy],1
	dec	[x+player_t.count]
	bne	rel1
	ldmb	[x+player_t.stat],STAT_jumpdown
	ldmb	[x+player_t.count],JumpHeight
rel1:
	bra	stat_done
stat_not_jumpup:
	cmpa	STAT_jumpdown
	bne	stat_done
	ldx	player
	addmb	[x+player_t.posy],1
	dec	[x+player_t.count]
	bne	rel2
	ldmb	[x+player_t.stat],STAT_neutral
	bra	stat_done
rel2:
stat_done:

/*
	bios.pick
	ldx	player
	cmpa	28
	bne	rel1
	ldaa	[x+player_t.posx]
	cmpa	vram.width-2
	beq	rel4
	inca
	staa	[x+player_t.posx]
	bra	rel4
rel1:
	cmpa	29
	bne	rel2
	ldaa	[x+player_t.posx]
	cmpa	0
	beq	rel4
	deca
	staa	[x+player_t.posx]
	bra	rel4
rel2:
	cmpa	30
	bne	rel3
	ldaa	[x+player_t.posy]
	cmpa	0
	beq	rel4
	deca
	staa	[x+player_t.posy]
	bra	rel4
rel3:
	cmpa	31
	bne	rel4
	ldaa	[x+player_t.posy]
	cmpa	vram.height-2
	beq	rel4
	inca
	staa	[x+player_t.posy]
rel4:
*/

	xrndn.mb vram.height
	staa	[work.byte]
	vram.fromxy vram.width-1,[work.byte]
	ldaa	'*'
	staa	[x]
	ldaa	7
	staa	[x+1]

	ldx	player
	vram.fromxy [x+player_t.posx],[x+player_t.posy]
	pcg.chkcircle2x2.put 0
	pcg.chkcircle2x2.putattr 1

	vram.refresh
	jmp	loop

player:	.ds	@player_t

work.byte:	.ds	1

	.pcgpage mainpage,cram:0x80

	.pcgstride 3,108

	.pcg	chkcircle2x2, 2,2, 2:0
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
	.include "vram36x24x3.inc"
