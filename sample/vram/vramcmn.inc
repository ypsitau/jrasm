;;;=============================================================================
;;; vramcmn.inc
;;;=============================================================================
	.cseg
;;;------------------------------------------------------------------------------
;;; vram.color
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

;;;-----------------------------------------------------------------------------
;;; vram.puts
;;;-----------------------------------------------------------------------------
vram.puts:
	.macro	str
	stx	[dstp]
	ldx	str
	stx	[srcp]
loop:
srcp:
	.equ	$+1
	ldx	0x0000
	ldaa	[x]
	beq	done
	inx
	stx	[srcp]
dstp:
	.equ	$+1
	ldx	0x0000
	staa	[x]
	ldaa	{0x0e}
	staa	[x+1]
	vram.nextx
	stx	[dstp]
	bra	loop
done:
	.end

;;;-----------------------------------------------------------------------------
;;; vram.fill
;;;-----------------------------------------------------------------------------
vram.fill:
	.macro	ch,cnt
	ldab	cnt
loop:
	ldaa	ch
	staa	[x]
	ldaa	{0x0e}
	staa	[x+1]
	vram.nextx
	decb
	bne	loop
	.end

;;;-----------------------------------------------------------------------------
;;; vram.clear
;;;-----------------------------------------------------------------------------
vram.clear:
	.macro
	jsr	vram._clear
	.end

vram._clear:
	.scope
	ldx	vram
	clra
loop:
	staa	[x]
	inx
	cpx	vram.end
	bne	loop
	rts
	.end

;;;-----------------------------------------------------------------------------
;;; vram.refresh
;;;-----------------------------------------------------------------------------
vram._refresh_without_margin:
	.scope
	ldmw	[srcp],vram
	ldmw	[charp],0xc100
	ldmw	[attrp],0xc500
loop:
srcp:
	.equ	$+1
	ldx	0x0000
	cpx	vram.end
	beq	done
	ldaa	[x]
	ldab	[x+1]
	vram.nextx
	stx	[srcp]
charp:
	.equ	$+1
	ldx	0x0000
	staa	[x]
	inx
	stx	[charp]
attrp:
	.equ	$+1
	ldx	0x0000
	stab	[x]
	inx
	stx	[attrp]
	bra	loop
done:
	rts
	.end

vram._refresh_with_margin:
	.scope
	ldmw	[srcp],vram + vram.margin@l * vram.bytes_per_char
	ldmw	[charp],0xc100
	ldmw	[attrp],0xc500
	ldmb	[cnty],24
loopy:
	ldmb	[cntx],32
srcp:
	.equ	$+1
loopx:
	ldx	0x0000
	ldaa	[x]
	ldab	[x+1]
	vram.nextx
	stx	[srcp]
charp:
	.equ	$+1
	ldx	0x0000
	staa	[x]
	inx
	stx	[charp]
attrp:
	.equ	$+1
	ldx	0x0000
	stab	[x]
	inx
	stx	[attrp]
	dec	[cntx]
	bne	loopx
	ldx	[srcp]
	addx.mb	(vram.margin@r + vram.margin@l) * vram.bytes_per_char
	stx	[srcp]
	dec	[cnty]
	bne	loopy
done:
	rts

	.wseg
cntx:
	.ds	1
cnty:
	.ds	1
	.end

;;;-----------------------------------------------------------------------------
;;; vram.scrollup
;;;  Format: vram.scrollup top=0,bottom=23
;;;-----------------------------------------------------------------------------
vram.scrollup:
	.macro	top=0,bottom=23
	vram.fromxy 0,bottom
	stx	[endaddr1]
	addx.mb	vram.width * vram.bytes_per_char
	stx	[endaddr2]
	vram.fromxy 0,top
loop1:
	ldaa	[x + vram.width * vram.bytes_per_char]
	staa	[x]
	inx
endaddr1:
	.equ	$+1
	cpx	0x0000
	bne	loop1
	clra
loop2:
	staa	[x]
	inx
endaddr2:
	.equ	$+1
	cpx	0x0000
	bne	loop2
	.end

;;;-----------------------------------------------------------------------------
;;; vram.scrolldown
;;;  Format: vram.scrolldown top=0,bottom=23
;;;-----------------------------------------------------------------------------
vram.scrolldown:
	.macro	top=0,bottom=23
	vram.fromxy 0,top
	dex
	stx	[endaddr1]
	subx.mb	vram.width * vram.bytes_per_char
	stx	[endaddr2]
	vram.fromxy 0,bottom
	dex
loop1:
	ldaa	[x]
	staa	[x + vram.width * vram.bytes_per_char]
	dex
endaddr1:
	.equ	$+1
	cpx	0x0000
	bne	loop1
	clra
loop2:
	staa	[x + vram.width * vram.bytes_per_char]
	dex
endaddr2:
	.equ	$+1
	cpx	0x0000
	bne	loop2
	.end

;;;-----------------------------------------------------------------------------
;;; VRAM Buffer
;;;-----------------------------------------------------------------------------
	.wseg
vram:
	.ds	vram.width * vram.height * vram.bytes_per_char
vram.end:
