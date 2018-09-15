//=============================================================================
// Generator.h
//=============================================================================
#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "Common.h"

class Context;
class ExprList;
class Expr_Instruction;

//-----------------------------------------------------------------------------
// Generator
//-----------------------------------------------------------------------------
class Generator {
public:
	inline Generator() {}
	virtual ~Generator();
	virtual bool CalcInstBytes(Context &context, const Expr_Instruction *pExpr, UInt32 *pBytes) const = 0;
	virtual bool Generate(Context &context, const Expr_Instruction *pExpr) const = 0;
};

#endif
