;;;=============================================================================
;;; oputil.inc
;;;=============================================================================
	.cseg
	
ldmb.a:
	.macro	memb
	staa	memb
	.end

ldmb.b:
	.macro	memb
	stab	memb
	.end

ldmb:
	.macro	memb,numb
	ldaa	numb
	ldmb.a	memb
	.end

ldmw.x:
	.macro	memw
	stx	memw
	.end
ldmw:
	.macro	memw,numw
	ldx	numw
	ldmw.x	memw
	.end

addmb.a:
	.macro	memb
	adda	memb
	staa	memb
	.end

addmb.mb:
	.macro	memb,numb
	ldaa	numb
	addmb.a	memb
	.end
addmb:
	.macro	memb,numb
	addmb.mb memb,numb
	.end

addmw.a:
	.macro	memw
	adda	memw <+> [2,1]
	staa	memw <+> [2,1]
	clra
	adca	memw <+> [2,0]
	staa	memw <+> [2,0]
	.end

addmw.mb:
	.macro	memw,numb
	ldaa	numb
	addmw.a	memw
	.end

addmw.mw:
	.macro	memw,numw
	ldaa	numw <+> [2,1]
	adda	memw <+> [2,1]
	staa	memw <+> [2,1]
	ldaa	numw <+> [2,0]
	adca	memw <+> [2,0]
	staa	memw <+> [2,0]
	.end

addx.a:
	.macro
	stx	[result]
	adda	[result.l]	
	staa	[result.l]
	clra
	adca	[result.h]
	staa	[result.h]
result:
	.equ	$+1
result.h:
	.equ	$+1
result.l:
	.equ	$+2
	ldx	0x0000
	.end

addx.mb:
	.macro	numb
	ldaa	numb
	addx.a
	.end

addx.mw:
	.macro	numw
	stx	[result]
	ldaa	numw <+> [2,1]
	adda	[result.l]
	staa	[result.l]
	ldaa	numw <+> [2,0]
	adca	[result.h]
	staa	[result.h]
result:
	.equ	$+1
result.h:
	.equ	$+1
result.l:
	.equ	$+2
	ldx	0x0000
	.end
