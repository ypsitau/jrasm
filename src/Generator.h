//=============================================================================
// Generator.h
//=============================================================================
#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "Expr.h"
#include "Context.h"

//-----------------------------------------------------------------------------
// Generator
//-----------------------------------------------------------------------------
class Generator {
public:
	inline Generator() {}
	virtual ~Generator();
	virtual bool EvalExpr(Context &context, const Expr *pExpr) = 0;
};

#endif
