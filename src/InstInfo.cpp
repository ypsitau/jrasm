//=============================================================================
// InstInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// InstInfo
//-----------------------------------------------------------------------------
InstInfo::InstInfo(const String &symbol) : _symbol(symbol)
{
}

InstInfo *InstInfo::Syntax_ACC(const String &symbol, UInt8 codeACC)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_ACC(codeACC));
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_REL(const String &symbol, UInt8 codeREL)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_REL(codeREL));
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_INH(const String &symbol, UInt8 codeINH)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_INH(codeINH));
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_ACC_ACC(const String &symbol, UInt8 codeACC_A, UInt8 codeACC_B)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_ACC(codeACC_A));
	pInstInfo->AddRule(new InstInfo::Rule_ACC(codeACC_B));
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_IDX_EXT(const String &symbol, UInt8 codeIDX, UInt8 codeEXT)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT));
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_DIR_IDX_EXT(
	const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeEXT)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_DIR(codeDIR));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT));
	return pInstInfo;
}


InstInfo *InstInfo::Syntax_DIR_IDX_IMM_EXT(
	const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeIMM, UInt8 codeEXT)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_DIR(codeDIR));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX));
	pInstInfo->AddRule(new InstInfo::Rule_IMM(codeIMM));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT));
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_ACC_ACC_IDX_EXT(
	const String &symbol, UInt8 codeACC_A, UInt8 codeACC_B, UInt8 codeIDX, UInt8 codeEXT)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_ACC(codeACC_A, "a"));
	pInstInfo->AddRule(new InstInfo::Rule_ACC(codeACC_B, "b"));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT));
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_IMM_DIR_IDX_EXT(
	const String &symbol, UInt8 codeIMM, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeEXT)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_IMM(codeIMM));
	pInstInfo->AddRule(new InstInfo::Rule_DIR(codeDIR));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT));
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_AxB_DIR_IDX_EXT(
	const String &symbol,
	UInt8 codeDIR_A, UInt8 codeIDX_A, UInt8 codeEXT_A,
	UInt8 codeDIR_B, UInt8 codeIDX_B, UInt8 codeEXT_B)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_DIR(codeDIR_A, "a"));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX_A, "a"));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT_A, "a"));
	pInstInfo->AddRule(new InstInfo::Rule_DIR(codeDIR_B, "b"));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX_B, "b"));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT_B, "b"));
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT(
	const String &symbol,
	UInt8 codeIMM_A, UInt8 codeDIR_A, UInt8 codeIDX_A, UInt8 codeEXT_A,
	UInt8 codeIMM_B, UInt8 codeDIR_B, UInt8 codeIDX_B, UInt8 codeEXT_B)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_IMM(codeIMM_A, "a"));
	pInstInfo->AddRule(new InstInfo::Rule_DIR(codeDIR_A, "a"));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX_A, "a"));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT_A, "a"));
	pInstInfo->AddRule(new InstInfo::Rule_IMM(codeIMM_B, "b"));
	pInstInfo->AddRule(new InstInfo::Rule_DIR(codeDIR_B, "b"));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX_B, "b"));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT_B, "b"));
	return pInstInfo;
}

//-----------------------------------------------------------------------------
// InstInfo::Rule
//-----------------------------------------------------------------------------
InstInfo::Rule::~Rule()
{
}

size_t InstInfo::Rule_ACC::Apply(Binary &buff, const ElementList &elemList)
{
	if (_accName.empty()) {
		// OP .. _accName is empty
		if (elemList.size() != 0) return 0;
	} else {
		// OP a ... _accName is "a"
		// OP b ... _accName is "b"
		if (elemList.size() != 1) return 0;
		const Element *pElem = elemList.front();
		if (!pElem->IsType(Element::TYPE_Symbol)) return 0;
		if (!dynamic_cast<const Element_Symbol *>(pElem)->MatchICase(_accName.c_str())) return 0;
	}
	buff += _code;
	return bytes;
}

size_t InstInfo::Rule_REL::Apply(Binary &buff, const ElementList &elemList)
{
	// OP disp
	if (elemList.size() != 1) return 0;
	buff += _code;
	return 0;
}

size_t InstInfo::Rule_INH::Apply(Binary &buff, const ElementList &elemList)
{
	// OP
	if (elemList.size() != 0) return 0;
	buff += _code;
	return bytes;
}

size_t InstInfo::Rule_IMM::Apply(Binary &buff, const ElementList &elemList)
{
	if (_accName.empty()) {
		// OP data8 ... _accName is empty
		if (elemList.size() != 1) return 0;
	} else {
		// OP a,data8 ... _accName is "a"
		// OP b,data8 ... _accName is "b"
		if (elemList.size() != 2) return 0;
		const Element *pElem = elemList.front();
		if (!pElem->IsType(Element::TYPE_Symbol)) return 0;
		if (!dynamic_cast<const Element_Symbol *>(pElem)->MatchICase(_accName.c_str())) return 0;
	}
	const Element *pElem = elemList.back();
	if (!pElem->IsType(Element::TYPE_Number)) return 0;
	UInt32 num = dynamic_cast<const Element_Number *>(pElem)->GetNumber();
	if (num > 0xff) {
		// error
	}
	buff += _code;
	buff += static_cast<UInt8>(num);
	return bytes;
}

size_t InstInfo::Rule_DIR::Apply(Binary &buff, const ElementList &elemList)
{
	if (_accName.empty()) {
		// OP (addr8) ... _accName is empty
		if (elemList.size() != 1) return 0;
	} else {
		// OP a,(addr8) ... _accName is "a"
		// OP b,(addr8) ... _accName is "b"
		if (elemList.size() != 2) return 0;
		const Element *pElem = elemList.front();
		if (!pElem->IsType(Element::TYPE_Symbol)) return 0;
		if (!dynamic_cast<const Element_Symbol *>(pElem)->MatchICase(_accName.c_str())) return 0;
	}
	buff += _code;
	return bytes;
}

size_t InstInfo::Rule_IDX::Apply(Binary &buff, const ElementList &elemList)
{
	if (_accName.empty()) {
		// OP [x+data8] ... _accName is empty
		if (elemList.size() != 1) return 0;
	} else {
		// OP a,[x+data8] ... _accName is "a"
		// OP b,[x+data8] ... _accName is "b"
		if (elemList.size() != 2) return 0;
		const Element *pElem = elemList.front();
		if (!pElem->IsType(Element::TYPE_Symbol)) return 0;
		if (!dynamic_cast<const Element_Symbol *>(pElem)->MatchICase(_accName.c_str())) return 0;
	}
	buff += _code;
	return bytes;
}

size_t InstInfo::Rule_EXT::Apply(Binary &buff, const ElementList &elemList)
{
	if (_accName.empty()) {
		// OP [addr16] ... _accName is empty
		if (elemList.size() != 1) return 0;
	} else {
		// OP a,[addr16] ... _accName is "a"
		// OP b,[addr16] ... _accName is "b"
		if (elemList.size() != 2) return 0;
		const Element *pElem = elemList.front();
		if (!pElem->IsType(Element::TYPE_Symbol)) return 0;
		if (!dynamic_cast<const Element_Symbol *>(pElem)->MatchICase(_accName.c_str())) return 0;
	}
	buff += _code;
	return bytes;
}

//-----------------------------------------------------------------------------
// InstInfo::RuleOwner
//-----------------------------------------------------------------------------
InstInfo::RuleOwner::~RuleOwner()
{
	Clear();
}

void InstInfo::RuleOwner::Clear()
{
	for (auto pRule : *this) {
		delete pRule;
	}
	clear();
}

//-----------------------------------------------------------------------------
// InstInfoMap
//-----------------------------------------------------------------------------
InstInfoMap::~InstInfoMap()
{
	for (auto iter : *this) {
		delete iter.second;
	}
}

void InstInfoMap::Initialize()
{
	Add(InstInfo::Syntax_ACC				("aba", 0x1b));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("adc", 0x89, 0x99, 0xa9, 0xb9, 0xc9, 0xd9, 0xe9, 0xf9));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("adca", 0x89, 0x99, 0xa9, 0xb9));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("adcb", 0xc9, 0xd9, 0xe9, 0xf9));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("add", 0x8b, 0x9b, 0xab, 0xbb, 0xcb, 0xdb, 0xeb, 0xfb));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("adda", 0x8b, 0x9b, 0xab, 0xbb));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("addb", 0xcb, 0xdb, 0xeb, 0xfb));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("and", 0x84, 0x94, 0xa4, 0xb4, 0xc4, 0xd4, 0xe4, 0xf4));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("anda", 0x84, 0x94, 0xa4, 0xb4));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("andb", 0xc4, 0xd4, 0xe4, 0xf4));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("asl", 0x48, 0x58, 0x68, 0x78));
	Add(InstInfo::Syntax_ACC				("asla", 0x48));
	Add(InstInfo::Syntax_ACC				("aslb", 0x58));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("asr", 0x47, 0x57, 0x67, 0x77));
	Add(InstInfo::Syntax_ACC				("asra", 0x47));
	Add(InstInfo::Syntax_ACC				("asrb", 0x57));
	Add(InstInfo::Syntax_REL				("bcc", 0x24));
	Add(InstInfo::Syntax_REL				("bcs", 0x25));
	Add(InstInfo::Syntax_REL				("beq", 0x27));
	Add(InstInfo::Syntax_REL				("bge", 0x2c));
	Add(InstInfo::Syntax_REL				("bgt", 0x2e));
	Add(InstInfo::Syntax_REL				("bhi", 0x22));
	Add(InstInfo::Syntax_REL				("ble", 0x2f));
	Add(InstInfo::Syntax_REL				("bls", 0x23));
	Add(InstInfo::Syntax_REL				("blt", 0x2d));
	Add(InstInfo::Syntax_REL				("bmi", 0x2b));
	Add(InstInfo::Syntax_REL				("bne", 0x26));
	Add(InstInfo::Syntax_REL				("bpl", 0x2a));
	Add(InstInfo::Syntax_REL				("bra", 0x20));
	Add(InstInfo::Syntax_REL				("bsr", 0x8d));
	Add(InstInfo::Syntax_REL				("bvc", 0x28));
	Add(InstInfo::Syntax_REL				("bvs", 0x29));
	Add(InstInfo::Syntax_INH				("cba", 0x11));
	Add(InstInfo::Syntax_INH				("clc", 0x0c));
	Add(InstInfo::Syntax_INH				("cli", 0x0e));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("clr", 0x4f, 0x5f, 0x6f, 0x7f));
	Add(InstInfo::Syntax_ACC				("clra", 0x4f));
	Add(InstInfo::Syntax_ACC				("clrb", 0x5f));
	Add(InstInfo::Syntax_INH				("clv", 0x0a));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("cmp", 0x81, 0x91, 0xa1, 0xb1, 0xc1, 0xd1, 0xe1, 0xf1));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("cmpa", 0x81, 0x91, 0xa1, 0xb1));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("cmpb", 0xc1, 0xd1, 0xe1, 0xf1));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("com", 0x43, 0x53, 0x63, 0x73));
	Add(InstInfo::Syntax_ACC				("coma", 0x43));
	Add(InstInfo::Syntax_ACC				("comb", 0x53));
	Add(InstInfo::Syntax_DIR_IDX_IMM_EXT	("cpx", 0x9c, 0xac, 0x8c, 0xbc));
	Add(InstInfo::Syntax_INH				("daa", 0x19));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("dec", 0x4a, 0x5a, 0x6a, 0x7a));
	Add(InstInfo::Syntax_ACC				("deca", 0x4a));
	Add(InstInfo::Syntax_ACC				("decb", 0x5a));
	Add(InstInfo::Syntax_INH				("des", 0x34));
	Add(InstInfo::Syntax_INH				("dex", 0x09));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("eor", 0x88, 0x98, 0xa8, 0xb8, 0xc8, 0xd8, 0xe8, 0xf8));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("eora", 0x88, 0x98, 0xa8, 0xb8));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("eorb", 0xc8, 0xd8, 0xe8, 0xf8));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("inc", 0x4c, 0x5c, 0x6c, 0x7c));
	Add(InstInfo::Syntax_ACC				("inca", 0x4c));
	Add(InstInfo::Syntax_ACC				("incb", 0x5c));
	Add(InstInfo::Syntax_INH				("ins", 0x31));
	Add(InstInfo::Syntax_INH				("inx", 0x08));
	Add(InstInfo::Syntax_IDX_EXT			("jmp", 0x6e, 0x7e));
	Add(InstInfo::Syntax_IDX_EXT			("jsr", 0xad, 0xbd));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("lda", 0x86, 0x96, 0xa6, 0xb6, 0xc6, 0xd6, 0xe6, 0xf6));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("ldaa", 0x86, 0x96, 0xa6, 0xb6));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("ldab", 0xc6, 0xd6, 0xe6, 0xf6));
	Add(InstInfo::Syntax_DIR_IDX_IMM_EXT	("lds", 0x9e, 0xae, 0x8e, 0xbe));
	Add(InstInfo::Syntax_DIR_IDX_IMM_EXT	("ldx", 0xde, 0xee, 0xce, 0xfe));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("lsr", 0x44, 0x54, 0x64, 0x74));
	Add(InstInfo::Syntax_ACC				("lsra", 0x44));
	Add(InstInfo::Syntax_ACC				("lsrb", 0x54));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("neg", 0x40, 0x50, 0x60, 0x70));
	Add(InstInfo::Syntax_ACC				("nega", 0x40));
	Add(InstInfo::Syntax_ACC				("negb", 0x50));
	Add(InstInfo::Syntax_INH				("nop", 0x01));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("ora", 0x8a, 0x9a, 0xaa, 0xba, 0xca, 0xda, 0xea, 0xfa));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("oraa", 0x8a, 0x9a, 0xaa, 0xba));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("orab", 0xca, 0xda, 0xea, 0xfa));
	Add(InstInfo::Syntax_ACC_ACC			("psh", 0x36, 0x37));
	Add(InstInfo::Syntax_ACC				("psha", 0x36));
	Add(InstInfo::Syntax_ACC				("pshb", 0x37));
	Add(InstInfo::Syntax_ACC_ACC			("pul", 0x32, 0x33));
	Add(InstInfo::Syntax_ACC				("pula", 0x32));
	Add(InstInfo::Syntax_ACC				("pulb", 0x33));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("rol", 0x49, 0x59, 0x69, 0x79));
	Add(InstInfo::Syntax_ACC				("rola", 0x49));
	Add(InstInfo::Syntax_ACC				("rolb", 0x59));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("ror", 0x46, 0x56, 0x66, 0x76));
	Add(InstInfo::Syntax_ACC				("rora", 0x46));
	Add(InstInfo::Syntax_ACC				("rorb", 0x56));
	Add(InstInfo::Syntax_INH				("rti", 0x3b));
	Add(InstInfo::Syntax_INH				("rts", 0x39));
	Add(InstInfo::Syntax_INH				("sba", 0x10));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("sbc", 0x82, 0x92, 0xa2, 0xb2, 0xc2, 0xd2, 0xe2, 0xf2));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("sbca", 0x82, 0x92, 0xa2, 0xb2));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("sbcb", 0xc2, 0xd2, 0xe2, 0xf2));
	Add(InstInfo::Syntax_INH				("sec", 0x0d));
	Add(InstInfo::Syntax_INH				("sei", 0x0f));
	Add(InstInfo::Syntax_INH				("sev", 0x0b));
	Add(InstInfo::Syntax_AxB_DIR_IDX_EXT	("sta", 0x97, 0xa7, 0xb7, 0xd7, 0xe7, 0xf7));
	Add(InstInfo::Syntax_DIR_IDX_EXT		("staa", 0x97, 0xa7, 0xb7));
	Add(InstInfo::Syntax_DIR_IDX_EXT		("stab", 0xd7, 0xe7, 0xf7));
	Add(InstInfo::Syntax_DIR_IDX_EXT		("sts", 0x9f, 0xaf, 0xbf));
	Add(InstInfo::Syntax_DIR_IDX_EXT		("stx", 0xdf, 0xef, 0xff));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("sub", 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("suba", 0x80, 0x90, 0xa0, 0xb0));
	Add(InstInfo::Syntax_IMM_DIR_IDX_EXT	("subb", 0xc0, 0xd0, 0xe0, 0xf0));
	Add(InstInfo::Syntax_INH				("swi", 0x3f));
	Add(InstInfo::Syntax_INH				("tab", 0x16));
	Add(InstInfo::Syntax_INH				("tap", 0x06));
	Add(InstInfo::Syntax_INH				("tba", 0x17));
	Add(InstInfo::Syntax_INH				("tpa", 0x07));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("tst", 0x4d, 0x5d, 0x6d, 0x7d));
	Add(InstInfo::Syntax_ACC				("tsta", 0x4d));
	Add(InstInfo::Syntax_ACC				("tstb", 0x5d));
	Add(InstInfo::Syntax_INH				("tsx", 0x30));
	Add(InstInfo::Syntax_INH				("txs", 0x35));
	Add(InstInfo::Syntax_INH				("wai", 0x3e));
}

const InstInfo *InstInfoMap::Lookup(const char *symbol) const
{
	String symbolLower = ToLower(symbol);
	const_iterator iter = find(symbolLower);
	return (iter == end())? nullptr : iter->second;
}
