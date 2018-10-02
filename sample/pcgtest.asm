	.org	0x2000

	pcgpage.circles.store

loop:

	XY2CodeAddr	[posx],0
	pcg.circle2x2.erase
	CodeAddr2AttrAddr
	pcg.circle2x2.putattr 0,0

	inc		[posx]

	XY2CodeAddr	[posx],0
	pcg.circle2x2.put
	CodeAddr2AttrAddr
	pcg.circle2x2.putattr 2,0
	
	ldx		0x1000
delay:
	dex
	bne		delay

	jmp		loop

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

posx:	.dw	0


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
