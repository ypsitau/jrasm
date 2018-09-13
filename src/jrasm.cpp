//=============================================================================
// jrasm.cpp
//=============================================================================
#include "stdafx.h"
#include "jrasm.h"

//-----------------------------------------------------------------------------
// InstInfo
//-----------------------------------------------------------------------------
class InstInfo {
private:
	String _symbol;
public:
	InstInfo(const String &symbol);
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	static InstInfo *Syntax_ACC(const String &symbol, UInt8 codeACC);
	static InstInfo *Syntax_REL(const String &symbol, UInt8 codeREL);
	static InstInfo *Syntax_INH(const String &symbol, UInt8 codeINH);
	static InstInfo *Syntax_ACC_IMM_DIR_IDX_EXT(
		const String &symbol,
		UInt8 codeIMM_A, UInt8 codeDIR_A, UInt8 codeIDX_A, UInt8 codeEXT_A,
		UInt8 codeIMM_B, UInt8 codeDIR_B, UInt8 codeIDX_B, UInt8 codeEXT_B);
	static InstInfo *Syntax_DIR_IDX_IMM_EXT(
		const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeIMM, UInt8 codeEXT);
	static InstInfo *Syntax_ACC_ACC_IDX_EXT(
		const String &symbol, UInt8 codeACC_A, UInt8 codeACC_B, UInt8 codeIDX, UInt8 codeEXT);
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
	Add(InstInfo::Syntax_ACC_IMM_DIR_IDX_EXT("adc", 0x89, 0x99, 0xa9, 0xb9, 0xc9, 0xd9, 0xe9, 0xf9));
	Add(InstInfo::Syntax_ACC_IMM_DIR_IDX_EXT("add", 0x8b, 0x9b, 0xab, 0xbb, 0xcb, 0xdb, 0xeb, 0xfb));
	Add(InstInfo::Syntax_ACC_IMM_DIR_IDX_EXT("and", 0x84, 0x94, 0xa4, 0xb4, 0xc4, 0xd4, 0xe4, 0xf4));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("asl", 0x48, 0x58, 0x68, 0x78));
	Add(InstInfo::Syntax_ACC_ACC_IDX_EXT	("asr", 0x47, 0x57, 0x67, 0x77));
	Add(InstInfo::Syntax_REL				("bcc", 0x24));
	Add(InstInfo::Syntax_REL				("bcs", 0x25));
	Add(InstInfo::Syntax_REL				("beq", 0x27));
	Add(InstInfo::Syntax_REL				("bge", 0x2c));
	Add(InstInfo::Syntax_REL				("bgt", 0x2e));
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
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_REL(const String &symbol, UInt8 codeREL)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_INH(const String &symbol, UInt8 codeINH)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_ACC_IMM_DIR_IDX_EXT(
	const String &symbol,
	UInt8 codeIMM_A, UInt8 codeDIR_A, UInt8 codeIDX_A, UInt8 codeEXT_A,
	UInt8 codeIMM_B, UInt8 codeDIR_B, UInt8 codeIDX_B, UInt8 codeEXT_B)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_DIR_IDX_IMM_EXT(
	const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeIMM, UInt8 codeEXT)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	return pInstInfo;
}

InstInfo *InstInfo::Syntax_ACC_ACC_IDX_EXT(
	const String &symbol, UInt8 codeACC_A, UInt8 codeACC_B, UInt8 codeIDX, UInt8 codeEXT)
{
	InstInfo *pInstInfo = new InstInfo(symbol);
	return pInstInfo;
}

//-----------------------------------------------------------------------------
// Element
//-----------------------------------------------------------------------------
class Element {
public:
	enum Type {
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
public:
	DeclareReferenceAccessor(Element);
public:
	inline Element(Type type) : _cntRef(1), _type(type) {}
protected:
	virtual ~Element();
public:
	//virtual bool Reduce() = 0;
};

//-----------------------------------------------------------------------------
// ElementList
//-----------------------------------------------------------------------------
typedef std::vector<Element *> ElementList;

//-----------------------------------------------------------------------------
// ElementOwner
//-----------------------------------------------------------------------------
class ElementOwner : public ElementList {
public:
	~ElementOwner();
	void Clear();
};

//-----------------------------------------------------------------------------
// Element
//-----------------------------------------------------------------------------
Element::~Element()
{
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

class Element_A : public Element {
public:
	inline Element_A() : Element(TYPE_A) {}
};

class Element_B : public Element {
public:
	inline Element_B() : Element(TYPE_B) {}
};

class Element_X : public Element {
public:
	inline Element_X() : Element(TYPE_X) {}
};

class Element_Number : public Element {
private:
	UInt32 _num;
public:
	inline Element_Number(UInt32 num) : Element(TYPE_Number), _num(num) {}
};

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
};

class Element_String : public Element {
private:
	String _str;
public:
	inline Element_String(const String &str) : Element(TYPE_String), _str(str) {}
};

class Element_BinOp : public Element {
private:
	AutoPtr<Element> _pElementL;
	AutoPtr<Element> _pElementR;
public:
	inline Element_BinOp(Element *pElementL, Element *pElementR) :
		Element(TYPE_BinOp), _pElementL(pElementL), _pElementR(pElementR) {}
};

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
private:
	AutoPtr<Element> _pElementChild;
public:
	inline Element_Bracket(Element *pElementChild) :
		Element(TYPE_Bracket), _pElementChild(pElementChild) {}
};

class Element_Parenthesis : public Element {
private:
	AutoPtr<Element> _pElementChild;
public:
	inline Element_Parenthesis(Element *pElementChild) :
		Element(TYPE_Parenthesis), _pElementChild(pElementChild) {}
};

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
		STAT_Bracket,
		STAT_Parenthesis,
	};
private:
	Tokenizer _tokenizer;
	Stat _stat;
	ElementOwner _elemOwner;
public:
	Parser(const String &fileNameSrc);
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
			
			::printf("%s\n", token.ToString().c_str());
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
			
		} else if (token.IsType(Token::TYPE_Symbol)) {
			if (token.MatchICase("a")) {
				_elemOwner.push_back(new Element_A());
			} else if (token.MatchICase("b")) {
				_elemOwner.push_back(new Element_B());
			} else if (token.MatchICase("x")) {
				_elemOwner.push_back(new Element_X());
			} else {
				_elemOwner.push_back(new Element_Symbol(token.GetString()));
			}
		} else if (token.IsType(Token::TYPE_Number)) {
			_elemOwner.push_back(new Element_Number(token.GetNumber()));
		} else if (token.IsType(Token::TYPE_Plus)) {
			new Element_BinOp_Add(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Minus)) {
			new Element_BinOp_Sub(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Asterisk)) {
			new Element_BinOp_Mul(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_Slash)) {
			new Element_BinOp_Div(nullptr, nullptr);
		} else if (token.IsType(Token::TYPE_BracketL)) {
			_stat = STAT_Bracket;
		} else if (token.IsType(Token::TYPE_ParenthesisL)) {
			_stat = STAT_Parenthesis;
		} else {
			_tokenizer.SetErrMsg("invalid format of operands");
			rtn = false;
		}
		break;
	}
	case STAT_Bracket: {
		if (token.IsType(Token::TYPE_BracketR)) {
			_stat = STAT_Operand;
		}
		break;
	}
	case STAT_Parenthesis: {
		if (token.IsType(Token::TYPE_ParenthesisR)) {
			_stat = STAT_Operand;
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
