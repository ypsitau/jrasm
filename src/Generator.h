//=============================================================================
// Generator.h
//=============================================================================
#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "Common.h"

class Context;
class ExprList;

//-----------------------------------------------------------------------------
// Generator
//-----------------------------------------------------------------------------
class Generator {
public:
	inline Generator() {}
	virtual ~Generator();
	virtual bool Generate(Context &context, const char *symbol, const ExprList &operands) const = 0;
};

#endif
