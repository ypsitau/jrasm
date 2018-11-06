;;;=============================================================================
;;; skigame.asm
;;;=============================================================================
	.org	0x1000
	bios.cls
	ldmw	[score],0
	ldmb	[player.posx],16
	ldmb	[course.posx],0
	ldmb	[course.width],31
loop:
	bios.locate 0,0
	bios.puts "SCORE\0"
	bios.putdec.mb [score],4
	bios.scrolldown 0xc120
	bios.locate 0,1
	bios.filln 0,32
	;---------------------------
	; show a course
	;---------------------------
	bios.locate [course.posx],1
	bios.putc 0x9a
	ldaa	[course.posx]
	adda	[course.width]
	staa	[byte1]
	bios.locate [byte1],1
	bios.putc 0x9a
	;---------------------------
	; show a gate
	;---------------------------
	.scope
	xrndn.mb 8
	tsta
	bne	skip
	xrndn.mb 32 - 8 - 2
	inca
	staa	[gate.posx]
	bios.locate [gate.posx],1
	bios.puts "\x81\x9b\x9b\x9b\x9b\x9b\x9b\x81\0"
skip:
	.end
	;---------------------------
	; hit test
	;---------------------------
	.scope
	ldx	0xc100 + 17 * 32
	addx.mb	[player.posx]
	ldaa	[x]
	cmpa	0x9b
	bne	nohit_gate
	inc	[score]

	; erase gate
loop1:	ldaa	[x]
	cmpa	0x9b
	bne	rel1
	dex
	bra	loop1
rel1:	inx
loop2:	ldaa	[x]
	cmpa	0x9b
	bne	rel2
	ldmb	[x],0
	inx
	bra	loop2
rel2:
	bra	nohit_tree
nohit_gate:
	cmpa	0x81
	bne	nohit_tree
	dec	[score]
nohit_tree:
	.end

	;---------------------------
	; move player
	;---------------------------
	bios.pick
	cmpa	28
	bne	rel1
	ldaa	[player.posx]
	cmpa	31
	beq	rel2
	inca
	staa	[player.posx]
	bra	rel2
rel1:
	cmpa	29
	bne	rel2
	ldaa	[player.posx]
	cmpa	0
	beq	rel2
	deca
	staa	[player.posx]
rel2:
	bios.locate [player.posx], 17
	bios.putc 'A'
	jmp	loop

	.wseg
byte1:
	.ds	1
byte2:
	.ds	1
word1:
	.ds	2
word2:
	.ds	2
score:
	.ds	1
course.posx:
	.ds	1
course.width:
	.ds	1
gate.posx:
	.ds	1
player.posx:
	.ds	1

	.include "xrnd.inc"
	.include "bios.inc"
	.include "oputil.inc"
