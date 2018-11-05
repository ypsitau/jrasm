;;;=============================================================================
;;; jbranch.inc
;;;=============================================================================
	.cseg
;;;-----------------------------------------------------------------------------
;;; Format: jXX addr16
;;; Param: addr16 [EXT] .. Address
;;;-----------------------------------------------------------------------------
jcc:	.macro addr16
	bcs	skip
	jmp	addr16
skip:	.end

jcs:	.macro addr16
	bcc	skip
	jmp	addr16
skip:	.end

jeq:	.macro addr16
	bne	skip
	jmp	addr16
skip:	.end

jge:	.macro addr16
	blt	skip
	jmp	addr16
skip:	.end

jgt:	.macro addr16
	ble	skip
	jmp	addr16
skip:	.end

jhi:	.macro addr16
	bls	skip
	jmp	addr16
skip:	.end

jle:	.macro addr16
	bgt	skip
	jmp	addr16
skip:	.end

jls:	.macro addr16
	bhi	skip
	jmp	addr16
skip:	.end

jlt:	.macro addr16
	bge	skip
	jmp	addr16
skip:	.end

jmi:	.macro addr16
	bpl	skip
	jmp	addr16
skip:	.end

jne:	.macro addr16
	beq	skip
	jmp	addr16
skip:	.end

jpl:	.macro addr16
	bmi	skip
	jmp	addr16
skip:	.end

jvc:	.macro addr16
	bvs	skip
	jmp	addr16
skip:	.end

jvs:	.macro addr16
	bvc	skip
	jmp	addr16
skip:	.end
