//=============================================================================
// Operator.h
//=============================================================================
#ifndef __OPERATOR_H__
#define __OPERATOR_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// Operator
//-----------------------------------------------------------------------------
class Operator {
public:
	static const Operator *Add;
	static const Operator *Sub;
	static const Operator *Mul;
	static const Operator *Div;
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

#endif
