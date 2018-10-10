	.org	0x1000

external_val:	.equ	0x20

macro0:
	.macro
	.end

macro1:
	.macro	arg
	ldaa	arg
	ldaa	{arg}
	ldaa	[x+arg]
	ldaa	[arg]
	ldaa	arg+external_val
	ldaa	{arg+external_val}
	ldaa	[x+arg+external_val]
	ldaa	[arg+external_val]
	.end

macro2:
	.macro	arg1,arg2
	ldaa	arg1+arg2
	ldaa	{arg1+arg2}
	ldaa	[x+arg1+arg2]
	ldaa	[arg1+arg2]
	ldaa	arg1+arg2+external_val
	ldaa	{arg1+arg2+external_val}
	ldaa	[x+arg1+arg2+external_val]
	ldaa	[arg1+arg2+external_val]
	.end

macro3:
	.macro	arg
	ldaa	arg
	.end

macro4:
	.macro
	.dw	addr1
	.dw	addr2
addr1:
	.dw	addr1
addr2:
	.dw	addr2
	.end

macro5:
	.macro	arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14,arg15,arg16
	.dw	arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14,arg15,arg16
	.end

	macro0
	macro0
	macro1	8
	macro2	3, 4
	macro3	0x22
	macro3	{0x22}
	macro3	[x+0x22]
	macro3	[0x1234]
	macro4
	macro4
	macro4
	macro5	0x0000,0x1111,0x2222,0x3333,0x4444,0x5555,0x6666,0x7777,0x8888,0x9999,0xaaaa,0xbbbb,0xcccc,0xdddd,0xeeee,0xffff
macro6:	.macro
	.end

macro7:	.macro
	macro6
	.end

macro8:	.macro
	macro7
	.end

macro9:	.macro
	macro8
	.end

macro10:.macro
	macro9
	.end

	macro10

macro11:
	.macro arg1=0x11,arg2=0x22,arg3=0x33,arg4=0x44
	.db	arg1, arg2, arg3, arg4
	.end

	macro11
	macro11 0xaa
	macro11 0xaa, 0xbb
	macro11 0xaa, 0xbb, 0xcc
	macro11 0xaa, 0xbb, 0xcc, 0xdd
	macro11 , 0xbb, 0xcc, 0xdd
	macro11 0xaa, , 0xcc, 0xdd
	macro11 0xaa, 0xbb, , 0xdd
	macro11 0xaa, 0xbb, 0xcc,
	macro11 , , 0xcc, 0xdd
	macro11 0xaa, , , 0xdd
	macro11 0xaa, 0xbb, ,
	macro11 , , , 0xdd
	macro11 , , ,

addmb_acc:
	.macro	mem,acc
	add	acc,mem
	sta	acc,mem
	.end

	addmb_acc {0x12},a
	addmb_acc [x+0x12],a
	addmb_acc [0x1234],a

	addmb_acc {0x12},b
	addmb_acc [x+0x12],b
	addmb_acc [0x1234],b

addmb:
	.macro	mem,numr,accwk=a
	lda	accwk,numr
	addmb_acc mem,accwk
	.end

	addmb	{0x12},0x56
	addmb	{0x12},[0x5678]
	addmb	[x+0x12],[0x5678]
	addmb	[0x1234],[0x5678]

	addmb	{0x12},0x56,b
	addmb	{0x12},[0x5678],b
	addmb	[x+0x12],[0x5678],b
	addmb	[0x1234],[0x5678],b

addmw_acc:
	.macro	mem,acc
	add	acc,mem <+> 1
	sta	acc,mem <+> 1	; not change C
	lda	acc,mem		; not change C
	adc	acc,0
	sta	acc,mem
	.end

	addmw_acc {0x12},a
	addmw_acc [x+0x12],a
	addmw_acc [0x1234],a

	addmw_acc {0x12},b
	addmw_acc [x+0x12],b
	addmw_acc [0x1234],b

addmw:
	.macro	mem,numr,accwk=a
	lda	accwk,numr
	addmw_acc mem,accwk
	.end

	addmw	{0x12},0x56
	addmw	{0x12},[0x5678]
	addmw	[x+0x12],[0x5678]
	addmw	[0x1234],[0x5678]

	addmw	{0x12},0x56,b
	addmw	{0x12},[0x5678],b
	addmw	[x+0x12],[0x5678],b
	addmw	[0x1234],[0x5678],b

addmw_mw:
	.macro	mem,memr,accwk=a
	lda	accwk,memr <+> 1
	add	accwk,mem <+> 1
	sta	accwk,mem <+> 1	; not change C
	lda	accwk,mem	; not change C
	adc	accwk,memr
	sta	accwk,mem
	.end

	addmw_mw {0x12},[0x5678]
	addmw_mw [x+0x12],[0x5678]
	addmw_mw [0x1234],[0x5678]

	addmw_mw {0x12},[0x5678],b
	addmw_mw [x+0x12],[0x5678],b
	addmw_mw [0x1234],[0x5678],b
