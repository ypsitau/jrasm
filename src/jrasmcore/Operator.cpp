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
const Operator *Operator::FieldSep	= nullptr;

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
	FieldSep	= new Operator_FieldSep();
}

Expr *Operator::Resolve(Context &context, AutoPtr<Expr> pExpr) const
{
	return new Expr_Null();
}

Expr *Operator::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	return new Expr_Null();
}

//-----------------------------------------------------------------------------
// Operator_Add
//-----------------------------------------------------------------------------
Expr *Operator_Add::Resolve(Context &context, AutoPtr<Expr> pExpr) const
{
	return pExpr->Clone();
}

Expr *Operator_Add::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL + numR);
	} else if (pExprL->IsTypeBinaryOp(Operator::Add) && pExprR->IsTypeInteger()) {
		const Expr_BinaryOp *pExprBinaryOp = dynamic_cast<Expr_BinaryOp *>(pExprL.get());
		if (pExprBinaryOp->GetLeft()->IsTypeInteger()) {
			Integer numL = dynamic_cast<const Expr_Integer *>(pExprBinaryOp->GetLeft())->GetInteger();
			Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
			return new Expr_BinaryOp(
				Operator::Add, new Expr_Integer(numL + numR), pExprBinaryOp->GetRight()->Reference());
		}
	} else if (pExprL->IsTypeSymbol() && pExprR->IsTypeInteger()) {
		return new Expr_BinaryOp(Operator::Add, pExprR.release(), pExprL.release());
	}
	return new Expr_BinaryOp(Operator::Add, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_Sub
//-----------------------------------------------------------------------------
Expr *Operator_Sub::Resolve(Context &context, AutoPtr<Expr> pExpr) const
{
	if (pExpr->IsTypeInteger()) {
		Integer num = dynamic_cast<const Expr_Integer *>(pExpr.get())->GetInteger();
		return new Expr_Integer(-num);
	}
	return new Expr_UnaryOp(Operator::Sub, pExpr.release());
}

Expr *Operator_Sub::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeInteger() && pExprR->IsTypeInteger()) {
		Integer numL = dynamic_cast<const Expr_Integer *>(pExprL.get())->GetInteger();
		Integer numR = dynamic_cast<const Expr_Integer *>(pExprR.get())->GetInteger();
		return new Expr_Integer(numL - numR);
	}
	return new Expr_BinaryOp(Operator::Sub, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_AddInj
//-----------------------------------------------------------------------------
Expr *Operator_AddInj::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	if (pExprL->IsTypeBracket() && pExprR->IsTypeInteger()) {
		const ExprList &exprOperandsOrg = dynamic_cast<const Expr_Bracket *>(pExprL.get())->GetExprOperands();
		AutoPtr<ExprOwner> pExprOperands(new ExprOwner());
		for (auto pExprOperandOrg : exprOperandsOrg) {
			AutoPtr<Expr> pExprOperand(new Expr_BinaryOp(Operator::Add, pExprOperandOrg->Clone(), pExprR->Clone()));
			AutoPtr<Expr> pExprOperandResolved(pExprOperand->Resolve(context));
			if (pExprOperandResolved.IsNull()) return nullptr;
			pExprOperands->push_back(pExprOperandResolved.release());
		}
		return new Expr_Bracket(pExprOperands.release());
	} else if (pExprL->IsTypeBrace() && pExprR->IsTypeInteger()) {
		const ExprList &exprOperandsOrg = dynamic_cast<const Expr_Brace *>(pExprL.get())->GetExprOperands();
		AutoPtr<ExprOwner> pExprOperands(new ExprOwner());
		for (auto pExprOperandOrg : exprOperandsOrg) {
			AutoPtr<Expr> pExprOperand(new Expr_BinaryOp(Operator::Add, pExprOperandOrg->Clone(), pExprR->Clone()));
			AutoPtr<Expr> pExprOperandResolved(pExprOperand->Resolve(context));
			if (pExprOperandResolved.IsNull()) return nullptr;
			pExprOperands->push_back(pExprOperandResolved.release());
		}
		return new Expr_Brace(pExprOperands.release());
	}
	return new Expr_BinaryOp(Operator::AddInj, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::Mul, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::Div, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::Mod, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::LogicOr, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::LogicAnd, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::Or, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::Xor, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::And, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::Eq, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::NotEq, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::Lt, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::Le, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::Gt, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::Ge, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::ShiftL, pExprL.release(), pExprR.release());
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
	return new Expr_BinaryOp(Operator::ShiftR, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Operator_FieldSep
//-----------------------------------------------------------------------------
Expr *Operator_FieldSep::Resolve(Context &context, AutoPtr<Expr> pExprL, AutoPtr<Expr> pExprR) const
{
	return new Expr_BinaryOp(Operator::FieldSep, pExprL.release(), pExprR.release());
}
