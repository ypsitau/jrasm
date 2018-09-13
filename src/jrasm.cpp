//=============================================================================
// jrasm.cpp
//=============================================================================
#include "stdafx.h"

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
			_elemStack.back()->AddChild(new Element_Symbol(token.GetString()));
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
	const ElementList &elemList = parser.GetInstructions();
	//elemList.Print();
	for (auto pElem : elemList) {
		if (!pElem->IsType(Element::TYPE_Inst)) continue;
		const Element_Inst *pElemEx = dynamic_cast<const Element_Inst *>(pElem);
		const InstInfo *pInstInfo = InstInfo::Lookup(pElemEx->GetSymbol());
		if (pInstInfo == nullptr) {
			::printf("unknown instruction: %s\n", pElemEx->GetSymbol());
		} else {
			::printf("%s .. ", pElemEx->ToString().c_str());
		}
		Context context;
		pInstInfo->ApplyRule(context, pElemEx->GetOperands());
		for (auto data : context.GetBuffer()) {
			::printf(" %02x", static_cast<UInt8>(data));
		}
		:: printf("\n");
	}
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
