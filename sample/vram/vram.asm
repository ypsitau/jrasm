	.org	0x1000
	
transfer:
	.scope
	ldmw	[srcp],vrambuff
	ldmw	[dstp_char],0xc100
	ldmw	[dstp_attr],0xc500
loop:	
	ldx	[srcp]
	cpx	vrambuff_end
	beq	done
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
	bra	loop
done:
	rts
	.wseg
srcp:
	.ds	2
dstp_char:
	.ds	2
dstp_attr:
	.ds	2
	.end
	
	.wseg
vrambuff:
	.ds	32 * 2 * 24
vrambuff_end:

	.include "oputil.inc"
