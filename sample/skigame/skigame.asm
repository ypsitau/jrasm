;;;=============================================================================
;;; skigame.asm
;;;=============================================================================
	.org	0x1000
	bios.cls
	ldmw	[score],0
	ldmb	[player.posx],16
	ldmb	[course.posx],4
	ldmb	[course.dirx],0
	ldmb	[course.width],32 - 4 - 4
mainloop:
	bios.locate 0,0
	bios.puts "SCORE\0"
	bios.putdec.mb [score],4
	bios.scrolldown 0xc120
	;---------------------------
	; show a player
	;---------------------------
	bios.locate [player.posx], 18
	bios.putc '.'
	;---------------------------
	; show a course
	;---------------------------
	bios.locate 0,1
	bios.filln 0,32
	bios.locate [course.posx],1
	bios.putc 0x8c
	ldaa	[course.posx]
	adda	[course.width]
	deca
	staa	[byte1]
	bios.locate [byte1],1
	bios.putc 0x8c
	;---------------------------
	; move course
	;---------------------------
	.scope
	ldaa	[course.dirx]
	cmpa	1
	bne	elsif1
	ldaa	[course.posx]
	adda	[course.width]
	deca
	cmpa	31
	beq	endif
	addmb	[course.posx],1
	bra	endif
elsif1:	cmpa	2
	bne	endif
	ldaa	[course.posx]
	beq	endif
	submb	[course.posx],1
endif:	
	.end

	.scope
	xrndn.mb 10
	cmpa	3
	bge	endif
	staa	[course.dirx]
endif:
	.end

	;---------------------------
	; show a gate
	;---------------------------
	.scope
	xrndn.mb 8
	tsta
	bne	skip
	ldaa	[course.width]
	suba	8 + 2
	xrndn.a
	inca
	adda	[course.posx]
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
	cmpa	28	; [right] key
	bne	rel1
	cmpmb	[player.posx],31
	beq	rel2
	addmb	[player.posx],1
	bra	rel2
rel1:
	cmpa	29	; [left] key
	bne	rel2
	cmpmb	[player.posx],0
	beq	rel2
	submb	[player.posx],1
rel2:
	bios.locate [player.posx], 17
	bios.putc 'A'
	jmp	mainloop

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
course.dirx:
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
