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
		STAT_Colon,
		STAT_DetectZero,
		STAT_BinNumber,
		STAT_OctNumber,
		STAT_DecNumber,
		STAT_HexNumber,
		STAT_Symbol,
		STAT_BitPatternPre,
		STAT_Quoted,
		STAT_QuotedEsc,
	};
	enum QuotedType {
		QUOTEDTYPE_None,
		QUOTEDTYPE_String,
		QUOTEDTYPE_Character,
		QUOTEDTYPE_BitPattern,
	};
private:
	Stat _stat;
	Listener *_pListener;
	AutoPtr<StringShared> _pFileNameSrc;
	UInt16 _num;
	String _str;
	int _nLines;
	char _quotedType;
public:
	Tokenizer(Listener *pListener, const String &fileNameSrc);
	inline const char *GetFileNameSrc() const { return _pFileNameSrc->GetString(); }
	inline const StringShared *GetFileNameSrcShared() const { return _pFileNameSrc.get(); }
	bool FeedChar(char ch);
	void AddError(const char *format, ...) const;
	void AddErrorV(const char *format, va_list ap) const;
private:
	inline static bool IsEOF(char ch) { return ch == '\0'; }
	inline static bool IsEOL(char ch) { return ch == '\n'; }
	inline static bool IsWhite(char ch) { return ch == ' ' || ch == '\t'; }
	inline static bool IsSymbolFirst(char ch) {
		return ::isalpha(ch) || ch == '@' || ch == '.' || ch == '$' || ch == '_';
	}
	inline static bool IsSymbolFollow(char ch) { return IsSymbolFirst(ch) || ::isdigit(ch); }
	inline static bool IsDigit(char ch) { return ::isdigit(ch); }
	bool FeedToken(const TokenInfo &tokenInfo);
	bool FeedToken(const TokenInfo &tokenInfo, const String &str);
	bool FeedToken(const TokenInfo &tokenInfo, const String &str, UInt32 num);
};

#endif
