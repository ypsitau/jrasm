//=============================================================================
// jrasm.cpp
//=============================================================================
#include "jrasm.h"

class Token {
public:
	enum Type {
		TYPE_None,
		TYPE_NewLine,
		TYPE_White,
		TYPE_Number,
		TYPE_Symbol,
		TYPE_Comma,
		TYPE_Plus,
		TYPE_Minus,
		TYPE_Asterisk,
		TYPE_Slash,
		TYPE_BracketL,
		TYPE_BracketR,
		TYPE_ParenthesisL,
		TYPE_ParenthesisR,
	};
	struct TypeInfo {
		Type type;
		const char *name;
	};
private:
	Type _type;
	String _str;
	UInt32 _num;
	bool _validStrFlag;
public:
	inline Token() : _type(TYPE_None), _num(0), _validStrFlag(false) {}
	inline void Set(Type type) {
		_type = type;
		_validStrFlag = false;
	}
	inline void Set(Type type, const String &str, UInt32 num) {
		_type = type, _str = str, _num = num;
		_validStrFlag = true;
	}
	inline void Set(Type type, const String &str) {
		_type = type, _str = str;
		_validStrFlag = true;
	}
	inline UInt32 GetNumber() const { return _num; }
	inline const char *GetString() const { return _str.c_str(); }
	String ToString() const;
};

String Token::ToString() const
{
	static const TypeInfo typeInfoTbl[] = {
		{ TYPE_NewLine,			"Newline"	},
		{ TYPE_White,			"White"		},
		{ TYPE_Number,			"Number"	},
		{ TYPE_Symbol,			"Symbol"	},
		{ TYPE_Comma,			"Comma"		},
		{ TYPE_Plus,			"Plus"		},
		{ TYPE_Minus,			"Minus"		},
		{ TYPE_Asterisk,		"Asterisk"	},
		{ TYPE_Slash,			"Slash"		},
		{ TYPE_BracketL,		"BrkL"		},
		{ TYPE_BracketR,		"BrkR"		},
		{ TYPE_ParenthesisL,	"ParL"		},
		{ TYPE_ParenthesisR,	"ParR"		},
	};
	String str;
	for (int i = 0; i < ArraySizeOf(typeInfoTbl); i++) {
		if (_type == typeInfoTbl[i].type) {
			str = typeInfoTbl[i].name;
			if (_validStrFlag) {
				str += " ";
				str += _str;
			}
			break;
		}
	}
	return str;
}

class Tokenizer {
public:
	enum Stat {
		STAT_LineTop,
		STAT_Neutral,
		STAT_White,
		STAT_Comment,
		STAT_DecNumber,
		STAT_HexNumber,
		STAT_Symbol,
	};
	enum Result {
		RESULT_None,
		RESULT_Error,
		RESULT_Formed,
	};
private:
	String _errMsg;
	Stat _stat;
	UInt16 _num;
	String _str;
	Token _token;
	DeclarePushback(char);
public:
	Tokenizer();
	Result FeedChar(char ch);
	void FormatErrMsg(const char *format, ...);
	inline const Token &GetToken() const { return _token; }
	inline const char *GetErrMsg() const { return _errMsg.c_str(); }
	inline static bool IsEOF(char ch) { return ch == '\0'; }
	inline static bool IsEOL(char ch) { return ch == '\n'; }
	inline static bool IsWhite(char ch) { return ch == ' ' || ch == '\t'; }
	inline static bool IsSymbolFirst(char ch) { return ::isalpha(ch); }
	inline static bool IsSymbolFollow(char ch) { return IsSymbolFirst(ch) || ::isdigit(ch); }
	inline static bool IsDigit(char ch) { return ::isdigit(ch); }
};

Tokenizer::Tokenizer() : _stat(STAT_LineTop), _num(0)
{
}

Tokenizer::Result Tokenizer::FeedChar(char ch)
{
	Result result = RESULT_None;
	BeginPushbackRegion(ch);
	::printf("%c .. %d\n", ch, _stat);
	switch (_stat) {
	case STAT_LineTop: {
		if (IsEOF(ch) || IsEOL(ch)) {
			// nothing to do
		} else {
			_stat = STAT_Neutral;
			Pushback(ch);
		}
		break;
	}
	case STAT_Neutral: {
		if (IsEOF(ch) || IsEOL(ch)) {
			_stat = STAT_LineTop;
			_token.Set(Token::TYPE_NewLine);
			return RESULT_Formed;
		} else if (IsWhite(ch)) {
			_stat = STAT_White;
		} else if (IsSymbolFirst(ch)) {
			_str.clear();
			_str += ch;
			_stat = STAT_Symbol;
		} else if (IsDigit(ch)) {
			_num = 0;
			_str.clear();
			_stat = STAT_DecNumber;
			Pushback(ch);
		} else if (ch == '$') {
			_num = 0;
			_str.clear();
			_str += '$';
			_stat = STAT_HexNumber;
		} else if (ch == ';') {
			_stat = STAT_Comment;
		} else if (ch == ',') {
			_token.Set(Token::TYPE_Comma);
			return RESULT_Formed;
		} else if (ch == '+') {
			_token.Set(Token::TYPE_Plus);
			return RESULT_Formed;
		} else if (ch == '-') {
			_token.Set(Token::TYPE_Minus);
			return RESULT_Formed;
		} else if (ch == '*') {
			_token.Set(Token::TYPE_Asterisk);
			return RESULT_Formed;
		} else if (ch == '/') {
			_token.Set(Token::TYPE_Slash);
			return RESULT_Formed;
		} else if (ch == '[') {
			_token.Set(Token::TYPE_BracketL);
			return RESULT_Formed;
		} else if (ch == ']') {
			_token.Set(Token::TYPE_BracketR);
			return RESULT_Formed;
		} else if (ch == '(') {
			_token.Set(Token::TYPE_ParenthesisL);
			return RESULT_Formed;
		} else if (ch == ')') {
			_token.Set(Token::TYPE_ParenthesisR);
			return RESULT_Formed;
		} else {
			if (::isprint(ch)) {
				FormatErrMsg("invalid character: %c", ch);
			} else {
				FormatErrMsg("invalid character: 0x%02x", ch);
			}
			return RESULT_Error;
		}
		break;
	}
	case STAT_White: {
		if (IsWhite(ch)) {
			// nothing to do
		} else {
			_stat = STAT_Neutral;
			Pushback(ch);
			_token.Set(Token::TYPE_White);
			return RESULT_Formed;
		}
		break;
	}
	case STAT_Comment: {
		if (IsEOF(ch) || IsEOL(ch)) {
			_stat = STAT_LineTop;
			_token.Set(Token::TYPE_NewLine);
			return RESULT_Formed;
		} else {
			// nothing to do
		}
		break;
	}
	case STAT_Symbol: {
		if (IsSymbolFollow(ch)) {
			_str += ch;
		} else {
			_stat = STAT_Neutral;
			Pushback(ch);
			_token.Set(Token::TYPE_Symbol, _str);
			return RESULT_Formed;
		}
		break;
	}
	case STAT_DecNumber: {
		if (IsDigit(ch)) {
			_str += ch;
			_num += ch - '0';
		} else {
			_stat = STAT_Neutral;
			Pushback(ch);
			_token.Set(Token::TYPE_Number, _str, _num);
			return RESULT_Formed;
		}
		break;
	}
	case STAT_HexNumber: {
		if (IsDigit(ch)) {
			_str += ch;
			_num += ch - '0';
		} else if ('a' <= ch && ch <= 'f') {
			_str += ch;
			_num += ch - 'a' + 10;
		} else if ('A' <= ch && ch <= 'F') {
			_str += ch;
			_num += ch - 'A' + 10;
		} else {
			_stat = STAT_Neutral;
			Pushback(ch);
			_token.Set(Token::TYPE_Number, _str, _num);
			return RESULT_Formed;
		}
		break;
	}
	}
	EndPushbackRegion();
	return result;
}

void Tokenizer::FormatErrMsg(const char *format, ...)
{
	char buff[256];
	va_list ap;
	va_start(ap, format);
	::vsprintf(buff, format, ap);
	_errMsg = buff;
}

bool Parse(const char *fileName)
{
	FILE *fp;
	fp = ::fopen(fileName, "rt");
	Tokenizer tokenizer;
	for (;;) {
		int chRaw = ::fgetc(fp);
		char ch = (chRaw < 0)? '\0' : static_cast<unsigned char>(chRaw);
		Tokenizer::Result result = tokenizer.FeedChar(ch);
		if (result == Tokenizer::RESULT_Error) {
			::fprintf(stderr, "%s\n", tokenizer.GetErrMsg());
		} else if (result == Tokenizer::RESULT_Formed) {
			::printf("%s\n", tokenizer.GetToken().ToString().c_str());
		}
		if (ch == '\0') break;
	}
	::fclose(fp);
	return true;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		::fprintf(stderr, "usage: jasm file");
		::exit(1);
	}
	::Parse(argv[1]);
	return 0;
}
