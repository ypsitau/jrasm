//=============================================================================
// Operator.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Operator
//-----------------------------------------------------------------------------
const Operator *Operator::Add		= nullptr;
const Operator *Operator::Sub		= nullptr;
const Operator *Operator::AddInj	= nullptr;
const Operator *Operator::Mul		= nullptr;
const Operator *Operator::Div		= nullptr;
const Operator *Operator::Mod		= nullptr;
const Operator *Operator::LogicOr	= nullptr;
const Operator *Operator::LogicAnd	= nullptr;
const Operator *Operator::Or		= nullptr;
const Operator *Operator::Xor		= nullptr;
const Operator *Operator::And		= nullptr;
const Operator *Operator::Eq		= nullptr;
const Operator *Operator::NotEq		= nullptr;
const Operator *Operator::Lt		= nullptr;
const Operator *Operator::Le		= nullptr;
const Operator *Operator::Gt		= nullptr;
const Operator *Operator::Ge		= nullptr;
const Operator *Operator::ShiftL	= nullptr;
const Operator *Operator::ShiftR	= nullptr;

void Operator::Initialize()
{
	Add			= new Operator_Add();
	Sub			= new Operator_Sub();
	AddInj		= new Operator_AddInj();
	Mul			= new Operator_Mul();
	Div			= new Operator_Div();
	Mod			= new Operator_Mod();
	LogicOr		= new Operator_LogicOr();
	LogicAnd	= new Operator_LogicAnd();
	Or			= new Operator_Or();
	Xor			= new Operator_Xor();
	And			= new Operator_And();
	Eq			= new Operator_Eq();
	NotEq		= new Operator_NotEq();
	Lt			= new Operator_Lt();
	Le			= new Operator_Le();
	Gt			= new Operator_Gt();
	Ge			= new Operator_Ge();
	ShiftL		= new Operator_ShiftL();
	ShiftR		= new Operator_ShiftR();
}

//-----------------------------------------------------------------------------
// Operator_Add
//-----------------------------------------------------------------------------
Expr *Operator_Add::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL + numR);
	} else if (pExprL->IsTypeBinOp(Operator::Add) && pExprR->IsTypeInteger()) {
		const Expr_BinOp *pExprBinOp = dynamic_cast<Expr_BinOp *>(pExprL.get());
		if (pExprBinOp->GetLeft()->IsTypeInteger()) {
			Integer numL = dynamic_cast<const Expr_Integer *>(pExprBinOp->GetLeft())->GetInteger();
			Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
			return new Expr_BinOp(
				Operator::Add, new Expr_Integer(numL + numR), pExprBinOp->GetRight()->Reference());
		}
	} else if (pExprL->IsTypeSymbol() && pExprR->IsTypeInteger()) {
		return new Expr_BinOp(Operator::Add, pExprR.release(), pExprL.release());
	}
	return new Expr_BinOp(Operator::Add, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Sub
//-----------------------------------------------------------------------------
Expr *Operator_Sub::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL - numR);
	}
	return new Expr_BinOp(Operator::Sub, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_AddInj
//-----------------------------------------------------------------------------
Expr *Operator_AddInj::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeBracket() && pExprR->IsTypeInteger()) {
		const ExprList &exprOperands = dynamic_cast<const Expr_Bracket *>(pExprL.get())->GetExprOperands();
		if (exprOperands.size() != 1) {
			ErrorLog::AddError(pExprL.get(), "the bracket must contain one element");
			return nullptr;
		}
		AutoPtr<Expr> pExprGen(new Expr_BinOp(Operator::Add, exprOperands.front()->Reference(), pExprR.release()));
		return pExprGen->Resolve(context);
	}
	return new Expr_BinOp(Operator::AddInj, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Mul
//-----------------------------------------------------------------------------
Expr *Operator_Mul::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL * numR);
	}
	return new Expr_BinOp(Operator::Mul, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Div
//-----------------------------------------------------------------------------
Expr *Operator_Div::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		if (numR == 0) numR = 1;
		//if (numR == 0) {
		//	ErrorLog::AddError("divided by zero");
		//	return nullptr;
		//}
		return new Expr_Integer(numL / numR);
	}
	return new Expr_BinOp(Operator::Div, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Mod
//-----------------------------------------------------------------------------
Expr *Operator_Mod::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		if (numR == 0) numR = 1;
		return new Expr_Integer(numL % numR);
	}
	return new Expr_BinOp(Operator::Mod, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_LogicOr
//-----------------------------------------------------------------------------
Expr *Operator_LogicOr::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL || numR);
	}
	return new Expr_BinOp(Operator::LogicOr, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_LogicAnd
//-----------------------------------------------------------------------------
Expr *Operator_LogicAnd::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL && numR);
	}
	return new Expr_BinOp(Operator::LogicAnd, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Or
//-----------------------------------------------------------------------------
Expr *Operator_Or::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL | numR);
	}
	return new Expr_BinOp(Operator::Or, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Xor
//-----------------------------------------------------------------------------
Expr *Operator_Xor::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL ^ numR);
	}
	return new Expr_BinOp(Operator::Xor, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_And
//-----------------------------------------------------------------------------
Expr *Operator_And::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL & numR);
	}
	return new Expr_BinOp(Operator::And, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Eq
//-----------------------------------------------------------------------------
Expr *Operator_Eq::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer((numL == numR)? 1 : 0);
	}
	return new Expr_BinOp(Operator::Eq, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_NotEq
//-----------------------------------------------------------------------------
Expr *Operator_NotEq::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer((numL != numR)? 1 : 0);
	}
	return new Expr_BinOp(Operator::NotEq, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Lt
//-----------------------------------------------------------------------------
Expr *Operator_Lt::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer((numL < numR)? 1 : 0);
	}
	return new Expr_BinOp(Operator::Lt, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Le
//-----------------------------------------------------------------------------
Expr *Operator_Le::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer((numL <= numR)? 1 : 0);
	}
	return new Expr_BinOp(Operator::Le, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Gt
//-----------------------------------------------------------------------------
Expr *Operator_Gt::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer((numL > numR)? 1 : 0);
	}
	return new Expr_BinOp(Operator::Gt, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Ge
//-----------------------------------------------------------------------------
Expr *Operator_Ge::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer((numL >= numR)? 1 : 0);
	}
	return new Expr_BinOp(Operator::Ge, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_ShiftL
//-----------------------------------------------------------------------------
Expr *Operator_ShiftL::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL << numR);
	}
	return new Expr_BinOp(Operator::ShiftL, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_ShiftR
//-----------------------------------------------------------------------------
Expr *Operator_ShiftR::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL >> numR);
	}
	return new Expr_BinOp(Operator::ShiftR, pExprL.release(), pExprR.release());
}
