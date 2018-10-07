//=============================================================================
// Token.cpp
//=============================================================================
#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "Expr.h"

//-----------------------------------------------------------------------------
// TokenInfo
//-----------------------------------------------------------------------------
struct TokenInfo {
	int category;
	const char *name;
	const char *symbol;
	bool IsIdentical(const TokenInfo &tokenInfo) const { return this == &tokenInfo; }
};

extern const TokenInfo TOKEN_Expr;		// for parsing
extern const TokenInfo TOKEN_Empty;		// for parsing
extern const TokenInfo TOKEN_Eq;
extern const TokenInfo TOKEN_VBarVBar;
extern const TokenInfo TOKEN_AmpAmp;
extern const TokenInfo TOKEN_VBar;
extern const TokenInfo TOKEN_Hat;
extern const TokenInfo TOKEN_Amp;
extern const TokenInfo TOKEN_EqEq;
extern const TokenInfo TOKEN_BangEq;
extern const TokenInfo TOKEN_Lt;
extern const TokenInfo TOKEN_LtEq;
extern const TokenInfo TOKEN_Gt;
extern const TokenInfo TOKEN_GtEq;
extern const TokenInfo TOKEN_LtLt;
extern const TokenInfo TOKEN_GtGt;
extern const TokenInfo TOKEN_Plus;
extern const TokenInfo TOKEN_Minus;
extern const TokenInfo TOKEN_Asterisk;
extern const TokenInfo TOKEN_Slash;
extern const TokenInfo TOKEN_Percent;
extern const TokenInfo TOKEN_ParenL;
extern const TokenInfo TOKEN_ParenR;
extern const TokenInfo TOKEN_BracketL;
extern const TokenInfo TOKEN_BracketR;
extern const TokenInfo TOKEN_BraceL;
extern const TokenInfo TOKEN_BraceR;
extern const TokenInfo TOKEN_Number;
extern const TokenInfo TOKEN_Symbol;
extern const TokenInfo TOKEN_String;
extern const TokenInfo TOKEN_BitPattern;
extern const TokenInfo TOKEN_Bang;
extern const TokenInfo TOKEN_Comma;
extern const TokenInfo TOKEN_Colon;
extern const TokenInfo TOKEN_ColonColon;
extern const TokenInfo TOKEN_EOL;
extern const TokenInfo TOKEN_White;

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
	int _lineNo;
	String _str;
	Number _num;
	bool _validStrFlag;
	AutoPtr<Expr> _pExpr;	// only valid for TOKEN_Expr, TOKEN_BracketL and TOKEN_BraceL
private:
	static const Precedence _precMatrix[][17];
public:
	DeclareReferenceAccessor(Token);
public:
	inline Token(const TokenInfo &tokenInfo, int lineNo) :
		_cntRef(1), _pTokenInfo(&tokenInfo), _lineNo(lineNo), _num(0), _validStrFlag(false) {}
	inline Token(const TokenInfo &tokenInfo, int lineNo, Expr *pExpr) :
		_cntRef(1), _pTokenInfo(&tokenInfo), _lineNo(lineNo), _num(0), _validStrFlag(false), _pExpr(pExpr) {}
	inline Token(const TokenInfo &tokenInfo, int lineNo, const String &str) :
		_cntRef(1), _pTokenInfo(&tokenInfo), _lineNo(lineNo), _str(str), _num(0), _validStrFlag(true) {}
	inline Token(const TokenInfo &tokenInfo, int lineNo, const String &str, Number num) :
		_cntRef(1), _pTokenInfo(&tokenInfo), _lineNo(lineNo), _str(str), _num(num), _validStrFlag(true) {}
	inline Token(const TokenInfo &tokenInfo) :
		_cntRef(1), _pTokenInfo(&tokenInfo), _lineNo(0), _num(0), _validStrFlag(false) {}
	inline Token(Expr *pExpr) :
		_cntRef(1), _pTokenInfo(&TOKEN_Expr), _lineNo(0), _num(0), _validStrFlag(false), _pExpr(pExpr) {}
private:
	inline ~Token() {}
public:
	inline int GetCategory() const { return _pTokenInfo->category; }
	inline const char *GetName() const { return _pTokenInfo->name; }
	inline const char *GetSymbol() const { return _pTokenInfo->symbol; }
	inline bool HasPrecedence() const { return GetCategory() != 0; }
	inline bool IsType(const TokenInfo &tokenInfo) const { return _pTokenInfo->IsIdentical(tokenInfo); }
	inline int GetLineNo() const { return _lineNo; }
	inline Number GetNumber() const { return _num; }
	inline const char *GetString() const { return _str.c_str(); }
	inline const String &GetStringSTL() const { return _str; }
	inline const Expr *GetExpr() const { return _pExpr.get(); }
	inline bool MatchCase(const char *str) const { return ::strcmp(_str.c_str(), str) == 0; }
	inline bool MatchICase(const char *str) const { return ::strcasecmp(_str.c_str(), str) == 0; }
	inline static Precedence LookupPrec(const TokenInfo &tokenInfoL, const TokenInfo &tokenInfoR) {
		return _precMatrix[tokenInfoL.category][tokenInfoR.category];
	}
	inline static Precedence LookupPrec(const Token &tokenL, const Token &tokenR) {
		return _precMatrix[tokenL.GetCategory()][tokenR.GetCategory()];
	}
	String ToString() const;
};

//-----------------------------------------------------------------------------
// TokenStack
//-----------------------------------------------------------------------------
class TokenStack : public std::vector<Token *> {
public:
	TokenStack();
	~TokenStack();
	inline Token *Peek(int offset) { return *(rbegin() + offset); }
	inline void Push(Token *pToken) { push_back(pToken); }
	inline Token *Pop() { Token *pToken = back(); pop_back(); return pToken; }
	inline bool IsEmpty() const { return size() <= 1; }
	void Clear();
	void Reset();
	reverse_iterator SeekTerminal(reverse_iterator p);
	String ToString() const;
};

#endif
