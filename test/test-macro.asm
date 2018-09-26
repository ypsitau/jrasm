	.org	0x1000
	
external_val:	.equ	0x20
	
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
	.endm

macro2:
	.macro	arg1, arg2
	ldaa	arg1+arg2
	ldaa	{arg1+arg2}
	ldaa	[x+arg1+arg2]
	ldaa	[arg1+arg2]
	ldaa	arg1+arg2+external_val
	ldaa	{arg1+arg2+external_val}
	ldaa	[x+arg1+arg2+external_val]
	ldaa	[arg1+arg2+external_val]
	.endm

macro3:	
	.macro	arg
	ldaa	arg
	.endm

macro4:
	.macro
	.dw		addr1
	.dw		addr2
addr1:
	.dw		addr1
addr2:
	.dw		addr2
	.endm
	
	macro1	8
	macro2	3, 4
	macro3	0x22
	macro3	{0x22}
	macro3	[x+0x22]
	macro3	[0x1234]
	macro4
	macro4
	macro4
