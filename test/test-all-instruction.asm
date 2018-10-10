	.org	0x1000
start:
	aba
after_aba:
	adc	a,0x12
after_adc_a_imm:
	adc	a,{0x12}
after_adc_a_dir:
	adc	a,[X+0x12]
after_adc_a_idx:
	adc	a,[0x1234]
after_adc_a_ext:
	adc	b,0x12
after_adc_b_imm:
	adc	b,{0x12}
after_adc_b_dir:
	adc	b,[X+0x12]
after_adc_b_idx:
	adc	b,[0x1234]
after_adc_b_ext:
	adca	0x12
after_adca_imm:
	adca	{0x12}
after_adca_dir:
	adca	[X+0x12]
after_adca_idx:
	adca	[0x1234]
after_adca_ext:
	adcb	0x12
after_adcb_imm:
	adcb	{0x12}
after_adcb_dir:
	adcb	[X+0x12]
after_adcb_idx:
	adcb	[0x1234]
after_adcb_ext:
	add	a,0x12
after_add_a_imm:
	add	a,{0x12}
after_add_a_dir:
	add	a,[X+0x12]
after_add_a_idx:
	add	a,[0x1234]
after_add_a_ext:
	add	b,0x12
after_add_b_imm:
	add	b,{0x12}
after_add_b_dir:
	add	b,[X+0x12]
after_add_b_idx:
	add	b,[0x1234]
after_add_b_ext:
	adda	0x12
after_adda_imm:
	adda	{0x12}
after_adda_dir:
	adda	[X+0x12]
after_adda_idx:
	adda	[0x1234]
after_adda_ext:
	addb	0x12
after_addb_imm:
	addb	{0x12}
after_addb_dir:
	addb	[X+0x12]
after_addb_idx:
	addb	[0x1234]
after_addb_ext:
	and	a,0x12
after_and_a_imm:
	and	a,{0x12}
after_and_a_dir:
	and	a,[X+0x12]
after_and_a_idx:
	and	a,[0x1234]
after_and_a_ext:
	and	b,0x12
after_and_b_imm:
	and	b,{0x12}
after_and_b_dir:
	and	b,[X+0x12]
after_and_b_idx:
	and	b,[0x1234]
after_and_b_ext:
	anda	0x12
after_anda_imm:
	anda	{0x12}
after_anda_dir:
	anda	[X+0x12]
after_anda_idx:
	anda	[0x1234]
after_anda_ext:
	andb	0x12
after_andb_imm:
	andb	{0x12}
after_andb_dir:
	andb	[X+0x12]
after_andb_idx:
	andb	[0x1234]
after_andb_ext:
	asl	a
after_asl_a:
	asl	b
after_asl_b:
	asl	[X+0x12]
after_asl_idx:
	asl	[0x1234]
after_asl_ext:
	asla
after_asla:
	aslb
after_aslb:
	asr	a
after_asr_a:
	asr	b
after_asr_b:
	asr	[X+0x12]
after_asr_idx:
	asr	[0x1234]
after_asr_ext:
	asra
after_asra:
	asrb
after_asrb:
	bcc	0x12
after_bcc_imm:
	bcs	0x12
after_bcs_imm:
	beq	0x12
after_beq_imm:
	bge	0x12
after_bge_imm:
	bgt	0x12
after_bgt_imm:
	bhi	0x12
after_bhi_imm:
	bit	a,0x12
after_bit_a_imm:
	bit	a,{0x12}
after_bit_a_dir:
	bit	a,[X+0x12]
after_bit_a_idx:
	bit	a,[0x1234]
after_bit_a_ext:
	bit	b,0x12
after_bit_b_imm:
	bit	b,{0x12}
after_bit_b_dir:
	bit	b,[X+0x12]
after_bit_b_idx:
	bit	b,[0x1234]
after_bit_b_ext:
	bita	0x12
after_bita_imm:
	bita	{0x12}
after_bita_dir:
	bita	[X+0x12]
after_bita_idx:
	bita	[0x1234]
after_bita_ext:
	bitb	0x12
after_bitb_imm:
	bitb	{0x12}
after_bitb_dir:
	bitb	[X+0x12]
after_bitb_idx:
	bitb	[0x1234]
after_bitb_ext:
	ble	0x12
after_ble_imm:
	bls	0x12
after_bls_imm:
	blt	0x12
after_blt_imm:
	bmi	0x12
after_bmi_imm:
	bne	0x12
after_bne_imm:
	bpl	0x12
after_bpl_imm:
	bra	0x12
after_bra_imm:
	bsr	0x12
after_bsr_imm:
	bvc	0x12
after_bvc_imm:
	bvs	0x12
after_bvs_imm:
	cba
after_cba:
	clc
after_clc:
	cli
after_cli:
	clr	a
after_clr_a:
	clr	b
after_clr_b:
	clr	[X+0x12]
after_clr_idx:
	clr	[0x1234]
after_clr_ext:
	clra
after_clra:
	clrb
after_clrb:
	clv
after_clv:
	cmp	a,0x12
after_cmp_a_imm:
	cmp	a,{0x12}
after_cmp_a_dir:
	cmp	a,[X+0x12]
after_cmp_a_idx:
	cmp	a,[0x1234]
after_cmp_a_ext:
	cmp	b,0x12
after_cmp_b_imm:
	cmp	b,{0x12}
after_cmp_b_dir:
	cmp	b,[X+0x12]
after_cmp_b_idx:
	cmp	b,[0x1234]
after_cmp_b_ext:
	cmpa	0x12
after_cmpa_imm:
	cmpa	{0x12}
after_cmpa_dir:
	cmpa	[X+0x12]
after_cmpa_idx:
	cmpa	[0x1234]
after_cmpa_ext:
	cmpb	0x12
after_cmpb_imm:
	cmpb	{0x12}
after_cmpb_dir:
	cmpb	[X+0x12]
after_cmpb_idx:
	cmpb	[0x1234]
after_cmpb_ext:
	com	a
after_com_a:
	com	b
after_com_b:
	com	[X+0x12]
after_com_idx:
	com	[0x1234]
after_com_ext:
	coma
after_coma:
	comb
after_comb:
	cpx	{0x12}
after_cpx_dir:
	cpx	[X+0x12]
after_cpx_idx:
	cpx	0x1234
after_cpx_imm:
	cpx	[0x1234]
after_cpx_ext:
	daa
after_daa:
	dec	a
after_dec_a:
	dec	b
after_dec_b:
	dec	[X+0x12]
after_dec_idx:
	dec	[0x1234]
after_dec_ext:
	deca
after_deca:
	decb
after_decb:
	des
after_des:
	dex
after_dex:
	eor	a,0x12
after_eor_a_imm:
	eor	a,{0x12}
after_eor_a_dir:
	eor	a,[X+0x12]
after_eor_a_idx:
	eor	a,[0x1234]
after_eor_a_ext:
	eor	b,0x12
after_eor_b_imm:
	eor	b,{0x12}
after_eor_b_dir:
	eor	b,[X+0x12]
after_eor_b_idx:
	eor	b,[0x1234]
after_eor_b_ext:
	eora	0x12
after_eora_imm:
	eora	{0x12}
after_eora_dir:
	eora	[X+0x12]
after_eora_idx:
	eora	[0x1234]
after_eora_ext:
	eorb	0x12
after_eorb_imm:
	eorb	{0x12}
after_eorb_dir:
	eorb	[X+0x12]
after_eorb_idx:
	eorb	[0x1234]
after_eorb_ext:
	inc	a
after_inc_a:
	inc	b
after_inc_b:
	inc	[X+0x12]
after_inc_idx:
	inc	[0x1234]
after_inc_ext:
	inca
after_inca:
	incb
after_incb:
	ins
after_ins:
	inx
after_inx:
	jmp	X+0x12
after_jmp_idx:
	jmp	0x1234
after_jmp_ext:
	jsr	X+0x12
after_jsr_idx:
	jsr	0x1234
after_jsr_ext:
	lda	a,0x12
after_lda_a_imm:
	lda	a,{0x12}
after_lda_a_dir:
	lda	a,[X+0x12]
after_lda_a_idx:
	lda	a,[0x1234]
after_lda_a_ext:
	lda	b,0x12
after_lda_b_imm:
	lda	b,{0x12}
after_lda_b_dir:
	lda	b,[X+0x12]
after_lda_b_idx:
	lda	b,[0x1234]
after_lda_b_ext:
	ldaa	0x12
after_ldaa_imm:
	ldaa	{0x12}
after_ldaa_dir:
	ldaa	[X+0x12]
after_ldaa_idx:
	ldaa	[0x1234]
after_ldaa_ext:
	ldab	0x12
after_ldab_imm:
	ldab	{0x12}
after_ldab_dir:
	ldab	[X+0x12]
after_ldab_idx:
	ldab	[0x1234]
after_ldab_ext:
	lds	{0x12}
after_lds_dir:
	lds	[X+0x12]
after_lds_idx:
	lds	0x1234
after_lds_imm:
	lds	[0x1234]
after_lds_ext:
	ldx	{0x12}
after_ldx_dir:
	ldx	[X+0x12]
after_ldx_idx:
	ldx	0x1234
after_ldx_imm:
	ldx	[0x1234]
after_ldx_ext:
	lsr	a
after_lsr_a:
	lsr	b
after_lsr_b:
	lsr	[X+0x12]
after_lsr_idx:
	lsr	[0x1234]
after_lsr_ext:
	lsra
after_lsra:
	lsrb
after_lsrb:
	neg	a
after_neg_a:
	neg	b
after_neg_b:
	neg	[X+0x12]
after_neg_idx:
	neg	[0x1234]
after_neg_ext:
	nega
after_nega:
	negb
after_negb:
	nop
after_nop:
	ora	a,0x12
after_ora_a_imm:
	ora	a,{0x12}
after_ora_a_dir:
	ora	a,[X+0x12]
after_ora_a_idx:
	ora	a,[0x1234]
after_ora_a_ext:
	ora	b,0x12
after_ora_b_imm:
	ora	b,{0x12}
after_ora_b_dir:
	ora	b,[X+0x12]
after_ora_b_idx:
	ora	b,[0x1234]
after_ora_b_ext:
	oraa	0x12
after_oraa_imm:
	oraa	{0x12}
after_oraa_dir:
	oraa	[X+0x12]
after_oraa_idx:
	oraa	[0x1234]
after_oraa_ext:
	orab	0x12
after_orab_imm:
	orab	{0x12}
after_orab_dir:
	orab	[X+0x12]
after_orab_idx:
	orab	[0x1234]
after_orab_ext:
	psh	a
after_psh_a:
	psh	b
after_psh_b:
	psha
after_psha:
	pshb
after_pshb:
	pul	a
after_pul_a:
	pul	b
after_pul_b:
	pula
after_pula:
	pulb
after_pulb:
	rol	a
after_rol_a:
	rol	b
after_rol_b:
	rol	[X+0x12]
after_rol_idx:
	rol	[0x1234]
after_rol_ext:
	rola
after_rola:
	rolb
after_rolb:
	ror	a
after_ror_a:
	ror	b
after_ror_b:
	ror	[X+0x12]
after_ror_idx:
	ror	[0x1234]
after_ror_ext:
	rora
after_rora:
	rorb
after_rorb:
	rti
after_rti:
	rts
after_rts:
	sba
after_sba:
	sbc	a,0x12
after_sbc_a_imm:
	sbc	a,{0x12}
after_sbc_a_dir:
	sbc	a,[X+0x12]
after_sbc_a_idx:
	sbc	a,[0x1234]
after_sbc_a_ext:
	sbc	b,0x12
after_sbc_b_imm:
	sbc	b,{0x12}
after_sbc_b_dir:
	sbc	b,[X+0x12]
after_sbc_b_idx:
	sbc	b,[0x1234]
after_sbc_b_ext:
	sbca	0x12
after_sbca_imm:
	sbca	{0x12}
after_sbca_dir:
	sbca	[X+0x12]
after_sbca_idx:
	sbca	[0x1234]
after_sbca_ext:
	sbcb	0x12
after_sbcb_imm:
	sbcb	{0x12}
after_sbcb_dir:
	sbcb	[X+0x12]
after_sbcb_idx:
	sbcb	[0x1234]
after_sbcb_ext:
	sec
after_sec:
	sei
after_sei:
	sev
after_sev:
	sta	a,{0x12}
after_sta_a_dir:
	sta	a,[X+0x12]
after_sta_a_idx:
	sta	a,[0x1234]
after_sta_a_ext:
	sta	b,{0x12}
after_sta_b_dir:
	sta	b,[X+0x12]
after_sta_b_idx:
	sta	b,[0x1234]
after_sta_b_ext:
	staa	{0x12}
after_staa_dir:
	staa	[X+0x12]
after_staa_idx:
	staa	[0x1234]
after_staa_ext:
	stab	{0x12}
after_stab_dir:
	stab	[X+0x12]
after_stab_idx:
	stab	[0x1234]
after_stab_ext:
	sts	{0x12}
after_sts_dir:
	sts	[X+0x12]
after_sts_idx:
	sts	[0x1234]
after_sts_ext:
	stx	{0x12}
after_stx_dir:
	stx	[X+0x12]
after_stx_idx:
	stx	[0x1234]
after_stx_ext:
	sub	a,0x12
after_sub_a_imm:
	sub	a,{0x12}
after_sub_a_dir:
	sub	a,[X+0x12]
after_sub_a_idx:
	sub	a,[0x1234]
after_sub_a_ext:
	sub	b,0x12
after_sub_b_imm:
	sub	b,{0x12}
after_sub_b_dir:
	sub	b,[X+0x12]
after_sub_b_idx:
	sub	b,[0x1234]
after_sub_b_ext:
	suba	0x12
after_suba_imm:
	suba	{0x12}
after_suba_dir:
	suba	[X+0x12]
after_suba_idx:
	suba	[0x1234]
after_suba_ext:
	subb	0x12
after_subb_imm:
	subb	{0x12}
after_subb_dir:
	subb	[X+0x12]
after_subb_idx:
	subb	[0x1234]
after_subb_ext:
	swi
after_swi:
	tab
after_tab:
	tap
after_tap:
	tpa
after_tpa:
	tst	a
after_tst_a:
	tst	b
after_tst_b:
	tst	[X+0x12]
after_tst_idx:
	tst	[0x1234]
after_tst_ext:
	tsta
after_tsta:
	tstb
after_tstb:
	tsx
after_tsx:
	txs
after_txs:
	wai
after_wai:
