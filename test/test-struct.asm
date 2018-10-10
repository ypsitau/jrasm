	.org	0x1000

struct1:
	.struct
posx:	.ds	@byte
posy:	.ds	@byte
score:	.ds	@word
attr:	.ds	@byte
	.end
	
struct2:
	.struct
attr1:	.ds	@byte
attr2:	.ds	@byte * 8
attr3:	.ds	@word
attr4:	.ds	@byte * 2
attr5:	.ds	@word * 4
attr6:	.ds	@byte
attr7:	.ds	@byte
	.end
	
	.db	@struct1
	.db	struct1.posx
	.db	struct1.posy
	.db	struct1.score
	.db	struct1.attr
	.ds	@struct1 * 8

	.db	@struct2
	.db	struct2.attr1
	.db	struct2.attr2
	.db	struct2.attr3
	.db	struct2.attr4
	.db	struct2.attr5
	.db	struct2.attr6
	.db	struct2.attr7
