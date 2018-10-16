//=============================================================================
// Operator.h
//=============================================================================
#ifndef __JRASMCORE_OPERATOR_H__
#define __JRASMCORE_OPERATOR_H__

#include "Token.h"

class Expr;
class Context;

//-----------------------------------------------------------------------------
// Operator
//-----------------------------------------------------------------------------
class Operator {
public:
	static const Operator *Add;
	static const Operator *Sub;
	static const Operator *AddInj;
	static const Operator *Mul;
	static const Operator *Div;
	static const Operator *Mod;
	static const Operator *LogicOr;
	static const Operator *LogicAnd;
	static const Operator *Or;
	static const Operator *Xor;
	static const Operator *And;
	static const Operator *Eq;
	static const Operator *NotEq;
	static const Operator *Lt;
	static const Operator *Le;
	static const Operator *Gt;
	static const Operator *Ge;
	static const Operator *ShiftL;
	static const Operator *ShiftR;
	static const Operator *FieldSep;
private:
	const TokenInfo &_tokenInfo;
public:
	inline Operator(const TokenInfo &tokenInfo) : _tokenInfo(tokenInfo) {}
	static void Initialize();
	inline const TokenInfo &GetTokenInfo() const { return _tokenInfo; }
	inline const char *GetSymbol() const { return _tokenInfo.symbol; }
	inline bool IsIdentical(const Operator *pOperator) const { return this == pOperator; }
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const = 0;
	inline static Token::Precedence LookupPrec(const Operator *pOperator1, const Operator *pOperator2) {
		return Token::LookupPrec(pOperator1->GetTokenInfo(), pOperator2->GetTokenInfo());
	}
};

//-----------------------------------------------------------------------------
// Operator_Add
//-----------------------------------------------------------------------------
class Operator_Add : public Operator {
public:
	inline Operator_Add() : Operator(TOKEN_Plus) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Sub
//-----------------------------------------------------------------------------
class Operator_Sub : public Operator {
public:
	inline Operator_Sub() : Operator(TOKEN_Minus) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_AddInj
//-----------------------------------------------------------------------------
class Operator_AddInj : public Operator {
public:
	inline Operator_AddInj() : Operator(TOKEN_LtPlus) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Mul
//-----------------------------------------------------------------------------
class Operator_Mul : public Operator {
public:
	inline Operator_Mul() : Operator(TOKEN_Asterisk) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Div
//-----------------------------------------------------------------------------
class Operator_Div : public Operator {
public:
	inline Operator_Div() : Operator(TOKEN_Slash) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Mod
//-----------------------------------------------------------------------------
class Operator_Mod : public Operator {
public:
	inline Operator_Mod() : Operator(TOKEN_Percent) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_LogicOr
//-----------------------------------------------------------------------------
class Operator_LogicOr : public Operator {
public:
	inline Operator_LogicOr() : Operator(TOKEN_VBarVBar) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_LogicAnd
//-----------------------------------------------------------------------------
class Operator_LogicAnd : public Operator {
public:
	inline Operator_LogicAnd() : Operator(TOKEN_AmpAmp) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Or
//-----------------------------------------------------------------------------
class Operator_Or : public Operator {
public:
	inline Operator_Or() : Operator(TOKEN_VBar) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Xor
//-----------------------------------------------------------------------------
class Operator_Xor : public Operator {
public:
	inline Operator_Xor() : Operator(TOKEN_Hat) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_And
//-----------------------------------------------------------------------------
class Operator_And : public Operator {
public:
	inline Operator_And() : Operator(TOKEN_Amp) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Eq
//-----------------------------------------------------------------------------
class Operator_Eq : public Operator {
public:
	inline Operator_Eq() : Operator(TOKEN_EqEq) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_NotEq
//-----------------------------------------------------------------------------
class Operator_NotEq : public Operator {
public:
	inline Operator_NotEq() : Operator(TOKEN_BangEq) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Lt
//-----------------------------------------------------------------------------
class Operator_Lt : public Operator {
public:
	inline Operator_Lt() : Operator(TOKEN_Lt) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Le
//-----------------------------------------------------------------------------
class Operator_Le : public Operator {
public:
	inline Operator_Le() : Operator(TOKEN_LtEq) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Gt
//-----------------------------------------------------------------------------
class Operator_Gt : public Operator {
public:
	inline Operator_Gt() : Operator(TOKEN_Gt) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Ge
//-----------------------------------------------------------------------------
class Operator_Ge : public Operator {
public:
	inline Operator_Ge() : Operator(TOKEN_GtEq) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_ShiftL
//-----------------------------------------------------------------------------
class Operator_ShiftL : public Operator {
public:
	inline Operator_ShiftL() : Operator(TOKEN_LtLt) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_ShiftR
//-----------------------------------------------------------------------------
class Operator_ShiftR : public Operator {
public:
	inline Operator_ShiftR() : Operator(TOKEN_GtGt) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_FieldSep
//-----------------------------------------------------------------------------
class Operator_FieldSep : public Operator {
public:
	inline Operator_FieldSep() : Operator(TOKEN_Colon) {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

#endif
