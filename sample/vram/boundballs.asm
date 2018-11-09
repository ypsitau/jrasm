;;;=============================================================================
;;; boundballs.asm
;;;=============================================================================
	.org	0x1000

;;;-----------------------------------------------------------------------------
;;; Structure
;;;-----------------------------------------------------------------------------
ball:	.struct
id:	.ds	@byte
posx:	.ds	@byte
posy:	.ds	@byte
dirx:	.ds	@byte
diry:	.ds	@byte
	.end

;;;-----------------------------------------------------------------------------
;;; Main routine
;;;-----------------------------------------------------------------------------
	ldmb	{0x00},0	; disable click sound
	bios.cls

	pcgpage.mainpage.store

	;; Initialize parameter table
	.scope
	clra
	ldx	balls
each_ball:
	.save	a
	staa	[x+ball.id]
	; ball.posx = xrndn.mb(30) + 1
	xrndn.mb	30
	inca
	staa	[x+ball.posx]
	; ball.posy = xrndn.mb(22) + 1
	xrndn.mb	22
	inca
	staa	[x+ball.posy]
	; ball.dirx = xrndn.mb(2) * 2 - 1
	xrndn.mb	2
	asla
	deca
	staa	[x+ball.dirx]
	; ball.diry = xrndn.mb(2) * 2 - 1
	xrndn.mb	2
	asla
	deca
	staa	[x+ball.diry]
	; next entry
	addx.mb	@ball
	.end
	inca
	cpx	ballsEnd
	bne	each_ball
	.end
	vram.clear
mainloop:
	.scope

	ldx	balls
eachball1:
	movebound [x+ball.posx], [x+ball.dirx], 0, 30
	movebound [x+ball.posy], [x+ball.diry], 0, 22
	.save	x
	ldmb	[ball_id],[x+ball.id]
	vram.fromxy [x+ball.posx], [x+ball.posy]
	pcg.chkcircle2x2.put 0
	pcg.chkcircle2x2.putattr 1
	pcg.chkcircle2x2.fill [ball_id],2
	ldaa	[ball_id]
	adda	'A'
	staa	[x]
	.end
	addx.mb	@ball
	cpx	ballsEnd
	jne	eachball1

	vram.refresh

	ldx	balls
eachball2:
	.save	x
	vram.fromxy [x+ball.posx], [x+ball.posy]
	pcg.chkcircle2x2.erase 0
	pcg.chkcircle2x2.eraseattr 7,0,1
	pcg.chkcircle2x2.fill 0,2
	.end
	addx.mb	@ball
	cpx	ballsEnd
	jne	eachball2

	jmp	mainloop
	.end

balls:
	.ds	@ball * 10
ballsEnd:

ball_id:
	.ds	1

	.pcgpage mainpage,cram:0x80

	.pcg	chkcircle2x2, 2,2, 3,96, 2:0
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
	.include "xrnd.inc"
	.include "jbranch.inc"
	.include "oputil.inc"
	.include "vram32x24x3.inc"
