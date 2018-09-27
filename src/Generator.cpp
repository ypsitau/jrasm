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

bool Generator::ForwardAddress(Context &context, const Expr_Instruction *pExpr) const
{
	if (!context.CheckSegmentRegionReady()) return false;
	return DoForwardAddress(context, pExpr);
}

bool Generator::Generate(Context &context, const Expr_Instruction *pExpr, Binary *pBuffDst) const
{
	if (!context.CheckSegmentRegionReady()) return false;
	if (pBuffDst == nullptr) pBuffDst = &context.GetSegmentBuffer();
	return DoGenerate(context, pExpr, *pBuffDst);
}
