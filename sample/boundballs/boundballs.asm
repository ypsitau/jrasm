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
	clra
	staa	{0x00}		; disable click sound
	jsr	0xec7f		; clear screen with attribute data in {0x0e}
	
	pcgpage.mainpage.store

	;; Initialize parameter table
	.scope
	ldx	balls
each_ball:
	; ball.posx = xrndn(30) + 1
	xrndn	30
	inca
	staa	[x+ball.posx]
	; ball.posy = xrndn(22) + 1
	xrndn	22
	inca
	staa	[x+ball.posy]
	; ball.dirx = xrndn(2) * 2 - 1
	xrndn	2
	asla
	deca
	staa	[x+ball.dirx]
	; ball.diry = xrndn(2) * 2 - 1
	xrndn	2
	asla
	deca
	staa	[x+ball.diry]
	; next entry
	addx	@ball
	cpx	ballsEnd
	bne	each_ball
	.end
	
mainloop:
	.scope
	ldx	balls
each_ball:
	.save	x
	xy2codeaddr [x+ball.posx], [x+ball.posy]
	pcg.chkcircle2x2.erase
	code2attraddr
	pcg.chkcircle2x2.eraseattr

	.restore x
	movebound [x+ball.posx], [x+ball.dirx], 0, 30
	movebound [x+ball.posy], [x+ball.diry], 0, 22

	xy2codeaddr [x+ball.posx], [x+ball.posy]
	pcg.chkcircle2x2.put
	code2attraddr
	pcg.chkcircle2x2.putattr

	.restore x
	.end
	addx	4
	cpx	ballsEnd
	jne	each_ball
	.end

	delay	0x10
	
	jmp	mainloop

balls:
	.ds	@ball * 100
ballsEnd:

	.pcgpage mainpage,cram:32

	.pcg	chkcircle2x2,2,2,,,2:0
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
;;; Format: movebound pos,dir,min,max
;;; Param: posx [IMM, DIR, IDX, EXT] .. Position
;;;        dir [IMM, DIR, IDX, EXT] .. Direction
;;;        min [IMM, DIR, IDX, EXT] .. Minimum value
;;;        max [IMM, DIR, IDX, EXT] .. Maximum value
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
	ldaa	pos
	adda	dir
	staa	pos
	.end

;;;-----------------------------------------------------------------------------
;;; Format: jXX addr16
;;; Param: addr16 [EXT] .. Address
;;;-----------------------------------------------------------------------------
jcc:	.macro addr16
	bcs	skip
	jmp	addr16
skip:	.end

jcs:	.macro addr16
	bcc	skip
	jmp	addr16
skip:	.end

jeq:	.macro addr16
	bne	skip
	jmp	addr16
skip:	.end

jge:	.macro addr16
	blt	skip
	jmp	addr16
skip:	.end

jgt:	.macro addr16
	ble	skip
	jmp	addr16
skip:	.end

jhi:	.macro addr16
	bls	skip
	jmp	addr16
skip:	.end

jle:	.macro addr16
	bgt	skip
	jmp	addr16
skip:	.end

jls:	.macro addr16
	bhi	skip
	jmp	addr16
skip:	.end

jlt:	.macro addr16
	bge	skip
	jmp	addr16
skip:	.end

jmi:	.macro addr16
	bpl	skip
	jmp	addr16
skip:	.end

jne:	.macro addr16
	beq	skip
	jmp	addr16
skip:	.end

jpl:	.macro addr16
	bmi	skip
	jmp	addr16
skip:	.end

jvc:	.macro addr16
	bvs	skip
	jmp	addr16
skip:	.end

jvs:	.macro addr16
	bvc	skip
	jmp	addr16
skip:	.end
	
	
;;;-----------------------------------------------------------------------------
;;; Format: delay num
;;; Param: num [IMM, DIR, IDX, EXT] .. Delay duration
;;;-----------------------------------------------------------------------------
delay:
	.macro	num
	ldx	num
loop:
	dex
	bne	loop
	.end

;;;-----------------------------------------------------------------------------
;;; Format: addx num
;;; Param: num [IMM, DIR, IDX, EXT] .. Added 8bit number
;;;-----------------------------------------------------------------------------
addx:
	.macro	num
	stx	[result]
	ldaa	num
	clrb
	adda	[lowbyte]
	adcb	[highbyte]
	staa	[lowbyte]
	stab	[highbyte]
result:	.equ $+1
highbyte: .equ $+1
lowbyte: .equ $+2
	ldx	0x0000
	.end

;;;-----------------------------------------------------------------------------
;;; Format: xy2codeaddr posx,posy
;;; Param: posx [IMM, DIR, IDX, EXT] .. X position
;;;        posy [IMM, DIR, IDX, EXT] .. Y position
;;; Output: x .. Code address that begins from 0xc100
;;; Broken: a, b
;;;-----------------------------------------------------------------------------
xy2codeaddr:
	.macro	posx,posy
	clra
	ldab	posy
	clc
	rorb
	rora
	rorb
	rora
	rorb
	rora
	adda	posx
	adcb	0xc1
	stab	[highbyte]
	staa	[lowbyte]
highbyte: .equ $+1
lowbyte: .equ $+2
	ldx	0x0000
	.end

;;;-----------------------------------------------------------------------------
;;; Format: code2attraddr
;;; Input: x .. Code address that begins from 0xc100
;;; Output: x .. Attribute address that begins from 0xc500
;;; Broken: a
;;;-----------------------------------------------------------------------------
code2attraddr:
	.macro
	stx	[result]
	ldaa	0x04
	adda	[result]
	staa	[result]
result:	.equ $+1
	ldx	0x0000
	.end

	.include "xorshift.inc"
