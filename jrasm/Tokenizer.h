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
		STAT_Minus,
		STAT_Colon,
		STAT_Equal,
		STAT_VerticalBar,
		STAT_Ampersand,
		STAT_LessThan,
		STAT_GreaterThan,
		STAT_Bang,
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
	AutoPtr<StringShared> _pPathNameSrc;
	bool _numNegFlag;
	Number _num;
	String _str;
	int _nLines;
	char _quotedType;
public:
	Tokenizer(Listener *pListener, const String &pathNameSrc);
	inline const char *GetPathNameSrc() const { return _pPathNameSrc->GetString(); }
	inline const StringShared *GetPathNameSrcShared() const { return _pPathNameSrc.get(); }
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
	bool FeedToken(const TokenInfo &tokenInfo, const String &str, Number num);
};

#endif
