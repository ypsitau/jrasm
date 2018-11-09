	.org	0x1000

;;;-----------------------------------------------------------------------------
;;; Structure
;;;-----------------------------------------------------------------------------
ball:	.struct
posx:	.ds	@byte
posy:	.ds	@byte
dirx:	.ds	@byte
diry:	.ds	@byte
	.end

;;;-----------------------------------------------------------------------------
;;; Main routine
;;;-----------------------------------------------------------------------------
	ldx	balls
	ldmb	[x+ball.posx],10
	ldmb	[x+ball.posy],10
	ldmb	[x+ball.dirx],1
	ldmb	[x+ball.diry],1
	pcgpage.mainpage.store
	vram.clear
loop:
	ldx	balls
	vram.fromxy [x+ball.posx],[x+ball.posy]
	pcg.chkcircle2x2.put 0
	pcg.chkcircle2x2.putattr 1
	.save	x
	vram.refresh
	pcg.chkcircle2x2.erase 0
	pcg.chkcircle2x2.eraseattr ,,1
	.end
	ldx	balls
	movebound [x+ball.posx],[x+ball.dirx],0,30
	movebound [x+ball.posy],[x+ball.diry],0,22
	jmp	loop

balls:
	.ds	@ball * 1
ballsEnd:

	.pcgpage mainpage,cram:0x80

	.pcg	chkcircle2x2, 2,2, 2,64, 2:0
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

;;;-----------------------------------------------------------------------------
;;; movebound
;;;  Format: movebound pos,dir,min,max
;;;  Param: posx [IMM, DIR, IDX, EXT] .. Position
;;;         dir [IMM, DIR, IDX, EXT] .. Direction
;;;         min [IMM, DIR, IDX, EXT] .. Minimum value
;;;         max [IMM, DIR, IDX, EXT] .. Maximum value
;;;-----------------------------------------------------------------------------
movebound:
	.macro	pos,dir,min,max
	ldaa	pos
	cmpa	max
	bne	rel1
	ldaa	0xff
	staa	dir
	bra	rel2
rel1:
	cmpa	min
	bne	rel2
	ldaa	1
	staa	dir
rel2:
	addmb	pos,dir
	.end

	.include "bios.inc"
	.include "oputil.inc"
	.include "vram32x24x2.inc"
