;;;==============================================================================
;;; bios.inc
;;;==============================================================================

;;;------------------------------------------------------------------------------
;;; bios.keyclick
;;; Sets the key click status.
;;;  Format: bios.keyclick click, mode=0
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
;;; bios.putdec.a
;;;  Format: bios.putdec.a
;;;  Param:  A .. Byte-sized number to print
;;;------------------------------------------------------------------------------
bios.putdec.a:
	.macro
	bios.todec.a
	bios.putn.x_b
	.end

;;;------------------------------------------------------------------------------
;;; bios.putdec.mb
;;;  Format: bios.putdec.mb num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Byte-sized number to print
;;;------------------------------------------------------------------------------
bios.putdec.mb:
	.macro	num
	ldaa	num
	bios.putdec.a
	.end

;;;------------------------------------------------------------------------------
;;; bios.putdec.x
;;;  Format: bios.putdec.x
;;;  Param:  X .. Word-sized number to print
;;;------------------------------------------------------------------------------
bios.putdec.x:
	.macro
	bios.todec.x
	bios.putn.x_b
	.end

;;;------------------------------------------------------------------------------
;;; bios.putdec.mw
;;;  Format: bios.putdec.mw num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Word-sized number to print
;;;------------------------------------------------------------------------------
bios.putdec.mw:
	.macro	num
	ldx	num
	bios.putdec.x
	.end

;;;------------------------------------------------------------------------------
;;; bios.puthex.a
;;;  Format: bios.puthex.a
;;;  Param:  A .. Byte-sized number to print
;;;------------------------------------------------------------------------------
bios.puthex.a:
	.macro
	bios.tohex.a
	bios.putn.x_b
	.end

;;;------------------------------------------------------------------------------
;;; bios.puthex.mb
;;;  Format: bios.puthex.mb num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Byte-sized number to print
;;;------------------------------------------------------------------------------
bios.puthex.mb:
	.macro	num
	ldaa	num
	bios.puthex.a
	.end

;;;------------------------------------------------------------------------------
;;; bios.puthex.x
;;;  Param:  X .. Word-sized number to print
;;;  Format: bios.puthex.mw num
;;;------------------------------------------------------------------------------
bios.puthex.x:
	.macro
	bios.tohex.x
	bios.putn.x_b
	.end

;;;------------------------------------------------------------------------------
;;; bios.puthex.mw
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Word-sized number to print
;;;  Format: bios.puthex.mw num
;;;------------------------------------------------------------------------------
bios.puthex.mw:
	.macro	num
	ldx	num
	bios.puthex.x
	.end

;;;------------------------------------------------------------------------------
;;; bios.todec.a
;;;  Format: bios.todec.a
;;;  Param:  A .. Byte-sized number to convert
;;;  Return: X .. Address of the result string
;;;          B .. Length of the result string
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.todec.a:
	.macro
	clr	[0x0160]
	staa	[0x0161]
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
;;; bios.todec.x
;;;  Format: bios.todec.mw num
;;;  Param:  X .. Word-sized number to convert
;;;  Return: X .. Address of the result string
;;;          B .. Length of the result string
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.todec.x:
	.macro
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
;;; bios.todec.mb
;;;  Format: bios.todec.mb num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Byte-sized number to convert
;;;  Return: X .. Address of the result string
;;;          B .. Length of the result string
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.todec.mb:
	.macro	num
	ldaa	num
	bios.todec.a
	.end

;;;------------------------------------------------------------------------------
;;; bios.todec.mw
;;;  Format: bios.todec.mw num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Word-sized number to convert
;;;  Return: X .. Address of the result string
;;;          B .. Length of the result string
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.todec.mw:
	.macro	num
	ldx	num
	bios.todec.x
	.end

;;;------------------------------------------------------------------------------
;;; bios.tohex.a
;;;  Format: bios.tohex.a
;;;  Param:  A .. Byte-sized number to convert
;;;  Return: X .. Address of the result string
;;;          B .. Length of the result string
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.tohex.a:
	.macro
	ldx	0x0170
	jsr	bios._tohex
	ldab	2
	.end

;;;------------------------------------------------------------------------------
;;; bios.tohex.mb
;;;  Format: bios.tohex.mb num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Byte-sized number to convert
;;;  Return: X .. Address of the result string
;;;          B .. Length of the result string
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.tohex.mb:
	.macro	num
	ldaa	num
	bios.tohex.a
	.end

;;;------------------------------------------------------------------------------
;;; bios.tohex.x
;;;  Format: bios.todec.x
;;;  Param:  X .. Word-sized number to convert
;;;  Return: X .. Address of the result string
;;;          B .. Length of the result string
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.tohex.x:
	.macro
	stx	[0x0160]
	ldaa	[0x0161]
	ldx	0x0172
	jsr	bios._tohex
	ldaa	[0x0160]
	ldx	0x0170
	jsr	bios._tohex
	ldab	4
	.end

;;;------------------------------------------------------------------------------
;;; bios.tohex.mw
;;;  Format: bios.todec.mw num
;;;  Param:  num [IMM, DIR, IDX, EXT] .. Word-sized number to convert
;;;  Return: X .. Address of the result string
;;;          B .. Length of the result string
;;;  Broken: A
;;;------------------------------------------------------------------------------
bios.tohex.mw:
	.macro	num
	ldx	num
	bios.tohex.x
	.end

;;;------------------------------------------------------------------------------
;;; bios.putc.a
;;; Prints a character of ASCII code specified by accumulator A
;;;  Format: bios.putc.a
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.putc.a:
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
	bios.putc.a
	.end

;;;------------------------------------------------------------------------------
;;; bios.puts.x
;;; Putss a string of ASCII characters that is terminated with zero on the screen.
;;;  Format: bios.puts.x
;;;  Param:  X .. Address of the string
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.puts.x:
	.macro
	jsr	bios._puts
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
	bios.puts.x
	.end

;;;------------------------------------------------------------------------------
;;; bios.filln.a_b
;;;  Format: bios.filln ch,len
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.filln.a_b:
	.macro
	jsr	bios._filln
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
	bios.filln.a_b
	.end

;;;------------------------------------------------------------------------------
;;; bios.putn.x_b
;;;  Format: bios.putn.x_b
;;;  Param:  X .. Address of string
;;;          B .. Length of string
;;;  Broken: A, B, X
;;;------------------------------------------------------------------------------
bios.putn.x_b:
	.macro
	jsr	bios._putn
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
	bios.putn.x_b
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

;;;------------------------------------------------------------------------------
;;; Helper procedures
;;;------------------------------------------------------------------------------
bios._tohex:
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

bios._filln:
	.scope
loop:	
	tstb
	beq	done
	.save	x,a,b
	bios.putc.a
	.end
	decb
	bne	loop
done:	
	rts
	.end

bios._puts:
	.scope
loop:	
	ldaa	[x]
	beq	done
	.save	x
	bios.putc.a
	.end
	inx
	bra	loop
done:	
	rts
	.end

bios._putn:
	.scope
loop:	
	tstb
	beq	done
	ldaa	[x]
	.save	x,b
	bios.putc.a
	.end
	inx
	decb
	bne	loop
done:	
	rts
	.end
