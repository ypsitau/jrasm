//=============================================================================
// jrasm.cpp
//=============================================================================
#include "stdafx.h"
#include "jrasm.h"

class ElementList;
class ElementOwner;
typedef String Binary;

//-----------------------------------------------------------------------------
// InstInfo
//-----------------------------------------------------------------------------
class InstInfo {
public:
	class Rule {
	protected:
		UInt8 _code;
	public:
		inline Rule(UInt8 code) : _code(code) {}
		virtual ~Rule();
		virtual size_t Apply(Binary &buff, const ElementList &elemList) = 0;
	};
	class Rule_ACC : public Rule {
	private:
		char _chAcc;
	public:
		enum { bytes = 1 };
	public:
		inline Rule_ACC(UInt8 code, char chAcc = '\0') : Rule(code), _chAcc(chAcc) {}
		virtual size_t Apply(Binary &buff, const ElementList &elemList);
	};
	class Rule_REL : public Rule {
	public:
		enum { bytes = 2 };
	public:
		inline Rule_REL(UInt8 code) : Rule(code) {}
		virtual size_t Apply(Binary &buff, const ElementList &elemList);
	};
	class Rule_INH : public Rule {
	public:
		enum { bytes = 1 };
	public:
		inline Rule_INH(UInt8 code) : Rule(code) {}
		virtual size_t Apply(Binary &buff, const ElementList &elemList);
	};
	class Rule_IMM : public Rule {
	private:
		char _chAcc;
	public:
		enum { bytes = 2 };
	public:
		inline Rule_IMM(UInt8 code, char chAcc = '\0') : Rule(code), _chAcc(chAcc) {}
		virtual size_t Apply(Binary &buff, const ElementList &elemList);
	};
	class Rule_DIR : public Rule {
	private:
		char _chAcc;
	public:
		enum { bytes = 2 };
	public:
		inline Rule_DIR(UInt8 code, char chAcc = '\0') : Rule(code), _chAcc(chAcc) {}
		virtual size_t Apply(Binary &buff, const ElementList &elemList);
	};
	class Rule_IDX : public Rule {
	private:
		char _chAcc;
	public:
		enum { bytes = 2 };
	public:
		inline Rule_IDX(UInt8 code, char chAcc = '\0') : Rule(code), _chAcc(chAcc) {}
		virtual size_t Apply(Binary &buff, const ElementList &elemList);
	};
	class Rule_EXT : public Rule {
	private:
		char _chAcc;
	public:
		enum { bytes = 3 };
	public:
		inline Rule_EXT(UInt8 code, char chAcc = '\0') : Rule(code), _chAcc(chAcc) {}
		virtual size_t Apply(Binary &buff, const ElementList &elemList);
	};
	typedef std::vector<Rule *> RuleList;
	class RuleOwner : public RuleList {
	public:
		~RuleOwner();
		void Clear();
	};
private:
	String _symbol;
	RuleOwner _ruleOwner;
public:
	InstInfo(const String &symbol);
public:
	//bool ApplyRule();
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline void AddRule(Rule *pRule) { _ruleOwner.push_back(pRule); }
	static InstInfo *Syntax_ACC(const String &symbol, UInt8 codeACC);
	static InstInfo *Syntax_REL(const String &symbol, UInt8 codeREL);
	static InstInfo *Syntax_INH(const String &symbol, UInt8 codeINH);
	static InstInfo *Syntax_ACC_ACC(const String &symbol, UInt8 codeACC_A, UInt8 codeACC_B);
	static InstInfo *Syntax_IDX_EXT(const String &symbol, UInt8 codeIDX, UInt8 codeEXT);
	static InstInfo *Syntax_DIR_IDX_EXT(
		const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeEXT);
	static InstInfo *Syntax_DIR_IDX_IMM_EXT(
		const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeIMM, UInt8 codeEXT);
	static InstInfo *Syntax_ACC_ACC_IDX_EXT(
		const String &symbol, UInt8 codeACC_A, UInt8 codeACC_B, UInt8 codeIDX, UInt8 codeEXT);
	static InstInfo *Syntax_AxB_DIR_IDX_EXT	(
		const String &symbol,
		UInt8 codeDIR_A, UInt8 codeIDX_A, UInt8 codeEXT_A,
		UInt8 codeDIR_B, UInt8 codeIDX_B, UInt8 codeEXT_B);
	static InstInfo *Syntax_AxB_IMM_DIR_IDX_EXT(
		const String &symbol,
		UInt8 codeIMM_A, UInt8 codeDIR_A, UInt8 codeIDX_A, UInt8 codeEXT_A,
		UInt8 codeIMM_B, UInt8 codeDIR_B, UInt8 codeIDX_B, UInt8 codeEXT_B);
};

//-----------------------------------------------------------------------------
// InstInfoMap
//-----------------------------------------------------------------------------
class InstInfoMap : public std::map<String, InstInfo *> {
public:
	~InstInfoMap();
	void Initialize();
	inline void Add(InstInfo *pInstInfo) {
		insert(std::make_pair(ToLower(pInstInfo->GetSymbol()), pInstInfo));
	}
	const InstInfo *Lookup(const char *symbol) const;
};

//-----------------------------------------------------------------------------
// Element
//-----------------------------------------------------------------------------
class Element {
public:
	enum Type {
		TYPE_Inst,
		TYPE_A,
		TYPE_B,
		TYPE_X,
		TYPE_Number,
		TYPE_Symbol,
		TYPE_String,
		TYPE_BinOp,
		TYPE_Bracket,
		TYPE_Parenthesis,
	};
private:
	int _cntRef;
	Type _type;
	std::auto_ptr<ElementOwner> _pElemChildren;
public:
	DeclareReferenceAccessor(Element);
public:
	Element(Type type);
protected:
	virtual ~Element();
public:
	bool IsType(Type type) const { return _type == type; }
	void AddChild(Element *pElement);
	ElementOwner &GetChildren() { return *_pElemChildren; }
	const ElementOwner &GetChildren() const { return *_pElemChildren; }
	//virtual bool Reduce() = 0;
	virtual String ToString() const = 0;
};

//-----------------------------------------------------------------------------
// ElementList
//-----------------------------------------------------------------------------
class ElementList : public std::vector<Element *> {
public:
	String ToString() const;
	void Print() const;
};

//-----------------------------------------------------------------------------
// ElementStack
//-----------------------------------------------------------------------------
typedef std::vector<Element *> ElementStack;

//-----------------------------------------------------------------------------
// ElementOwner
//-----------------------------------------------------------------------------
class ElementOwner : public ElementList {
public:
	~ElementOwner();
	void Clear();
};

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
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("add", 0x8b, 0x9b, 0xab, 0xbb, 0xcb, 0xdb, 0xeb, 0xfb));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("and", 0x84, 0x94, 0xa4, 0xb4, 0xc4, 0xd4, 0xe4, 0xf4));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("asl", 0x48, 0x58, 0x68, 0x78));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("asr", 0x47, 0x57, 0x67, 0x77));
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
	Add(InstInfo::Syntax_INH				("clv", 0x0a));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("cmp", 0x81, 0x91, 0xa1, 0xb1, 0xc1, 0xd1, 0xe1, 0xf1));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("com", 0x43, 0x53, 0x63, 0x73));
	Add(InstInfo::Syntax_DIR_IDX_IMM_EXT	("cpx", 0x9c, 0xac, 0x8c, 0xbc));
	Add(InstInfo::Syntax_INH				("daa", 0x19));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("dec", 0x4a, 0x5a, 0x6a, 0x7a));
	Add(InstInfo::Syntax_INH				("des", 0x34));
	Add(InstInfo::Syntax_INH				("dex", 0x09));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("eor", 0x88, 0x98, 0xa8, 0xb8, 0xc8, 0xd8, 0xe8, 0xf8));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("inc", 0x4c, 0x5c, 0x6c, 0x7c));
	Add(InstInfo::Syntax_INH				("ins", 0x31));
	Add(InstInfo::Syntax_INH				("inx", 0x08));
	Add(InstInfo::Syntax_IDX_EXT			("jmp", 0x6e, 0x7e));
	Add(InstInfo::Syntax_IDX_EXT			("jsr", 0xad, 0xbd));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("lda", 0x86, 0x96, 0xa6, 0xb6, 0xc6, 0xd6, 0xe6, 0xf6));
	Add(InstInfo::Syntax_DIR_IDX_IMM_EXT	("lds", 0x9e, 0xae, 0x8e, 0xbe));
	Add(InstInfo::Syntax_DIR_IDX_IMM_EXT	("ldx", 0xde, 0xee, 0xce, 0xfe));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("lsr", 0x44, 0x54, 0x64, 0x74));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("neg", 0x40, 0x50, 0x60, 0x70));
	Add(InstInfo::Syntax_INH				("nop", 0x01));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("ora", 0x8a, 0x9a, 0xaa, 0xba, 0xca, 0xda, 0xea, 0xfa));
	Add(InstInfo::Syntax_ACC_ACC			("psh", 0x36, 0x37));
	Add(InstInfo::Syntax_ACC_ACC			("pul", 0x32, 0x32));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("rol", 0x49, 0x59, 0x69, 0x79));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("ror", 0x46, 0x56, 0x66, 0x76));
	Add(InstInfo::Syntax_INH				("rti", 0x3b));
	Add(InstInfo::Syntax_INH				("rts", 0x39));
	Add(InstInfo::Syntax_INH				("sba", 0x10));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("sbc", 0x82, 0x92, 0xa2, 0xb2, 0xc2, 0xd2, 0xe2, 0xf2));
	Add(InstInfo::Syntax_INH				("sec", 0x0d));
	Add(InstInfo::Syntax_INH				("sei", 0x0f));
	Add(InstInfo::Syntax_INH				("sev", 0x0b));
	Add(InstInfo::Syntax_AxB_DIR_IDX_EXT	("sta", 0x97, 0xa7, 0xb7, 0xe7, 0xe7, 0xf7));
	Add(InstInfo::Syntax_DIR_IDX_EXT		("sts", 0x9f, 0xaf, 0xbf));
	Add(InstInfo::Syntax_DIR_IDX_EXT		("stx", 0xdf, 0xef, 0xff));
	Add(InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT("sub", 0x80, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0));
	Add(InstInfo::Syntax_INH				("swi", 0x3f));
	Add(InstInfo::Syntax_INH				("tab", 0x16));
	Add(InstInfo::Syntax_INH				("tap", 0x06));
	Add(InstInfo::Syntax_INH				("tba", 0x17));
	Add(InstInfo::Syntax_INH				("tpa", 0x07));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("tst", 0x4d, 0x5d, 0x6d, 0x7d));
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
	pInstInfo->AddRule(new InstInfo::Rule_ACC(codeACC_A, 'a'));
	pInstInfo->AddRule(new InstInfo::Rule_ACC(codeACC_B, 'b'));
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
	pInstInfo->AddRule(new InstInfo::Rule_DIR(codeDIR_A, 'a'));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX_A, 'a'));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT_A, 'a'));
	pInstInfo->AddRule(new InstInfo::Rule_DIR(codeDIR_B, 'b'));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX_B, 'b'));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT_B, 'b'));
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_AxB_IMM_DIR_IDX_EXT(
	const String &symbol,
	UInt8 codeIMM_A, UInt8 codeDIR_A, UInt8 codeIDX_A, UInt8 codeEXT_A,
	UInt8 codeIMM_B, UInt8 codeDIR_B, UInt8 codeIDX_B, UInt8 codeEXT_B)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	pInstInfo->AddRule(new InstInfo::Rule_IMM(codeIMM_A, 'a'));
	pInstInfo->AddRule(new InstInfo::Rule_DIR(codeDIR_A, 'a'));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX_A, 'a'));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT_A, 'a'));
	pInstInfo->AddRule(new InstInfo::Rule_IMM(codeIMM_B, 'b'));
	pInstInfo->AddRule(new InstInfo::Rule_DIR(codeDIR_B, 'b'));
	pInstInfo->AddRule(new InstInfo::Rule_IDX(codeIDX_B, 'b'));
	pInstInfo->AddRule(new InstInfo::Rule_EXT(codeEXT_B, 'b'));
	return pInstInfo;
}

class ElementOwner;

//-----------------------------------------------------------------------------
// InstInfo::Rule
//-----------------------------------------------------------------------------
InstInfo::Rule::~Rule()
{
}

size_t InstInfo::Rule_ACC::Apply(Binary &buff, const ElementList &elemList)
{
	if (_chAcc == '\0') {
		// OP .. _chAcc == '\0'
		if (elemList.size() != 0) return 0;
	} else {
		// OP a ... _chAcc == 'a'
		// OP b ... _chAcc == 'b'
		if (elemList.size() != 1) return 0;
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
	if (_chAcc == '\0') {
		// OP #data8 ... _chAcc == '\0'
		if (elemList.size() != 1) return 0;
	} else {
		// OP a,#data8 ... _chAcc == 'a'
		// OP b,#data8 ... _chAcc == 'b'
		if (elemList.size() != 2) return 0;
	}
	buff += _code;
	return bytes;
}

size_t InstInfo::Rule_DIR::Apply(Binary &buff, const ElementList &elemList)
{
	if (_chAcc == '\0') {
		// OP (addr8) ... _chAcc == '\0'
		if (elemList.size() != 1) return 0;
	} else {
		// OP a,(addr8) ... _chAcc == 'a'
		// OP b,(addr8) ... _chAcc == 'b'
		if (elemList.size() != 2) return 0;
	}
	buff += _code;
	return bytes;
}

size_t InstInfo::Rule_IDX::Apply(Binary &buff, const ElementList &elemList)
{
	if (_chAcc == '\0') {
		// OP [x+data8] ... _chAcc == '\0'
		if (elemList.size() != 1) return 0;
	} else {
		// OP a,[x+data8] ... _chAcc == 'a'
		// OP b,[x+data8] ... _chAcc == 'b'
		if (elemList.size() != 2) return 0;
	}
	buff += _code;
	return bytes;
}

size_t InstInfo::Rule_EXT::Apply(Binary &buff, const ElementList &elemList)
{
	if (_chAcc == '\0') {
		// OP [addr16] ... _chAcc == '\0'
		if (elemList.size() != 1) return 0;
	} else {
		// OP a,[addr16] ... _chAcc == 'a'
		// OP b,[addr16] ... _chAcc == 'b'
		if (elemList.size() != 2) return 0;
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


String ElementList::ToString() const
{
	String rtn;
	for (auto pElement : *this) {
		if (!rtn.empty()) rtn += ",";
		rtn += pElement->ToString();
	}
	return rtn;
}

void ElementList::Print() const
{
	for (auto pElement : *this) {
		::printf("%s\n", pElement->ToString().c_str());
	}
}

//-----------------------------------------------------------------------------
// Element
//-----------------------------------------------------------------------------
Element::Element(Type type) : _cntRef(1), _type(type), _pElemChildren(new ElementOwner())
{
}

Element::~Element()
{
}

void Element::AddChild(Element *pElement)
{
	_pElemChildren->push_back(pElement);
}

//-----------------------------------------------------------------------------
// ElementOwner
//-----------------------------------------------------------------------------
ElementOwner::~ElementOwner()
{
	Clear();
}

void ElementOwner::Clear()
{
	for (auto pElement : *this) {
		Element::Delete(pElement);
	}
	clear();
}

class Element_Inst : public Element {
private:
	String _symbol;
public:
	inline Element_Inst(const String &symbol) : Element(TYPE_Inst), _symbol(symbol) {}
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	virtual String ToString() const;
};

String Element_Inst::ToString() const
{
	String str = _symbol;
	str += " ";
	str += GetChildren().ToString();
	return str;
}

class Element_A : public Element {
public:
	inline Element_A() : Element(TYPE_A) {}
	virtual String ToString() const;
};

String Element_A::ToString() const
{
	return "A";
}

class Element_B : public Element {
public:
	inline Element_B() : Element(TYPE_B) {}
	virtual String ToString() const;
};

String Element_B::ToString() const
{
	return "B";
}

class Element_X : public Element {
public:
	inline Element_X() : Element(TYPE_X) {}
	virtual String ToString() const;
};

String Element_X::ToString() const
{
	return "X";
}

class Element_Number : public Element {
private:
	UInt32 _num;
public:
	inline Element_Number(UInt32 num) : Element(TYPE_Number), _num(num) {}
	virtual String ToString() const;
};

String Element_Number::ToString() const
{
	char buff[128];
	::sprintf_s(buff, "$%x", _num);
	return buff;
}

class Element_Symbol : public Element {
private:
	String _symbol;
	UInt32 _num;
	bool _validNumFlag;
public:
	inline Element_Symbol(const String &symbol) :
		Element(TYPE_Symbol), _symbol(symbol), _num(0), _validNumFlag(false) {}
	inline void SetNumber(UInt32 num) { _num = num; _validNumFlag = true; }
	inline void InvalidateNumber() { _validNumFlag = false; }
	inline UInt32 GetNumber() const { return _num; }
	inline bool IsValidNumber() const { return _validNumFlag; }
	virtual String ToString() const;
};

String Element_Symbol::ToString() const
{
	return _symbol;
}

class Element_String : public Element {
private:
	String _str;
public:
	inline Element_String(const String &str) : Element(TYPE_String), _str(str) {}
	virtual String ToString() const;
};

String Element_String::ToString() const
{
	String str;
	str = "\"";
	str += _str;
	str += "\"";
	return str;
}

class Element_BinOp : public Element {
public:
	inline Element_BinOp(Element *pElementL, Element *pElementR) : Element(TYPE_BinOp) {
		GetChildren().push_back(pElementL);
		GetChildren().push_back(pElementR);
	}
	const Element *GetLeft() const { return GetChildren()[0]; }
	const Element *GetRight() const { return GetChildren()[1]; }
	virtual String ToString() const;
};

String Element_BinOp::ToString() const
{
	String str;
	str = GetLeft()->ToString();
	str += " + ";
	str += GetRight()->ToString();
	return str;
}

class Element_BinOp_Add : public Element_BinOp {
public:
	inline Element_BinOp_Add(Element *pElementL, Element *pElementR) :
		Element_BinOp(pElementL, pElementR) {}
};

class Element_BinOp_Sub : public Element_BinOp {
public:
	inline Element_BinOp_Sub(Element *pElementL, Element *pElementR) :
		Element_BinOp(pElementL, pElementR) {}
};

class Element_BinOp_Mul : public Element_BinOp {
public:
	inline Element_BinOp_Mul(Element *pElementL, Element *pElementR) :
		Element_BinOp(pElementL, pElementR) {}
};

class Element_BinOp_Div : public Element_BinOp {
public:
	inline Element_BinOp_Div(Element *pElementL, Element *pElementR) :
		Element_BinOp(pElementL, pElementR) {}
};

class Element_Bracket : public Element {
public:
	inline Element_Bracket() : Element(TYPE_Bracket) {}
	virtual String ToString() const;
};

String Element_Bracket::ToString() const
{
	String str;
	str = "[";
	str += GetChildren().ToString();
	str += "]";
	return str;
}

class Element_Parenthesis : public Element {
public:
	inline Element_Parenthesis() : Element(TYPE_Parenthesis) {}
	virtual String ToString() const;
};

String Element_Parenthesis::ToString() const
{
	String str;
	str = "(";
	str += GetChildren().ToString();
	str += ")";
	return str;
}

//-----------------------------------------------------------------------------
// Parser
//-----------------------------------------------------------------------------
class Parser : public Tokenizer::Listener {
private:
	enum Stat {
		STAT_LineTop,
		STAT_Label,
		STAT_Instruction,
		STAT_Operand,
	};
private:
	Tokenizer _tokenizer;
	Stat _stat;
	ElementOwner _elemOwner;
	ElementStack _elemStack;
public:
	Parser(const String &fileNameSrc);
	inline const ElementOwner &GetInstructions() const { return _elemOwner; }
	inline bool FeedChar(char ch) { return _tokenizer.FeedChar(ch); }
	inline const char *GetErrMsg() const { return _tokenizer.GetErrMsg(); }
	virtual bool FeedToken(const Token &token);
};

Parser::Parser(const String &fileNameSrc) : _tokenizer(this, fileNameSrc), _stat(STAT_LineTop)
{
}

bool Parser::FeedToken(const Token &token)
{
	bool rtn = true;
	//::printf("%s\n", token.ToString().c_str());
	switch (_stat) {
	case STAT_LineTop: {
		if (token.IsType(Token::TYPE_Symbol)) {
			_stat = STAT_Label;
		} else if (token.IsType(Token::TYPE_White)) {
			_stat = STAT_Instruction;
		}
		break;
	}
	case STAT_Label: {
		if (token.IsType(Token::TYPE_Colon)) {
			
		} else if (token.IsType(Token::TYPE_White)) {
			// nothing to do
		} else {
			_tokenizer.SetErrMsg("invalid format of label");
			rtn = false;
		}
		break;
	}
	case STAT_Instruction: {
		if (token.IsType(Token::TYPE_Symbol)) {
			Element *pElem = new Element_Inst(token.GetString());
			_elemOwner.push_back(pElem);
			_elemStack.push_back(pElem);
			_stat = STAT_Operand;
		} else {
			_tokenizer.SetErrMsg("instruction or pseudo command is expected");
			rtn = false;
		}
		break;
	}
	case STAT_Operand: {
		if (token.IsType(Token::TYPE_White)) {
			// nothing to do
		} else if (token.IsType(Token::TYPE_EOL)) {
			_elemStack.pop_back();
			_stat = STAT_LineTop;
		} else if (token.IsType(Token::TYPE_Comma)) {
			
		} else if (token.IsType(Token::TYPE_Symbol)) {
			if (token.MatchICase("a")) {
				_elemStack.back()->AddChild(new Element_A());
			} else if (token.MatchICase("b")) {
				_elemStack.back()->AddChild(new Element_B());
			} else if (token.MatchICase("x")) {
				_elemStack.back()->AddChild(new Element_X());
			} else {
				_elemStack.back()->AddChild(new Element_Symbol(token.GetString()));
			}
		} else if (token.IsType(Token::TYPE_Number)) {
			_elemStack.back()->AddChild(new Element_Number(token.GetNumber()));
		} else if (token.IsType(Token::TYPE_Plus)) {
			new Element_BinOp_Add(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Minus)) {
			new Element_BinOp_Sub(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Asterisk)) {
			new Element_BinOp_Mul(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Slash)) {
			new Element_BinOp_Div(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_BracketL)) {
			Element *pElem = new Element_Bracket();
			_elemStack.back()->AddChild(pElem);
			_elemStack.push_back(pElem);
		} else if (token.IsType(Token::TYPE_BracketR)) {
			if (!_elemStack.back()->IsType(Element::TYPE_Bracket)) {
				_tokenizer.SetErrMsg("no opening bracket matched");
			}
			_elemStack.pop_back();
		} else if (token.IsType(Token::TYPE_ParenthesisL)) {
			Element *pElem = new Element_Parenthesis();
			_elemStack.back()->AddChild(pElem);
			_elemStack.push_back(pElem);
		} else if (token.IsType(Token::TYPE_ParenthesisR)) {
			if (!_elemStack.back()->IsType(Element::TYPE_Parenthesis)) {
				_tokenizer.SetErrMsg("no opening parenthesis matched");
			}
			_elemStack.pop_back();
		} else {
			_tokenizer.SetErrMsg("invalid format of operands");
			rtn = false;
		}
		break;
	}
	}
	return rtn;
}

bool Parse(const char *fileName)
{
	FILE *fp;
	if (::fopen_s(&fp, fileName, "rt") != 0) {
		::fprintf(stderr, "failed to open file\n");
		::exit(1);
	}
	Parser parser(fileName);
	for (;;) {
		int chRaw = ::fgetc(fp);
		char ch = (chRaw < 0)? '\0' : static_cast<unsigned char>(chRaw);
		if (!parser.FeedChar(ch)) {
			::fprintf(stderr, "%s\n", parser.GetErrMsg());
		}
		if (ch == '\0') break;
	}
	::fclose(fp);
	parser.GetInstructions().Print();
	return true;
}

int main(int argc, char *argv[])
{

#if 1
	if (argc < 2) {
		::fprintf(stderr, "usage: jasm file\n");
		::exit(1);
	}
	::Parse(argv[1]);
#else
	FILE *fp = ::fopen("test.cjr", "wb");
	CJRFormat::Write(fp, "hello", 0x2000,
					 (const UInt8 *)"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f", 16);
	::fclose(fp);
#endif
	return 0;
}
