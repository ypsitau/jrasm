//=============================================================================
// Generator.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Generator
//-----------------------------------------------------------------------------
Generator::~Generator()
{
}

bool Generator::Generate(Context &context, const Expr_Instruction *pExpr) const
{
	return Generate(context, pExpr, context.GetBuffer());
}
