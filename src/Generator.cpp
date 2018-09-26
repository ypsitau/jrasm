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

bool Generator::ForwardAddress(Context &context, const Expr_Instruction *pExpr, UInt32 *pBytes) const
{
	if (!context.CheckRegionReady()) return false;
	return DoForwardAddress(context, pExpr, pBytes);
}

bool Generator::Generate(Context &context, const Expr_Instruction *pExpr) const
{
	if (!context.CheckRegionReady()) return false;
	return DoGenerate(context, pExpr, context.GetBuffer());
}

bool Generator::Generate(Context &context, const Expr_Instruction *pExpr, Binary &buffDst) const
{
	if (!context.CheckRegionReady()) return false;
	return DoGenerate(context, pExpr, buffDst);
}
