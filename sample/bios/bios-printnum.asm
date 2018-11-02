;;;=============================================================================
;;; bios-printnum.asm
;;;=============================================================================
	.org	0x1000

	bios.cls

	ldx	0x0000
	stx	[num]
loop:
	bios.scrollup

	.scope
	bios.locate 0,23
	bios.puthex.mw [num]
	.end

	.scope
	bios.locate 6,23
	bios.todec.mw [num]
	.save	x,b
	negb
	addb	6
	ldaa	'_'
	bios.filln.a_b
	.end
	bios.putn.x_b
	.end

	.scope
	bios.locate 14,23
	bios.putdec.mw [num]
	.end

	.scope
	bios.locate 22,23
	bios.puthex.mb [num+1]
	.end

	.scope
	bios.locate 26,23
	bios.putdec.mb [num+1]
	.end

	ldx	[num]
	inx
	stx	[num]

	jmp	loop

	.include "bios.inc"

	.wseg
num:	.dw	0
