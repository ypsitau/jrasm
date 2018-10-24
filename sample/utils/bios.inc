;;;==============================================================================
;;; bios.inc
;;;==============================================================================

;;;------------------------------------------------------------------------------
;;; bios.keyclick
;;; Specifies the position of cursor.
;;;  Format: bios.keyclick flag
;;;  Param: flag [IMM, DIR, IDX, EXT] .. Click sound is enabled (1) or disabled (0)
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.keyclick:
	.macro	flag
	ldaa	flag
	clc
	rora
	rora
	rora
	staa	{0x00}
	.end
	
;;;------------------------------------------------------------------------------
;;; bios.color
;;; Specifies color attribute.
;;;  Format: bios.color fg, bg, user=0
;;;  Param: fg [IMM] .. Color code for foreground (0 to 7)
;;;         bg [IMM] .. Color code for background (0 to 7)
;;;         user [IMM] .. Specifies normal (0) or user-defined mode (1)
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.color:
	.macro	fg, bg, user=0
	ldaa	fg | (bg << 3) | (user << 6)
	staa	{0x0e}
	.end

;;;------------------------------------------------------------------------------
;;; bios.locate
;;; Specifies the position of cursor.
;;;  Format: bios.locate x, y
;;;  Param: x [IMM, DIR, IDX, EXT] .. X-position.
;;;         y [IMM, DIR, IDX, EXT] .. Y-position.
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.locate:
	.macro	x, y
	ldaa	x
	staa	{0x09}
	ldaa	y
	staa	{0x0a}
	.end

;;;------------------------------------------------------------------------------
;;; bios.breakkey
;;; Specifies the position of cursor.
;;;  Format: bios.breakkey flag
;;;  Param: flag [IMM, DIR, IDX, EXT] .. Break key is enabled (1) or disabled (0)
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.breakkey:
	.macro	flag
	ldaa	flag
	staa	{0x0b}
	.end

;;;------------------------------------------------------------------------------
;;; bios.cls
;;; Clears screen. 
;;;  Format: bios.cls
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.cls:
	.macro
	jsr	0xec7f
	.end

;;;------------------------------------------------------------------------------
;;; bios.putc
;;; Prints ASCII character specified by accumulator A.
;;;  Format: bios.putc
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.putc:
	.macro
	jsr	0xebe7
	.end
