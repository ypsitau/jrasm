;;;==============================================================================
;;; bios.inc
;;;==============================================================================

;;;------------------------------------------------------------------------------
;;; bios.keystat
;;; Sets the key status.
;;;  Format: bios.keystat click, mode=0
;;;  Param: click [IMM] .. Click sound is disabled (0) or enabled (1)
;;;         mode [IMM] .. Sets key-in mode to BASIC (0) or neutral (1).
;;;                       Default is 0.
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.keyclick:
	.macro	click, mode=0
	ldaa	(click << 6) | (mode << 7)
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
;;; Prints a character of ASCII code specified by accumulator A
;;;  Format: bios.putc
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.putc:
	.macro
	jsr	0xebe7
	.end

;;;------------------------------------------------------------------------------
;;; bios.puts
;;; Putss a string of ASCII characters that is terminated with zero on the screen.
;;;  Format: bios.puts addr
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.puts:
	.macro addr
	ldx	addr
	jsr	bios._puts
	.end

bios._puts:
	.scope
loop:	
	ldaa	[x]
	beq	done
	.save	x
	bios.putc
	.restore x
	.end
	inx
	bra	loop
done:	
	rts
	.end

;;;------------------------------------------------------------------------------
;;; bios.pick
;;; Returns a key code that is being pressed as well as a status of joysticks.
;;;  Format: bios.pick
;;;  Returns: A: keyboard, {0x02}: joystick#1, {0x03}: joystick#2
;;;  Broken: (unknown)
;;;------------------------------------------------------------------------------
bios.pick:
	.macro
	jsr	0xe8cb
	ldaa	{0x01}
	.end

;;;------------------------------------------------------------------------------
;;; bios.scrollup
;;; Scrolls the screen up.
;;;  Format: bios.scrollup addr
;;;------------------------------------------------------------------------------
bios.scrollup:
	.macro	addr=0xc3e0
	ldx	addr
	stx	{0x22}
	jsr	0xed0d
	.end

;;;------------------------------------------------------------------------------
;;; bios.scrolldown
;;; Scrolls the screen down.
;;;  Format: bios.scrollup addr
;;;------------------------------------------------------------------------------
bios.scrolldown:
	.macro	addr=0xc100
	ldx	addr
	stx	{0x22}
	jsr	0xed76
	.end
