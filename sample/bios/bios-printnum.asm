;;;=============================================================================
;;; bios-printnum.asm
;;;=============================================================================
	.org	0x1000

	bios.cls

	ldx	0xff00
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
	subb	5
	negb
	ldaa	'*'
	bios.filln.a_b
	.end
	bios.putn.x_b
	.end

	.scope
	bios.locate 14,23
	bios.putdec.mw [num],5
	.end

	.scope
	bios.locate 22,23
	bios.puthex.mb [num+1]
	.end

	.scope
	bios.locate 26,23
	bios.putdec.mb [num+1],3
	.end

	ldx	[num]
	inx
	stx	[num]

	jmp	loop

	.wseg
num:	.dw	0

	.include "bios.inc"
