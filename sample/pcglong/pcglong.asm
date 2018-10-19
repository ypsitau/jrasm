	.org	0x1000
	
	clra
	staa	{0x00}		; disable click sound
	jsr	0xec7f		; clear screen with attribute data in {0x0e}
	
	pcgpage.page1.store
	
	ldx	0xc100
	pcg.pattern1x8.put
	ldx	0xc500
	pcg.pattern1x8.putattr
	
	ldx	0xc102
	pcg.pattern1x9.put
	ldx	0xc502
	pcg.pattern1x9.putattr
	
	ldx	0xc104
	pcg.pattern1x16.put
	ldx	0xc504
	pcg.pattern1x16.putattr
	
	ldx	0xc106
	pcg.pattern1x17.put
	ldx	0xc506
	pcg.pattern1x17.putattr
	
	ldx	0xc108
	pcg.pattern1x24.put
	ldx	0xc508
	pcg.pattern1x24.putattr
	
infinite:
	bra infinite
	
	.pcgpage page1,user:0x40
	
	.pcg	pattern1x8,1,8
	.db	0xff,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xff
	.end
	
	.pcg	pattern1x9,1,9
	.db	0xff,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xff
	.end
	
	.pcg	pattern1x16,1,16
	.db	0xff,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xff
	.end
	
	.pcg	pattern1x17,1,17
	.db	0xff,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xff
	.end
	
	.pcg	pattern1x24,1,24
	.db	0xff,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81
	.db	0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xff
	.end
	
	.end
	