//=============================================================================
// Tokenizer.h
//=============================================================================
#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include "Token.h"

class Tokenizer {
public:
	class Listener {
	public:
		virtual bool FeedToken(const Token &token) = 0;
	};
	enum Stat {
		STAT_LineTop,
		STAT_Neutral,
		STAT_White,
		STAT_Comment,
		STAT_DecNumber,
		STAT_HexNumber,
		STAT_Symbol,
	};
private:
	Listener *_pListener;
	String _errMsg;
	Stat _stat;
	UInt16 _num;
	String _str;
	Token _token;
public:
	Tokenizer(Listener *pListener);
	bool FeedChar(char ch);
	inline const char *GetErrMsg() const { return _errMsg.c_str(); }
private:
	bool CompleteToken(Token::Type type);
	bool CompleteToken(Token::Type type, const String &str);
	bool CompleteToken(Token::Type type, const String &str, UInt32 num);
	void FormatErrMsg(const char *format, ...);
	inline static bool IsEOF(char ch) { return ch == '\0'; }
	inline static bool IsEOL(char ch) { return ch == '\n'; }
	inline static bool IsWhite(char ch) { return ch == ' ' || ch == '\t'; }
	inline static bool IsSymbolFirst(char ch) { return ::isalpha(ch); }
	inline static bool IsSymbolFollow(char ch) { return IsSymbolFirst(ch) || ::isdigit(ch); }
	inline static bool IsDigit(char ch) { return ::isdigit(ch); }
};

#endif
