	.org	0x1000

	pcgpage.page1.store
	
	pcg.pcg1.put
	pcg.pcg1.erase
	pcg.pcg1.fill 0xaa
	pcg.pcg1.putattr
	pcg.pcg1.putattrfg
	pcg.pcg1.eraseattr
	
	.pcgpage page1,user:0x20:0x21,cram:0x80:0x81,user:0x3e
	.pcg	pcg1,3,3
	.db	0xff,0xff,0xff
	.db	0x01,0x02,0x03
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0x04,0x05,0x06
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0x07,0x08,0x09
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.db	0xff,0xff,0xff
	.end
	
	.end
