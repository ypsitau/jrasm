//=============================================================================
// Tokenizer.h
//=============================================================================
#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include "Token.h"

//-----------------------------------------------------------------------------
// Tokenizer
//-----------------------------------------------------------------------------
class Tokenizer {
public:
	class Listener {
	public:
		virtual bool FeedToken(AutoPtr<Token> pToken) = 0;
	};
	enum Stat {
		STAT_LineTop,
		STAT_Neutral,
		STAT_White,
		STAT_Comment,
		STAT_DetectZero,
		STAT_OctNumber,
		STAT_DecNumber,
		STAT_HexNumber,
		STAT_Symbol,
		STAT_String,
		STAT_StringEsc,
	};
private:
	Stat _stat;
	Listener *_pListener;
	String _fileNameSrc;
	String _errMsg;
	UInt16 _num;
	String _str;
	int _nLines;
public:
	Tokenizer(Listener *pListener, const String &fileNameSrc);
	bool FeedChar(char ch);
	void SetError(const char *format, ...);
	inline const char *GetErrMsg() const { return _errMsg.c_str(); }
private:
	bool FeedToken(const TokenInfo &tokenInfo);
	bool FeedToken(const TokenInfo &tokenInfo, const String &str);
	bool FeedToken(const TokenInfo &tokenInfo, const String &str, UInt32 num);
	inline static bool IsEOF(char ch) { return ch == '\0'; }
	inline static bool IsEOL(char ch) { return ch == '\n'; }
	inline static bool IsWhite(char ch) { return ch == ' ' || ch == '\t'; }
	inline static bool IsSymbolFirst(char ch) {
		return ::isalpha(ch) || ch == '@' || ch == '.' || ch == '$';
	}
	inline static bool IsSymbolFollow(char ch) { return IsSymbolFirst(ch) || ::isdigit(ch); }
	inline static bool IsDigit(char ch) { return ::isdigit(ch); }
};

#endif
