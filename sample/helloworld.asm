	.ORG	0x2000
loop:
	LDX		[ptr_src]
	LDAA	[x]
	INX
	STX		[ptr_src]
	CMPA	0x00
	BEQ		done
	LDX		[ptr_dst]
	STAA	[x]
	INX
	STX		[ptr_dst]
	BRA		loop
done:
	RTS
ptr_src:
	.DW		hello_world
ptr_dst:
	.DW		0xc100
hello_world:
	.DB		"Hello, world!", 0
