;;;=============================================================================
;;; vram.inc
;;;=============================================================================
	.cseg

refresh_vram:
	.scope
	ldmw	[srcp],vram
	ldmw	[dstp_char],0xc100
	ldmw	[dstp_attr],0xc500
loop:	
srcp:
	.equ	$+1
	ldx	0x0000
	cpx	vram.end
	beq	done
	ldaa	[x]
	inx
	ldab	[x]
	inx
	stx	[srcp]
dstp_char:
	.equ	$+1
	ldx	0x0000
	staa	[x]
	inx
	stx	[dstp_char]
dstp_attr:
	.equ	$+1
	ldx	0x0000
	stab	[x]
	inx
	stx	[dstp_attr]
	bra	loop
done:
	rts
	.end
	
	.wseg
vram:
	.ds	32 * 2 * 24
vram.end:

	.include "oputil.inc"
