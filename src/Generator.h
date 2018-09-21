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
	size_t _bytesInstMax;
	static std::unique_ptr<Generator> _pGenerator;
public:
	inline Generator(size_t bytesInstMax) {}
	virtual ~Generator();
	inline static Generator &GetInstance() { return *_pGenerator; }
	inline size_t GetBytesInstMax() const { return _bytesInstMax; }
	static void Initialize(Generator *pGenerator);
	bool CalcInstBytes(Context &context, const Expr_Instruction *pExpr, UInt32 *pBytes) const;
	bool Generate(Context &context, const Expr_Instruction *pExpr) const;
	bool Generate(Context &context, const Expr_Instruction *pExpr, Binary &buffDst) const;
	virtual bool IsRegisterSymbol(const char *symbol) const = 0;
	virtual bool DoCalcInstBytes(Context &context, const Expr_Instruction *pExpr, UInt32 *pBytes) const = 0;
	virtual bool DoGenerate(Context &context, const Expr_Instruction *pExpr, Binary &buffDst) const = 0;
};

#endif
