	.org	0x1000
	
margin_l: .equ	2
margin_r: .equ	2

transfer:
	.scope
	ldmw	[srcp],vrambuff + margin_l * 2
	ldmw	[dstp_char],0xc100
	ldmw	[dstp_attr],0xc500
	ldmb	[cnty],24
loopy:	
	ldmb	[cntx],32
loopx:	
	ldx	[srcp]
	ldaa	[x]
	inx
	ldab	[x]
	inx
	stx	[srcp]
	ldx	[dstp_char]
	staa	[x]
	inx
	stx	[dstp_char]
	ldx	[dstp_attr]
	stab	[x]
	inx
	stx	[dstp_attr]
	dec	[cntx]
	bne	loopx
	addx.mb	(margin_r + margin_l) * 2
	dec	[cnty]
	bne	loopy
done:
	rts
	.wseg
cntx:
	.ds	1
cnty:
	.ds	1
srcp:
	.ds	2
dstp_char:
	.ds	2
dstp_attr:
	.ds	2
	.end
	
	.wseg
vrambuff:
	.ds	(margin_l + 32 + margin_r) * 2 * 24
vrambuff_end:

	.include "oputil.inc"
