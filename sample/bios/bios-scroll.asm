;;;=============================================================================
;;; bios-scroll.asm
;;;=============================================================================
	.org	0x1000
scroll_down:
	.scope
	bios.cls
	bios.locate 0,0
	bios.puts "Scroll-Down. Push [\x88] for Up\0"
loop:
	bios.pick
	cmpa	30
	beq	scroll_up
	bios.locate 15,1
	bios.puthex.mw [num]
	inc.w	[num]
	bios.scrolldown 0xc100 + 32
	bra	loop
	.end
	
scroll_up:
	.scope
	bios.cls
	bios.locate 0,23
	bios.puts "Scroll-Up. Push [\x87] for Down\0"
loop:
	bios.pick
	cmpa	31
	beq	scroll_down
	bios.locate 15,22
	bios.puthex.mw [num]
	inc.w	[num]
	bios.scrollup 0xc3e0 - 32
	bra	loop
	.end

num:	.dw	0

	.include "bios.inc"

inc.w:
	.macro	num
	ldx	num
	inx
	stx	num
	.end

dec.w:
	.macro	num
	ldx	num
	dex
	stx	num
	.end
