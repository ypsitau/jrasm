;;;=============================================================================
;;; bios-printnum.asm
;;;=============================================================================
	.org	0x1000

	bios.cls

	ldx	0x0000
	stx	[num]
loop:
	bios.puthex.mw [num]
	bios.putc ' '
	bios.todec.mw [num]
	.save	x,b
	negb
	addb	6
	ldaa	'_'
	bios.filln.a_b
	.end
	bios.putn.x_b
	bios.putc ' '
	bios.putdec.mw [num]
	bios.putc '\r'
	ldx	[num]
	inx
	stx	[num]
	bra	loop
	rts

	.include "bios.inc"

	.wseg
num:	.dw	0
