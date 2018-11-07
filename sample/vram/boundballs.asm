;;;=============================================================================
;;; boundballs.asm
;;;=============================================================================
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
	ldmb	{0x00},0	; disable click sound
	bios.cls

	pcgpage.mainpage.store

	;; Initialize parameter table
	.scope
	ldx	balls
each_ball:
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
	xy2vram vram,[x+ball.posx], [x+ball.posy]
	pcg.chkcircle2x2.put 0
	pcg.chkcircle2x2.putattr 1
	.end
	addx.mb	@ball
	cpx	ballsEnd
	jne	eachball1

	vram.refresh

	ldx	balls
eachball2:
	.save	x
	xy2vram vram,[x+ball.posx], [x+ball.posy]
	pcg.chkcircle2x2.erase 0
	pcg.chkcircle2x2.eraseattr ,,1
	.end
	addx.mb	@ball
	cpx	ballsEnd
	jne	eachball2

	jmp	mainloop
	.end

balls:
	.ds	@ball * 10
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

;;;-----------------------------------------------------------------------------
;;; delay
;;;  Format: delay num
;;;  Param: num [IMM, DIR, IDX, EXT] .. Delay duration
;;;-----------------------------------------------------------------------------
delay:
	.macro	num
	ldx	num
loop:
	dex
	bne	loop
	.end

	.include "bios.inc"
	.include "xrnd.inc"
	.include "jbranch.inc"
	.include "oputil.inc"
	.include "utils.inc"
	.include "vram.inc"
