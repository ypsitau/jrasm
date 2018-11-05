;;;=============================================================================
;;; addsub.inc
;;; Macros to extend add/sub operations.
;;;=============================================================================
	.cseg

;;;-----------------------------------------------------------------------------
;;; ldmb.a
;;; memb <- A
;;;  Format: ldmd.a memb
;;;  Param:  memb [DIR,IDX,EXT] .. Byte-sized memory to write to
;;;-----------------------------------------------------------------------------
ldmb.a:
	.macro	memb
	staa	memb
	.end

;;;-----------------------------------------------------------------------------
;;; ldmb.b
;;; memb <- B
;;;  Format: memb
;;;  Param:  memb [DIR,IDX,EXT] .. Byte-sized memory to write to
;;;-----------------------------------------------------------------------------
ldmb.b:
	.macro	memb
	stab	memb
	.end

;;;-----------------------------------------------------------------------------
;;; ldmb
;;; memb <- numb
;;;  Format: ldmb memb,numb
;;;  Param:  memb [DIR,IDX,EXT] .. Byte-sized memory to write to
;;;          numb [IMM,DIR,IDX,EXT] .. Byte-sized number to be loaded
;;;-----------------------------------------------------------------------------
ldmb:
	.macro	memb,numb
	ldaa	numb
	ldmb.a	memb
	.end

;;;-----------------------------------------------------------------------------
;;; ldmw.x
;;; memw <- X
;;;  Format: ldmw.x memw
;;;  Param:  memw [DIR,IDX,EXT] .. Word-sized memory to write to
;;;-----------------------------------------------------------------------------
ldmw.x:
	.macro	memw
	stx	memw
	.end
;;;-----------------------------------------------------------------------------
;;; ldmw
;;; memw <- numw
;;;  Format: ldmw memw,numw
;;;  Param:  memw [DIR,IDX,EXT] .. Word-sized memory to write to
;;;          numb [IMM,DIR,IDX,EXT] .. Byte-sized number to be loaded
;;;-----------------------------------------------------------------------------
ldmw:
	.macro	memw,numw
	ldx	numw
	ldmw.x	memw
	.end

;;;-----------------------------------------------------------------------------
;;; addmb.a
;;; memb <- memb + A
;;;  Format: addmb.a memb
;;;  Param:  memb [DIR,IDX,EXT] .. Byte-sized memory to read from and write to
;;;-----------------------------------------------------------------------------
addmb.a:
	.macro	memb
	adda	memb
	staa	memb
	.end

;;;-----------------------------------------------------------------------------
;;; addmb.mb
;;; memb <- memb + numb
;;;  Format: addmb.mb memb,numb
;;;  Param:  memb [DIR,IDX,EXT] .. Byte-sized memory to read from and write to
;;;          numb [IMM,DIR,IDX,EXT] .. Byte-sized number to be added
;;;-----------------------------------------------------------------------------
addmb.mb:
	.macro	memb,numb
	ldaa	numb
	addmb.a	memb
	.end

;;;-----------------------------------------------------------------------------
;;; addmb
;;; memb <- memb + numb
;;;  Format: addmb memb,numb
;;;  Param:  memb [DIR,IDX,EXT] .. Byte-sized memory to read from and write to
;;;          numb [IMM,DIR,IDX,EXT] .. Byte-sized number to be added
;;;-----------------------------------------------------------------------------
addmb:
	.macro	memb,numb
	addmb.mb memb,numb
	.end

;;;-----------------------------------------------------------------------------
;;; addmw.a
;;; memw <- memw + A
;;;  Format: addmw.a memw
;;;  Param:  memw [DIR,IDX,EXT] .. Word-sized memory to read from and write to
;;;-----------------------------------------------------------------------------
addmw.a:
	.macro	memw
	adda	memw <+> [2,1]
	staa	memw <+> [2,1]
	ldaa	0
	adca	memw <+> [2,0]
	staa	memw <+> [2,0]
	.end

;;;-----------------------------------------------------------------------------
;;; addmw.mb
;;; memw <- memw + numb
;;;  Format: addmw.mb memw,numb
;;;  Param:  memw [DIR,IDX,EXT] .. Word-sized memory to read from and write to
;;;          numb [IMM,DIR,IDX,EXT] .. Byte-sized number to be added
;;;-----------------------------------------------------------------------------
addmw.mb:
	.macro	memw,numb
	ldaa	numb
	addmw.a	memw
	.end

;;;-----------------------------------------------------------------------------
;;; addmw.mw
;;; memw <- memw + numw
;;;  Format: addmw.mw memw,numw
;;;  Param:  memw [DIR,IDX,EXT] .. Word-sized memory to read from and write to
;;;          numw [IMM,DIR,IDX,EXT] .. Word-sized number to be added
;;;-----------------------------------------------------------------------------
addmw.mw:
	.macro	memw,numw
	ldaa	numw <+> [2,1]
	adda	memw <+> [2,1]
	staa	memw <+> [2,1]
	ldaa	numw <+> [2,0]
	adca	memw <+> [2,0]
	staa	memw <+> [2,0]
	.end

;;;-----------------------------------------------------------------------------
;;; addx.a
;;; X <- X + A
;;;  Format: addx.a
;;;-----------------------------------------------------------------------------
addx.a:
	.macro
	stx	[result]
	addmw.a [result]
result:
	.equ	$+1
	ldx	0x0000
	.end

;;;-----------------------------------------------------------------------------
;;; addx.mb
;;; X <- X + numb
;;;  Format: addx.mb numb
;;;  Param:  numb [IMM,DIR,IDX,EXT] .. Byte-sized number to be added
;;;-----------------------------------------------------------------------------
addx.mb:
	.macro	numb
	stx	[result]
	addmw.mb [result],numb
result:
	.equ	$+1
	ldx	0x0000
	.end

;;;-----------------------------------------------------------------------------
;;; addx.mw
;;; X <- X + numw
;;;  Format: addx.mw numw
;;;  Param:  numw [IMM,DIR,IDX,EXT] .. Word-sized number to be added
;;;-----------------------------------------------------------------------------
addx.mw:
	.macro	numw
	stx	[result]
	addmw.mw [result],numw
result:
	.equ	$+1
	ldx	0x0000
	.end

;;;-----------------------------------------------------------------------------
;;; submb.a
;;; memb <- memb - A
;;;  Format: submb.a memb
;;;  Param:  memb [DIR,IDX,EXT] .. Byte-sized memory to read from and write to
;;;-----------------------------------------------------------------------------
submb.a:
	.macro	memb
	suba	memb
	nega
	staa	memb
	.end

;;;-----------------------------------------------------------------------------
;;; submb.mb
;;; memb <- memb - numb
;;;  Format: submb.mb memb,numb
;;;  Param:  memb [DIR,IDX,EXT] .. Byte-sized memory to read from and write to
;;;          numb [IMM,DIR,IDX,EXT] .. Byte-sized number to be subtracted
;;;-----------------------------------------------------------------------------
submb.mb:
	.macro	memb,numb
	ldaa	memb
	suba	numb
	staa	memb
	.end

;;;-----------------------------------------------------------------------------
;;; submb
;;; memb <- memb - numb
;;;  Format: submb memb,numb
;;;  Param:  memb [DIR,IDX,EXT] .. Byte-sized memory to read from and write to
;;;          numb [IMM,DIR,IDX,EXT] .. Byte-sized number to be subtracted
;;;-----------------------------------------------------------------------------
submb:
	.macro	memb,numb
	submb.mb memb,numb
	.end

;;;-----------------------------------------------------------------------------
;;; submw.a
;;; memw <- memw - A
;;;  Format: submw.a memw
;;;  Param:  memw [DIR,IDX,EXT] .. Word-sized memory to read from and write to
;;;-----------------------------------------------------------------------------
submw.a:
	.macro	memw
	ldab	memw <+> [2,1]
	staa	memw <+> [2,1]
	subb	memw <+> [2,1]
	stab	memw <+> [2,1]
	ldaa	memw <+> [2,0]
	sbca	0
	staa	memw <+> [2,0]
	.end

;;;-----------------------------------------------------------------------------
;;; submw.mb
;;; memw <- memw - numb
;;;  Format: submw.mb memw,numb
;;;  Param:  memw [DIR,IDX,EXT] .. Word-sized memory to read from and write to
;;;          numb [IMM,DIR,IDX,EXT] .. Byte-sized number to be subtracted
;;;-----------------------------------------------------------------------------
submw.mb:
	.macro	memw,numb
	ldaa	memw <+> [2,1]
	suba	numb
	staa	memw <+> [2,1]
	ldaa	memw <+> [2,0]
	sbca	0
	staa	memw <+> [2,0]
	.end

;;;-----------------------------------------------------------------------------
;;; submw.mw
;;; memw <- memw - numw
;;;  Format: submw.mw memw,numw
;;;  Param:  memw [DIR,IDX,EXT] .. Word-sized memory to read from and write to
;;;          numw [IMM,DIR,IDX,EXT] .. Word-sized number to be subtracted
;;;-----------------------------------------------------------------------------
submw.mw:
	.macro	memw,numw
	ldaa	memw <+> [2,1]
	suba	numw <+> [2,1]
	staa	memw <+> [2,1]
	ldaa	memw <+> [2,0]
	sbca	numw <+> [2,0]
	staa	memw <+> [2,0]
	.end

;;;-----------------------------------------------------------------------------
;;; subx.a
;;; X <- X - A
;;;  Format: subx.a
;;;-----------------------------------------------------------------------------
subx.a:
	.macro
	stx	[result]
	submw.a [result]
result:
	.equ	$+1
	ldx	0x0000
	.end

;;;-----------------------------------------------------------------------------
;;; subx.mb
;;; X <- X - numb
;;;  Format: subx.mb numb
;;;  Param:  numb [IMM,DIR,IDX,EXT] .. Byte-sized number to be added
;;;-----------------------------------------------------------------------------
subx.mb:
	.macro	numb
	stx	[result]
	submw.mb [result],numb
result:
	.equ	$+1
	ldx	0x0000
	.end

;;;-----------------------------------------------------------------------------
;;; subx.mw
;;; X <- X - numw
;;;  Format: subx.mw numw
;;;  Param:  numw [IMM,DIR,IDX,EXT] .. Byte-sized number to be added
;;;-----------------------------------------------------------------------------
subx.mw:
	.macro	numw
	stx	[result]
	submw.mw [result],numw
result:
	.equ	$+1
	ldx	0x0000
	.end
