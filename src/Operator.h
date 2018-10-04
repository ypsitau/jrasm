//=============================================================================
// Operator.h
//=============================================================================
#ifndef __OPERATOR_H__
#define __OPERATOR_H__

#include "Common.h"

class Expr;
class Context;

//-----------------------------------------------------------------------------
// Operator
//-----------------------------------------------------------------------------
class Operator {
public:
	static const Operator *Add;
	static const Operator *Sub;
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
private:
	String _symbol;
public:
	inline Operator(const String &symbol) : _symbol(symbol) {}
	static void Initialize();
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline bool IsIdentical(const Operator *pOperator) const { return this == pOperator; }
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const = 0;
};

//-----------------------------------------------------------------------------
// Operator_Add
//-----------------------------------------------------------------------------
class Operator_Add : public Operator {
public:
	inline Operator_Add() : Operator("+") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Sub
//-----------------------------------------------------------------------------
class Operator_Sub : public Operator {
public:
	inline Operator_Sub() : Operator("-") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Mul
//-----------------------------------------------------------------------------
class Operator_Mul : public Operator {
public:
	inline Operator_Mul() : Operator("*") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Div
//-----------------------------------------------------------------------------
class Operator_Div : public Operator {
public:
	inline Operator_Div() : Operator("/") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Mod
//-----------------------------------------------------------------------------
class Operator_Mod : public Operator {
public:
	inline Operator_Mod() : Operator("%") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_LogicOr
//-----------------------------------------------------------------------------
class Operator_LogicOr : public Operator {
public:
	inline Operator_LogicOr() : Operator("||") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_LogicAnd
//-----------------------------------------------------------------------------
class Operator_LogicAnd : public Operator {
public:
	inline Operator_LogicAnd() : Operator("&&") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Or
//-----------------------------------------------------------------------------
class Operator_Or : public Operator {
public:
	inline Operator_Or() : Operator("|") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Xor
//-----------------------------------------------------------------------------
class Operator_Xor : public Operator {
public:
	inline Operator_Xor() : Operator("^") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_And
//-----------------------------------------------------------------------------
class Operator_And : public Operator {
public:
	inline Operator_And() : Operator("&") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Eq
//-----------------------------------------------------------------------------
class Operator_Eq : public Operator {
public:
	inline Operator_Eq() : Operator("==") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_NotEq
//-----------------------------------------------------------------------------
class Operator_NotEq : public Operator {
public:
	inline Operator_NotEq() : Operator("!=") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Lt
//-----------------------------------------------------------------------------
class Operator_Lt : public Operator {
public:
	inline Operator_Lt() : Operator("<") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Le
//-----------------------------------------------------------------------------
class Operator_Le : public Operator {
public:
	inline Operator_Le() : Operator("<=") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Gt
//-----------------------------------------------------------------------------
class Operator_Gt : public Operator {
public:
	inline Operator_Gt() : Operator(">") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_Ge
//-----------------------------------------------------------------------------
class Operator_Ge : public Operator {
public:
	inline Operator_Ge() : Operator(">=") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_ShiftL
//-----------------------------------------------------------------------------
class Operator_ShiftL : public Operator {
public:
	inline Operator_ShiftL() : Operator("<<") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

//-----------------------------------------------------------------------------
// Operator_ShiftR
//-----------------------------------------------------------------------------
class Operator_ShiftR : public Operator {
public:
	inline Operator_ShiftR() : Operator(">>") {}
	virtual Expr *Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const;
};

#endif
