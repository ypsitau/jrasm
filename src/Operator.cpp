//=============================================================================
// Operator.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Operator
//-----------------------------------------------------------------------------
const Operator *Operator::Add = nullptr;
const Operator *Operator::Sub = nullptr;
const Operator *Operator::Mul = nullptr;
const Operator *Operator::Div = nullptr;

void Operator::Initialize()
{
	Add = new Operator_Add();
	Sub = new Operator_Sub();
	Mul = new Operator_Mul();
	Div = new Operator_Div();
}

//-----------------------------------------------------------------------------
// Operator_Add
//-----------------------------------------------------------------------------
Expr *Operator_Add::Eval(const Expr *pExprL, const Expr *pExprR)
{
	
	return nullptr;
}

//-----------------------------------------------------------------------------
// Operator_Sub
//-----------------------------------------------------------------------------
Expr *Operator_Sub::Eval(const Expr *pExprL, const Expr *pExprR)
{
	return nullptr;
}

//-----------------------------------------------------------------------------
// Operator_Mul
//-----------------------------------------------------------------------------
Expr *Operator_Mul::Eval(const Expr *pExprL, const Expr *pExprR)
{
	return nullptr;
}

//-----------------------------------------------------------------------------
// Operator_Div
//-----------------------------------------------------------------------------
Expr *Operator_Div::Eval(const Expr *pExprL, const Expr *pExprR)
{
	return nullptr;
}
