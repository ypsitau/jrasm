//=============================================================================
// Generator.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Generator
//-----------------------------------------------------------------------------
std::unique_ptr<Generator> Generator::_pGenerator;

Generator::~Generator()
{
}

void Generator::Initialize(Generator *pGenerator)
{
	_pGenerator.reset(pGenerator);
}

bool Generator::Generate(Context &context, const Expr_Instruction *pExpr) const
{
	if (!context.CheckRegionReady()) return false;
	return Generate(context, pExpr, context.GetBuffer());
}
