;;;=============================================================================
;;; vramcmn.inc
;;;=============================================================================
	.cseg
;;;------------------------------------------------------------------------------
;;; bios.color
;;; Specifies color attribute.
;;;  Format: bios.color fg, bg, user=0
;;;  Param: fg [IMM] .. Color code for foreground (0 to 7)
;;;         bg [IMM] .. Color code for background (0 to 7)
;;;         user [IMM] .. Specifies normal (0) or user-defined mode (1)
;;;  Broken: A
;;;------------------------------------------------------------------------------
vram.color:
	.macro	fg, bg, user=0
	ldaa	fg | (bg << 3) | (user << 6)
	staa	{0x0e}
	.end
