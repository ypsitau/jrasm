	.org	0x1000

ball:	.struct
posx:	.ds	@byte
posy:	.ds	@byte
dirx:	.ds	@byte
diry:	.ds	@byte
	.end
	
	clra
	staa	{0x00}		; disable click sound
	jsr	0xec7f		; clear screen with attribute data in {0x0e}
	
	pcgpage.mainpage.store

mainloop:

	.scope
	ldx	balls
loop:
	.save	x
	xy2codeaddr [x+ball.posx], [x+ball.posy]
	pcg.chkcircle2x2.erase
	code2attraddr
	pcg.chkcircle2x2.eraseattr 7, 0
	.end
	addx	4
	cpx	ballsEnd
	bne	loop
	.end

	.scope
	ldx	balls
loop:
	.save	x
	movebound [x+ball.posx], [x+ball.dirx], 0, 30
	movebound [x+ball.posy], [x+ball.diry], 0, 22
	.end
	addx	4
	cpx	ballsEnd
	bne	loop
	.end

	.scope
	ldx	balls
loop:
	.save	x
	xy2codeaddr [x+ball.posx], [x+ball.posy]
	pcg.chkcircle2x2.put
	code2attraddr
	pcg.chkcircle2x2.putattr
	.end
	
	addx	4
	cpx	ballsEnd
	bne	loop
	.end

	ldx	0x1000
delay:
	dex
	bne	delay

	jmp	mainloop

balls:
	.db	20, 10, 1, 1
	.db	23, 13, 1, 1
	.db	20, 4, -1, -1
	.db	28, 12, 1, 1
	.db	10, 11, -1, 1
	.db	9, 17, 1, -1
	.db	18, 12, 1, 1
	.db	0, 15, 1, 1
	.db	21, 12, -1, 1
	.db	27, 19, 1, -1
ballsEnd:

	.pcgpage mainpage,cram:32

	.pcg	chkcircle2x2,2,2,1,32,2:0
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

;-----------------------------------------------------------------------------
; Format: addx num
; Param: num [IMM, DIR, IDX, EXT] .. Added 8bit number
;-----------------------------------------------------------------------------
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

;-----------------------------------------------------------------------------
; Format: movebound pos,dir,min,max
; Param: posx [IMM, DIR, IDX, EXT] .. Position
;        dir [IMM, DIR, IDX, EXT] .. Direction
;        min [IMM, DIR, IDX, EXT] .. Minimum value
;        max [IMM, DIR, IDX, EXT] .. Maximum value
;-----------------------------------------------------------------------------
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

;-----------------------------------------------------------------------------
; Format: xy2codeaddr posx,posy
; Param: posx [IMM, DIR, IDX, EXT] .. X position
;        posy [IMM, DIR, IDX, EXT] .. Y position
; Output: x .. Code address that begins from 0xc100
; Broken: a, b
;-----------------------------------------------------------------------------
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

;-----------------------------------------------------------------------------
; Format: code2attraddr
; Input: x .. Code address that begins from 0xc100
; Output: x .. Attribute address that begins from 0xc500
; Broken: a
;-----------------------------------------------------------------------------
code2attraddr:
	.macro
	stx	[result]
	ldaa	0x04
	adda	[result]
	staa	[result]
result:	.equ $+1
	ldx	0x0000
	.end
