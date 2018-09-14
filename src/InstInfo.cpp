//=============================================================================
// InstInfo.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// InstInfo
//-----------------------------------------------------------------------------
InstInfo::InstInfo()
{
	InitializeEntryMap();
}

void InstInfo::InitializeEntryMap()
{
	EntryMap &m = _entryMap;
	m.Add(Entry_ACC					("aba", 0x1b));
	m.Add(Entry_AxB_IMM8_DIR_IDX_EXT("adc", 0x89, 0x99, 0xa9, 0xb9, 0xc9, 0xd9, 0xe9, 0xf9));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("adca", 0x89, 0x99, 0xa9, 0xb9));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("adcb", 0xc9, 0xd9, 0xe9, 0xf9));
	m.Add(Entry_AxB_IMM8_DIR_IDX_EXT("add", 0x8b, 0x9b, 0xab, 0xbb, 0xcb, 0xdb, 0xeb, 0xfb));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("adda", 0x8b, 0x9b, 0xab, 0xbb));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("addb", 0xcb, 0xdb, 0xeb, 0xfb));
	m.Add(Entry_AxB_IMM8_DIR_IDX_EXT("and", 0x84, 0x94, 0xa4, 0xb4, 0xc4, 0xd4, 0xe4, 0xf4));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("anda", 0x84, 0x94, 0xa4, 0xb4));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("andb", 0xc4, 0xd4, 0xe4, 0xf4));
	m.Add(Entry_ACC_ACC_IDX_EXT		("asl", 0x48, 0x58, 0x68, 0x78));
	m.Add(Entry_ACC					("asla", 0x48));
	m.Add(Entry_ACC					("aslb", 0x58));
	m.Add(Entry_ACC_ACC_IDX_EXT		("asr", 0x47, 0x57, 0x67, 0x77));
	m.Add(Entry_ACC					("asra", 0x47));
	m.Add(Entry_ACC					("asrb", 0x57));
	m.Add(Entry_REL					("bcc", 0x24));
	m.Add(Entry_REL					("bcs", 0x25));
	m.Add(Entry_REL					("beq", 0x27));
	m.Add(Entry_REL					("bge", 0x2c));
	m.Add(Entry_REL					("bgt", 0x2e));
	m.Add(Entry_REL					("bhi", 0x22));
	m.Add(Entry_AxB_IMM8_DIR_IDX_EXT("bit", 0x85, 0x95, 0xa5, 0xb5, 0xc5, 0xd5, 0xe5, 0xf5));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("bita", 0x85, 0x95, 0xa5, 0xb5));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("bitb", 0xc5, 0xd5, 0xe5, 0xf5));
	m.Add(Entry_REL					("ble", 0x2f));
	m.Add(Entry_REL					("bls", 0x23));
	m.Add(Entry_REL					("blt", 0x2d));
	m.Add(Entry_REL					("bmi", 0x2b));
	m.Add(Entry_REL					("bne", 0x26));
	m.Add(Entry_REL					("bpl", 0x2a));
	m.Add(Entry_REL					("bra", 0x20));
	m.Add(Entry_REL					("bsr", 0x8d));
	m.Add(Entry_REL					("bvc", 0x28));
	m.Add(Entry_REL					("bvs", 0x29));
	m.Add(Entry_INH					("cba", 0x11));
	m.Add(Entry_INH					("clc", 0x0c));
	m.Add(Entry_INH					("cli", 0x0e));
	m.Add(Entry_ACC_ACC_IDX_EXT		("clr", 0x4f, 0x5f, 0x6f, 0x7f));
	m.Add(Entry_ACC					("clra", 0x4f));
	m.Add(Entry_ACC					("clrb", 0x5f));
	m.Add(Entry_INH					("clv", 0x0a));
	m.Add(Entry_AxB_IMM8_DIR_IDX_EXT("cmp", 0x81, 0x91, 0xa1, 0xb1, 0xc1, 0xd1, 0xe1, 0xf1));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("cmpa", 0x81, 0x91, 0xa1, 0xb1));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("cmpb", 0xc1, 0xd1, 0xe1, 0xf1));
	m.Add(Entry_ACC_ACC_IDX_EXT		("com", 0x43, 0x53, 0x63, 0x73));
	m.Add(Entry_ACC					("coma", 0x43));
	m.Add(Entry_ACC					("comb", 0x53));
	m.Add(Entry_DIR_IDX_IMM16_EXT	("cpx", 0x9c, 0xac, 0x8c, 0xbc));
	m.Add(Entry_INH					("daa", 0x19));
	m.Add(Entry_ACC_ACC_IDX_EXT		("dec", 0x4a, 0x5a, 0x6a, 0x7a));
	m.Add(Entry_ACC					("deca", 0x4a));
	m.Add(Entry_ACC					("decb", 0x5a));
	m.Add(Entry_INH					("des", 0x34));
	m.Add(Entry_INH					("dex", 0x09));
	m.Add(Entry_AxB_IMM8_DIR_IDX_EXT("eor", 0x88, 0x98, 0xa8, 0xb8, 0xc8, 0xd8, 0xe8, 0xf8));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("eora", 0x88, 0x98, 0xa8, 0xb8));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("eorb", 0xc8, 0xd8, 0xe8, 0xf8));
	m.Add(Entry_ACC_ACC_IDX_EXT		("inc", 0x4c, 0x5c, 0x6c, 0x7c));
	m.Add(Entry_ACC					("inca", 0x4c));
	m.Add(Entry_ACC					("incb", 0x5c));
	m.Add(Entry_INH					("ins", 0x31));
	m.Add(Entry_INH					("inx", 0x08));
	m.Add(Entry_IDX_EXT				("jmp", 0x6e, 0x7e));
	m.Add(Entry_IDX_EXT				("jsr", 0xad, 0xbd));
	m.Add(Entry_AxB_IMM8_DIR_IDX_EXT("lda", 0x86, 0x96, 0xa6, 0xb6, 0xc6, 0xd6, 0xe6, 0xf6));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("ldaa", 0x86, 0x96, 0xa6, 0xb6));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("ldab", 0xc6, 0xd6, 0xe6, 0xf6));
	m.Add(Entry_DIR_IDX_IMM16_EXT	("lds", 0x9e, 0xae, 0x8e, 0xbe));
	m.Add(Entry_DIR_IDX_IMM16_EXT	("ldx", 0xde, 0xee, 0xce, 0xfe));
	m.Add(Entry_ACC_ACC_IDX_EXT		("lsr", 0x44, 0x54, 0x64, 0x74));
	m.Add(Entry_ACC					("lsra", 0x44));
	m.Add(Entry_ACC					("lsrb", 0x54));
	m.Add(Entry_ACC_ACC_IDX_EXT		("neg", 0x40, 0x50, 0x60, 0x70));
	m.Add(Entry_ACC					("nega", 0x40));
	m.Add(Entry_ACC					("negb", 0x50));
	m.Add(Entry_INH					("nop", 0x01));
	m.Add(Entry_AxB_IMM8_DIR_IDX_EXT("ora", 0x8a, 0x9a, 0xaa, 0xba, 0xca, 0xda, 0xea, 0xfa));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("oraa", 0x8a, 0x9a, 0xaa, 0xba));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("orab", 0xca, 0xda, 0xea, 0xfa));
	m.Add(Entry_ACC_ACC				("psh", 0x36, 0x37));
	m.Add(Entry_ACC					("psha", 0x36));
	m.Add(Entry_ACC					("pshb", 0x37));
	m.Add(Entry_ACC_ACC				("pul", 0x32, 0x33));
	m.Add(Entry_ACC					("pula", 0x32));
	m.Add(Entry_ACC					("pulb", 0x33));
	m.Add(Entry_ACC_ACC_IDX_EXT		("rol", 0x49, 0x59, 0x69, 0x79));
	m.Add(Entry_ACC					("rola", 0x49));
	m.Add(Entry_ACC					("rolb", 0x59));
	m.Add(Entry_ACC_ACC_IDX_EXT		("ror", 0x46, 0x56, 0x66, 0x76));
	m.Add(Entry_ACC					("rora", 0x46));
	m.Add(Entry_ACC					("rorb", 0x56));
	m.Add(Entry_INH					("rti", 0x3b));
	m.Add(Entry_INH					("rts", 0x39));
	m.Add(Entry_INH					("sba", 0x10));
	m.Add(Entry_AxB_IMM8_DIR_IDX_EXT("sbc", 0x82, 0x92, 0xa2, 0xb2, 0xc2, 0xd2, 0xe2, 0xf2));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("sbca", 0x82, 0x92, 0xa2, 0xb2));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("sbcb", 0xc2, 0xd2, 0xe2, 0xf2));
	m.Add(Entry_INH					("sec", 0x0d));
	m.Add(Entry_INH					("sei", 0x0f));
	m.Add(Entry_INH					("sev", 0x0b));
	m.Add(Entry_AxB_DIR_IDX_EXT		("sta", 0x97, 0xa7, 0xb7, 0xd7, 0xe7, 0xf7));
	m.Add(Entry_DIR_IDX_EXT			("staa", 0x97, 0xa7, 0xb7));
	m.Add(Entry_DIR_IDX_EXT			("stab", 0xd7, 0xe7, 0xf7));
	m.Add(Entry_DIR_IDX_EXT			("sts", 0x9f, 0xaf, 0xbf));
	m.Add(Entry_DIR_IDX_EXT			("stx", 0xdf, 0xef, 0xff));
	m.Add(Entry_AxB_IMM8_DIR_IDX_EXT("sub", 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("suba", 0x80, 0x90, 0xa0, 0xb0));
	m.Add(Entry_IMM8_DIR_IDX_EXT	("subb", 0xc0, 0xd0, 0xe0, 0xf0));
	m.Add(Entry_INH					("swi", 0x3f));
	m.Add(Entry_INH					("tab", 0x16));
	m.Add(Entry_INH					("tap", 0x06));
	m.Add(Entry_INH					("tba", 0x17));
	m.Add(Entry_INH					("tpa", 0x07));
	m.Add(Entry_ACC_ACC_IDX_EXT		("tst", 0x4d, 0x5d, 0x6d, 0x7d));
	m.Add(Entry_ACC					("tsta", 0x4d));
	m.Add(Entry_ACC					("tstb", 0x5d));
	m.Add(Entry_INH					("tsx", 0x30));
	m.Add(Entry_INH					("txs", 0x35));
	m.Add(Entry_INH					("wai", 0x3e));
}

bool InstInfo::EvalExpr(Context &context, const Expr *pExpr)
{
	if (pExpr->IsType(Expr::TYPE_Inst)) {
		const Expr_Inst *pExprEx = dynamic_cast<const Expr_Inst *>(pExpr);
		const Entry *pEntry = _entryMap.Lookup(pExprEx->GetSymbol());
		if (pEntry == nullptr) {
			::printf("unknown instruction: %s\n", pExprEx->GetSymbol());
		} else {
			::printf("%s .. ", pExprEx->ToString().c_str());
		}
		return pEntry->ApplyRule(context, pExprEx->GetOperands());
	} else {
		// this shouldn't happen
		return false;
	}
	return true;
}

InstInfo::Entry *InstInfo::Entry_ACC(const String &symbol, UInt8 codeACC)
{
	Entry *pEntry = new Entry(symbol, "ACC");
	pEntry->AddRule(new Rule_ACC(codeACC));
	return pEntry;
}

InstInfo::Entry *InstInfo::Entry_REL(const String &symbol, UInt8 codeREL)
{
	Entry *pEntry = new Entry(symbol, "REL");
	pEntry->AddRule(new InstInfo::Rule_REL(codeREL));
	return pEntry;
}

InstInfo::Entry *InstInfo::Entry_INH(const String &symbol, UInt8 codeINH)
{
	Entry *pEntry = new Entry(symbol, "INH");
	pEntry->AddRule(new InstInfo::Rule_INH(codeINH));
	return pEntry;
}

InstInfo::Entry *InstInfo::Entry_ACC_ACC(const String &symbol, UInt8 codeACC_A, UInt8 codeACC_B)
{
	Entry *pEntry = new Entry(symbol, "ACC_ACC");
	pEntry->AddRule(new InstInfo::Rule_ACC(codeACC_A, "a"));
	pEntry->AddRule(new InstInfo::Rule_ACC(codeACC_B, "b"));
	return pEntry;
}

InstInfo::Entry *InstInfo::Entry_IDX_EXT(const String &symbol, UInt8 codeIDX, UInt8 codeEXT)
{
	Entry *pEntry = new Entry(symbol, "IDX_EXT");
	pEntry->AddRule(new InstInfo::Rule_IDX(codeIDX));
	pEntry->AddRule(new InstInfo::Rule_EXT(codeEXT));
	return pEntry;
}

InstInfo::Entry *InstInfo::Entry_DIR_IDX_EXT(
	const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeEXT)
{
	Entry *pEntry = new Entry(symbol, "DIR_IDX_EXT");
	pEntry->AddRule(new InstInfo::Rule_DIR(codeDIR));
	pEntry->AddRule(new InstInfo::Rule_IDX(codeIDX));
	pEntry->AddRule(new InstInfo::Rule_EXT(codeEXT));
	return pEntry;
}

InstInfo::Entry *InstInfo::Entry_DIR_IDX_IMM8_EXT(
	const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeIMM8, UInt8 codeEXT)
{
	Entry *pEntry = new Entry(symbol, "DIR_IDX_IMM8_EXT");
	pEntry->AddRule(new InstInfo::Rule_DIR(codeDIR));
	pEntry->AddRule(new InstInfo::Rule_IDX(codeIDX));
	pEntry->AddRule(new InstInfo::Rule_IMM8(codeIMM8));
	pEntry->AddRule(new InstInfo::Rule_EXT(codeEXT));
	return pEntry;
}

InstInfo::Entry *InstInfo::Entry_DIR_IDX_IMM16_EXT(
	const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeIMM16, UInt8 codeEXT)
{
	Entry *pEntry = new Entry(symbol, "DIR_IDX_IMM16_EXT");
	pEntry->AddRule(new InstInfo::Rule_DIR(codeDIR));
	pEntry->AddRule(new InstInfo::Rule_IDX(codeIDX));
	pEntry->AddRule(new InstInfo::Rule_IMM16(codeIMM16));
	pEntry->AddRule(new InstInfo::Rule_EXT(codeEXT));
	return pEntry;
}

InstInfo::Entry *InstInfo::Entry_ACC_ACC_IDX_EXT(
	const String &symbol, UInt8 codeACC_A, UInt8 codeACC_B, UInt8 codeIDX, UInt8 codeEXT)
{
	Entry *pEntry = new Entry(symbol, "ACC_ACC_IDX_EXT");
	pEntry->AddRule(new InstInfo::Rule_ACC(codeACC_A, "a"));
	pEntry->AddRule(new InstInfo::Rule_ACC(codeACC_B, "b"));
	pEntry->AddRule(new InstInfo::Rule_IDX(codeIDX));
	pEntry->AddRule(new InstInfo::Rule_EXT(codeEXT));
	return pEntry;
}

InstInfo::Entry *InstInfo::Entry_IMM8_DIR_IDX_EXT(
	const String &symbol, UInt8 codeIMM8, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeEXT)
{
	Entry *pEntry = new Entry(symbol, "IMM8_DIR_IDX_EXT");
	pEntry->AddRule(new InstInfo::Rule_IMM8(codeIMM8));
	pEntry->AddRule(new InstInfo::Rule_DIR(codeDIR));
	pEntry->AddRule(new InstInfo::Rule_IDX(codeIDX));
	pEntry->AddRule(new InstInfo::Rule_EXT(codeEXT));
	return pEntry;
}

InstInfo::Entry *InstInfo::Entry_AxB_DIR_IDX_EXT(
	const String &symbol,
	UInt8 codeDIR_A, UInt8 codeIDX_A, UInt8 codeEXT_A,
	UInt8 codeDIR_B, UInt8 codeIDX_B, UInt8 codeEXT_B)
{
	Entry *pEntry = new Entry(symbol, "AxB_DIR_IDX_EXT");
	pEntry->AddRule(new InstInfo::Rule_DIR(codeDIR_A, "a"));
	pEntry->AddRule(new InstInfo::Rule_IDX(codeIDX_A, "a"));
	pEntry->AddRule(new InstInfo::Rule_EXT(codeEXT_A, "a"));
	pEntry->AddRule(new InstInfo::Rule_DIR(codeDIR_B, "b"));
	pEntry->AddRule(new InstInfo::Rule_IDX(codeIDX_B, "b"));
	pEntry->AddRule(new InstInfo::Rule_EXT(codeEXT_B, "b"));
	return pEntry;
}

InstInfo::Entry *InstInfo::Entry_AxB_IMM8_DIR_IDX_EXT(
	const String &symbol,
	UInt8 codeIMM8_A, UInt8 codeDIR_A, UInt8 codeIDX_A, UInt8 codeEXT_A,
	UInt8 codeIMM8_B, UInt8 codeDIR_B, UInt8 codeIDX_B, UInt8 codeEXT_B)
{
	Entry *pEntry = new Entry(symbol, "AxB_IMM8_DIR_IDX_EXT");
	pEntry->AddRule(new InstInfo::Rule_IMM8(codeIMM8_A, "a"));
	pEntry->AddRule(new InstInfo::Rule_DIR(codeDIR_A, "a"));
	pEntry->AddRule(new InstInfo::Rule_IDX(codeIDX_A, "a"));
	pEntry->AddRule(new InstInfo::Rule_EXT(codeEXT_A, "a"));
	pEntry->AddRule(new InstInfo::Rule_IMM8(codeIMM8_B, "b"));
	pEntry->AddRule(new InstInfo::Rule_DIR(codeDIR_B, "b"));
	pEntry->AddRule(new InstInfo::Rule_IDX(codeIDX_B, "b"));
	pEntry->AddRule(new InstInfo::Rule_EXT(codeEXT_B, "b"));
	return pEntry;
}

//-----------------------------------------------------------------------------
// InstInfo::Rule
//-----------------------------------------------------------------------------
InstInfo::Rule::~Rule()
{
}

size_t InstInfo::Rule_ACC::Apply(Context &context, const ExprList &operands)
{
	if (_accName.empty()) {
		// OP .. _accName is empty
		if (operands.size() != 0) return 0;
	} else {
		// OP a ... _accName is "a"
		// OP b ... _accName is "b"
		if (operands.size() != 1) return 0;
		const Expr *pExpr = operands.front();
		if (!pExpr->IsType(Expr::TYPE_Symbol)) return 0;
		if (!dynamic_cast<const Expr_Symbol *>(pExpr)->MatchICase(_accName.c_str())) return 0;
	}
	context.PutByte(_code);
	return bytes;
}

size_t InstInfo::Rule_REL::Apply(Context &context, const ExprList &operands)
{
	// OP disp
	if (operands.size() != 1) return 0;
	context.PutByte(_code);
	return 0;
}

size_t InstInfo::Rule_INH::Apply(Context &context, const ExprList &operands)
{
	// OP
	if (operands.size() != 0) return 0;
	context.PutByte(_code);
	return bytes;
}

size_t InstInfo::Rule_IMM8::Apply(Context &context, const ExprList &operands)
{
	if (_accName.empty()) {
		// OP data8 ... _accName is empty
		if (operands.size() != 1) return 0;
	} else {
		// OP a,data8 ... _accName is "a"
		// OP b,data8 ... _accName is "b"
		if (operands.size() != 2) return 0;
		const Expr *pExpr = operands.front();
		if (!pExpr->IsType(Expr::TYPE_Symbol)) return 0;
		if (!dynamic_cast<const Expr_Symbol *>(pExpr)->MatchICase(_accName.c_str())) return 0;
	}
	const Expr *pExpr = operands.back();
	if (!pExpr->IsType(Expr::TYPE_Number)) return 0;
	UInt32 num = dynamic_cast<const Expr_Number *>(pExpr)->GetNumber();
	context.PutByte(_code);
	if (num > 0xff) {
		// error
	}
	context.PutByte(static_cast<UInt8>(num));
	return bytes;
}

size_t InstInfo::Rule_IMM16::Apply(Context &context, const ExprList &operands)
{
	// OP data16
	if (operands.size() != 1) return 0;
	const Expr *pExpr = operands.back();
	if (!pExpr->IsType(Expr::TYPE_Number)) return 0;
	UInt32 num = dynamic_cast<const Expr_Number *>(pExpr)->GetNumber();
	context.PutByte(_code);
	if (num > 0xffff) {
		// error
	}
	context.PutByte(static_cast<UInt8>(num >> 8));
	context.PutByte(static_cast<UInt8>(num));
	return bytes;
}

size_t InstInfo::Rule_DIR::Apply(Context &context, const ExprList &operands)
{
	if (_accName.empty()) {
		// OP (addr8) ... _accName is empty
		if (operands.size() != 1) return 0;
	} else {
		// OP a,(addr8) ... _accName is "a"
		// OP b,(addr8) ... _accName is "b"
		if (operands.size() != 2) return 0;
		const Expr *pExpr = operands.front();
		if (!pExpr->IsType(Expr::TYPE_Symbol)) return 0;
		if (!dynamic_cast<const Expr_Symbol *>(pExpr)->MatchICase(_accName.c_str())) return 0;
	}
	const Expr *pExpr = operands.back();
	if (!pExpr->IsType(Expr::TYPE_Parenthesis)) return 0;
	context.PutByte(_code);
	return bytes;
}

size_t InstInfo::Rule_IDX::Apply(Context &context, const ExprList &operands)
{
	if (_accName.empty()) {
		// OP [x+data8] ... _accName is empty
		if (operands.size() != 1) return 0;
	} else {
		// OP a,[x+data8] ... _accName is "a"
		// OP b,[x+data8] ... _accName is "b"
		if (operands.size() != 2) return 0;
		const Expr *pExpr = operands.front();
		if (!pExpr->IsType(Expr::TYPE_Symbol)) return 0;
		if (!dynamic_cast<const Expr_Symbol *>(pExpr)->MatchICase(_accName.c_str())) return 0;
	}
	const Expr *pExpr = operands.back();
	if (!pExpr->IsType(Expr::TYPE_Bracket)) return 0;
	context.PutByte(_code);
	return bytes;
}

size_t InstInfo::Rule_EXT::Apply(Context &context, const ExprList &operands)
{
	if (_accName.empty()) {
		// OP [addr16] ... _accName is empty
		if (operands.size() != 1) return 0;
	} else {
		// OP a,[addr16] ... _accName is "a"
		// OP b,[addr16] ... _accName is "b"
		if (operands.size() != 2) return 0;
		const Expr *pExpr = operands.front();
		if (!pExpr->IsType(Expr::TYPE_Symbol)) return 0;
		if (!dynamic_cast<const Expr_Symbol *>(pExpr)->MatchICase(_accName.c_str())) return 0;
	}
	const Expr *pExpr = operands.back();
	if (!pExpr->IsType(Expr::TYPE_Bracket)) return 0;
	context.PutByte(_code);
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
// InstInfo::Entry
//-----------------------------------------------------------------------------
InstInfo::Entry::Entry(const String &symbol, const String &syntaxDesc) :
	_symbol(symbol), _syntaxDesc(syntaxDesc)
{
}

bool InstInfo::Entry::ApplyRule(Context &context, const ExprList &operands) const
{
	for (auto pRule : _ruleOwner) {
		size_t bytes = pRule->Apply(context, operands);
		if (bytes != 0) return true;
	}
	// error
	return false;
}

//-----------------------------------------------------------------------------
// InstInfo::EntryMap
//-----------------------------------------------------------------------------
InstInfo::EntryMap::~EntryMap()
{
	for (auto iter : *this) {
		delete iter.second;
	}
}

const InstInfo::Entry *InstInfo::EntryMap::Lookup(const char *symbol) const
{
	String symbolLower = ToLower(symbol);
	const_iterator iter = find(symbolLower);
	return (iter == end())? nullptr : iter->second;
}
