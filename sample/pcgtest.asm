	.org	0x2000

	pcgpage.circles.store

loop:

	.scope
	ldx		balls
loop:
	stx		[restore]
	XY2CodeAddr	[x+posx],[x+posy]
	pcg.circle2x2.erase
	CodeAddr2AttrAddr
	pcg.circle2x2.eraseattr 7,0
restore:	.equ $+1
	ldx		0x0000
	AddX	4
	cpx		ballsEnd
	bne		loop
	.end

	.scope
	ldx		balls
loop:
	stx		[restore]
	MoveBound	[x+posx],[x+dirx],0,30
	MoveBound	[x+posy],[x+diry],0,22
restore:	.equ $+1
	ldx		0x0000
	AddX	4
	cpx		ballsEnd
	bne		loop
	.end

	.scope
	ldx		balls
loop:
	stx		[restore]
	XY2CodeAddr	[x+posx],[x+posy]
	pcg.circle2x2.put
	CodeAddr2AttrAddr
	pcg.circle2x2.putattr 2,0
restore:	.equ $+1
	ldx		0x0000
	AddX	4
	cpx		ballsEnd
	bne		loop
	.end

	ldx		0x1000
delay:
	dex
	bne		delay

	jmp		loop

posx:	.equ	0
posy:	.equ	1
dirx:	.equ	2
diry:	.equ	3

balls:
	.db	20,10,1,1
	.db	23,13,1,1
	.db	20,4,0xff,0xff
	.db	28,12,1,1
	.db	10,11,0xff,1
	.db	9,17,1,0xff
	.db	18,12,1,1
	.db	0,15,1,1
	.db	21,12,0xff,1
	.db	27,19,1,0xff
ballsEnd:

	XY2CodeAddr	30,0
	pcg.circle2x2.put
	CodeAddr2AttrAddr
	pcg.circle2x2.putattr 2,0

	XY2CodeAddr	16,12
	pcg.circle2x2.put
	CodeAddr2AttrAddr
	pcg.circle2x2.putattr 2,0

	XY2CodeAddr	0,22
	pcg.circle2x2.put
	CodeAddr2AttrAddr
	pcg.circle2x2.putattr 2,0

	XY2CodeAddr	30,22
	pcg.circle2x2.put
	CodeAddr2AttrAddr
	pcg.circle2x2.putattr 2,0

	rts


	.pcgpage circles,user,32
	
	.pcg	circle1x1,1,1
	.db		b"..####.."
	.db		b".#....#."
	.db		b"#......#"
	.db		b"#......#"
	.db		b"#......#"
	.db		b"#......#"
	.db		b".#....#."
	.db		b"..####.."
	.end

	.pcg	dblcircle1x1,1,1
	.db		b"..####.."
	.db		b".#....#."
	.db		b"#..##..#"
	.db		b"#.#..#.#"
	.db		b"#.#..#.#"
	.db		b"#..##..#"
	.db		b".#....#."
	.db		b"..####.."
	.end
	
	.pcg	circle2x2,2,2
	.db		b".....######....."
	.db		b"...##......##..."
	.db		b"..#..........#.."
	.db		b".#............#."
	.db		b".#............#."
	.db		b"#..............#"
	.db		b"#..............#"
	.db		b"#..............#"
	.db		b"#..............#"
	.db		b"#..............#"
	.db		b"#..............#"
	.db		b".#............#."
	.db		b".#............#."
	.db		b"..#..........#.."
	.db		b"...##......##..."
	.db		b".....######....."
	.end
	
	.pcg	dblcircle2x2,2,2
	.db		b".....######....."
	.db		b"...##......##..."
	.db		b"..#..........#.."
	.db		b".#............#."
	.db		b".#....####....#."
	.db		b"#....#....#....#"
	.db		b"#...#......#...#"
	.db		b"#...#......#...#"
	.db		b"#...#......#...#"
	.db		b"#...#......#...#"
	.db		b"#....#....#....#"
	.db		b".#....####....#."
	.db		b".#............#."
	.db		b"..#..........#.."
	.db		b"...##......##..."
	.db		b".....######....."
	.end
	
	.pcg	blank2x1,2,1
	.db		b"................"
	.db		b"................"
	.db		b"................"
	.db		b"................"
	.db		b"................"
	.db		b"................"
	.db		b"................"
	.db		b"................"
	.end
	
	.pcg	chkcircle2x2,2,2
	.db		b".....######....."
	.db		b"...##...#####..."
	.db		b"..#.....######.."
	.db		b".#......#######."
	.db		b".#......#######."
	.db		b"#.......########"
	.db		b"#.......########"
	.db		b"#.......########"
	.db		b"########.......#"
	.db		b"########.......#"
	.db		b"########.......#"
	.db		b".#######......#."
	.db		b".#######......#."
	.db		b"..######.....#.."
	.db		b"...#####...##..."
	.db		b".....######....."
	.end
	
	.end

;-----------------------------------------------------------------------------
; Format: AddX num
;-----------------------------------------------------------------------------
AddX:
	.macro	num
	stx		[result]
	ldaa	num
	clrb
	adda	[lowbyte]
	adcb	[highbyte]
	staa	[lowbyte]
	stab	[highbyte]
result:	.equ $+1
highbyte: .equ $+1
lowbyte: .equ $+2
	ldx		0x0000
	.end

;-----------------------------------------------------------------------------
; Format: MoveBound pos,dir,min,max
; Param: posx [IMM, DIR, IDX, EXT] .. Position
;        dir [IMM, DIR, IDX, EXT] .. Direction
;        min [IMM, DIR, IDX, EXT] .. Minimum value
;        max [IMM, DIR, IDX, EXT] .. Maximum value
;-----------------------------------------------------------------------------
MoveBound:
	.macro	pos,dir,min,max
	ldaa	pos
	cmpa	max
	bne		rel1
	ldaa	0xff
	staa	dir
	bra		rel2
rel1:
	cmpa	min
	bne		rel2
	ldaa	1
	staa	dir
rel2:
	ldaa	pos
	adda	dir
	staa	pos
	.end

;-----------------------------------------------------------------------------
; Format: XY2CodeAddr posx,posy
; Param: posx [IMM, DIR, IDX, EXT] .. X position
;        posy [IMM, DIR, IDX, EXT] .. Y position
; Output: x .. Code address that begins from 0xc100
; Broken: a, b
;-----------------------------------------------------------------------------
XY2CodeAddr:
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
	ldx		0x0000
	.end

;-----------------------------------------------------------------------------
; Format: CodeAddr2AttrAddr
; Input: x .. Code address that begins from 0xc100
; Output: x .. Attribute address that begins from 0xc500
; Broken: a
;-----------------------------------------------------------------------------
CodeAddr2AttrAddr:
	.macro
	stx		[result]
	ldaa	0x04
	adda	[result]
	staa	[result]
result:	.equ $+1
	ldx		0x0000
	.end
