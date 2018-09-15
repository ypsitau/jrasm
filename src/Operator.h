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
public:
	inline Operator() {}
	static void Initialize();
	virtual Expr *Eval(const Expr *pExprL, const Expr *pExprR) = 0;
};

//-----------------------------------------------------------------------------
// Operator_Add
//-----------------------------------------------------------------------------
class Operator_Add : public Operator {
public:
	virtual Expr *Eval(const Expr *pExprL, const Expr *pExprR);
};

//-----------------------------------------------------------------------------
// Operator_Sub
//-----------------------------------------------------------------------------
class Operator_Sub : public Operator {
public:
	virtual Expr *Eval(const Expr *pExprL, const Expr *pExprR);
};

//-----------------------------------------------------------------------------
// Operator_Mul
//-----------------------------------------------------------------------------
class Operator_Mul : public Operator {
public:
	virtual Expr *Eval(const Expr *pExprL, const Expr *pExprR);
};

//-----------------------------------------------------------------------------
// Operator_Div
//-----------------------------------------------------------------------------
class Operator_Div : public Operator {
public:
	virtual Expr *Eval(const Expr *pExprL, const Expr *pExprR);
};

#endif
