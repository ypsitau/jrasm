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
;;; bios.putdec.w
;;;  Format: bios.putdec.w num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Word-sized number to print
;;;------------------------------------------------------------------------------
bios.putdec.w:
	.macro	num
	bios.todec.w num
	jsr	bios._putn
	.end

;;;------------------------------------------------------------------------------
;;; bios.puthex.acca
;;;------------------------------------------------------------------------------
bios.puthex.acca:
	.macro
	jsr	bios._puthex.acca
	.end

;;;------------------------------------------------------------------------------
;;; bios.puthex.b
;;;  Format: bios.puthex.b num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Byte-sized number to print
;;;------------------------------------------------------------------------------
bios.puthex.b:
	.macro	num
	ldaa	num
	jsr	bios._tohex.acca
	.end

;;;------------------------------------------------------------------------------
;;; bios.puthex.w
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Word-sized number to print
;;;  Format: bios.puthex.w num
;;;------------------------------------------------------------------------------
bios.puthex.w:
	.macro	num
	bios.tohex.w num
	jsr	bios._putn
	.end

;;;------------------------------------------------------------------------------
;;; bios.todec.w
;;;  Format: bios.todec.w num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Word-sized number to convert
;;;  Return: X .. Address of the result string
;;;          B .. Length of the result string
;;;------------------------------------------------------------------------------
bios.todec.w:
	.macro	num
	ldx	num
	stx	[0x0160]
	ldx	0x0160
	jsr	0xfe86
	ldx	{0xb7}
	tstb
	bne	nonzero
	ldaa	'0'
	staa	[x]
	incb
nonzero:
	.end

;;;------------------------------------------------------------------------------
;;; bios.tohex.b
;;;  Format: bios.todec.b num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Byte-sized number to convert
;;;  Return: X .. Address of the result string
;;;          B .. Length of the result string
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.tohex.b:
	.macro	num
	ldaa	num
	ldx	0x0170
	jsr	bios._tohex.acca
	ldab	2
	.end

;;;------------------------------------------------------------------------------
;;; bios.tohex.w
;;;  Format: bios.todec.w num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Word-sized number to convert
;;;  Return: X .. Address of the result string
;;;          B .. Length of the result string
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.tohex.w:
	.macro	num
	ldx	num
	stx	[0x0160]
	ldaa	[0x0161]
	ldx	0x0172
	jsr	bios._tohex.acca
	ldaa	[0x0160]
	ldx	0x0170
	jsr	bios._tohex.acca
	ldab	4
	.end

;;;------------------------------------------------------------------------------
;;; bios._puthex.acca
;;;------------------------------------------------------------------------------
bios._puthex.acca:
	ldx	0x0170
	jsr	bios._tohex.acca
	ldab	2
	jsr	bios._putn
	rts

;;;------------------------------------------------------------------------------
;;; bios._tohex.acca
;;; Converts accumulator A's value into a hexadecimal string.
;;;  Param: A .. the value
;;;         X .. address to store the result string
;;;------------------------------------------------------------------------------
bios._tohex.acca:
	.scope
	.save	a
	rora
	rora
	rora
	rora
	anda	0x0f
	cmpa	10
	blt	rel1
	adda	'A' - 10 - '0'
rel1:	adda	'0'
	staa	[x+0]
	.end
	anda	0x0f
	cmpa	10
	blt	rel2
	adda	'A' - 10 - '0'
rel2:	adda	'0'
	staa	[x+1]
	rts
	.end

;;;------------------------------------------------------------------------------
;;; bios.putc.acca
;;; Prints a character of ASCII code specified by accumulator A
;;;  Format: bios.putc.acca
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.putc.acca:
	.macro
	jsr	0xebe7
	.end

;;;------------------------------------------------------------------------------
;;; bios.putc
;;; Prints a character of ASCII code
;;;  Format: bios.putc ch
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.putc:
	.macro	ch
	ldaa	ch
	bios.putc.acca
	.end

;;;------------------------------------------------------------------------------
;;; bios.puts
;;; Putss a string of ASCII characters that is terminated with zero on the screen.
;;;  Format: bios.puts addr
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.puts:
	.macro	addr
	ldx	addr
	jsr	bios._puts
	.end

bios._puts:
	.scope
loop:	
	ldaa	[x]
	beq	done
	.save	x
	bios.putc.acca
	.end
	inx
	bra	loop
done:	
	rts
	.end

;;;------------------------------------------------------------------------------
;;; bios.filln
;;;  Format: bios.filln ch,len
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.filln:
	.macro	ch,len
	ldaa	ch
	ldab	len
	jsr	bios._filln
	.end

bios._filln:
	.scope
loop:	
	tstb
	beq	done
	.save	x,a,b
	bios.putc.acca
	.end
	decb
	bne	loop
done:	
	rts
	.end

;;;------------------------------------------------------------------------------
;;; bios.putn
;;;  Format: bios.putn addr,len
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.putn:
	.macro	addr,len
	ldx	addr
	ldab	len
	jsr	bios._putn
	.end

bios._putn:
	.scope
loop:	
	tstb
	beq	done
	ldaa	[x]
	.save	x,b
	bios.putc.acca
	.end
	inx
	decb
	bne	loop
done:	
	rts
	.end

;;;------------------------------------------------------------------------------
;;; bios.pick
;;; Returns a key code that is being pressed as well as a status of joysticks.
;;;  Format: bios.pick
;;;  Return: A: keyboard
;;;          {0x02}: joystick#1
;;;          {0x03}: joystick#2
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
;;;  Param:  addr [IMM, DIR, IDX, EXT]
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
