//=============================================================================
// Token.cpp
//=============================================================================
#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// TokenInfo
//-----------------------------------------------------------------------------
struct TokenInfo {
	int category;
	const char *name;
	const char *symbol;
	bool IsIdentical(const TokenInfo &tokenInfo) const { return this == &tokenInfo; }
};

extern const TokenInfo TOKEN_Begin;
extern const TokenInfo TOKEN_EOL;
extern const TokenInfo TOKEN_Expr;
extern const TokenInfo TOKEN_White;
extern const TokenInfo TOKEN_Number;
extern const TokenInfo TOKEN_Symbol;
extern const TokenInfo TOKEN_String;
extern const TokenInfo TOKEN_Colon;
extern const TokenInfo TOKEN_Comma;
extern const TokenInfo TOKEN_Plus;
extern const TokenInfo TOKEN_Minus;
extern const TokenInfo TOKEN_Asterisk;
extern const TokenInfo TOKEN_Slash;
extern const TokenInfo TOKEN_BracketL;
extern const TokenInfo TOKEN_BracketR;
extern const TokenInfo TOKEN_ParenthesisL;
extern const TokenInfo TOKEN_ParenthesisR;

//-----------------------------------------------------------------------------
// Token
//-----------------------------------------------------------------------------
class Token {
public:
	enum Precedence {
		PREC_LT,
		PREC_EQ,
		PREC_GT,
		PREC_Error,
	};
private:
	int _cntRef;
	const TokenInfo *_pTokenInfo;
	String _str;
	UInt32 _num;
	bool _validStrFlag;
private:
	static const Precedence _precMatrix[][7];
public:
	DeclareReferenceAccessor(Token);
public:
	inline Token(const TokenInfo &tokenInfo) :
		_cntRef(1), _pTokenInfo(&tokenInfo), _num(0), _validStrFlag(false) {}
	inline Token(const TokenInfo &tokenInfo, const String &str) :
		_cntRef(1), _pTokenInfo(&tokenInfo), _str(str), _num(0), _validStrFlag(true) {}
	inline Token(const TokenInfo &tokenInfo, const String &str, UInt32 num) :
		_cntRef(1), _pTokenInfo(&tokenInfo), _str(str), _num(num), _validStrFlag(true) {}
private:
	inline ~Token() {}
public:
	inline int GetCategory() const { return _pTokenInfo->category; }
	inline const char *GetName() const { return _pTokenInfo->name; }
	inline const char *GetSymbol() const { return _pTokenInfo->symbol; }
	inline bool HasPrecedence() const { return GetCategory() != 0; }
	inline bool IsType(const TokenInfo &tokenInfo) const { return _pTokenInfo->IsIdentical(tokenInfo); }
	inline UInt32 GetNumber() const { return _num; }
	inline const char *GetString() const { return _str.c_str(); }
	inline bool MatchCase(const char *str) const { return ::strcmp(_str.c_str(), str) == 0; }
	inline bool MatchICase(const char *str) const { return ::strcasecmp(_str.c_str(), str) == 0; }
	inline static Precedence LookupPrec(const Token &tokenLeft, const Token &tokenRight) {
		return _precMatrix[tokenLeft.GetCategory()][tokenRight.GetCategory()];
	}
	String ToString() const;
};

//-----------------------------------------------------------------------------
// TokenStack
//-----------------------------------------------------------------------------
class TokenStack : public std::vector<Token *> {
public:
	~TokenStack();
	reverse_iterator SeekTerminal(reverse_iterator p);
	Token *Peek(int offset) { return *(rbegin() + offset); }
	void Clear();
	String ToString() const;
	inline bool IsEmpty() const { return size() <= 1; }
};

#endif
