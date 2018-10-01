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
	.dw		addr1
	.dw		addr2
addr1:
	.dw		addr1
addr2:
	.dw		addr2
	.end

macro5:
	.macro	arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14,arg15,arg16
	.dw		arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,arg13,arg14,arg15,arg16
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
