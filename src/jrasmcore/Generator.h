//=============================================================================
// Generator.h
//=============================================================================
#ifndef __JRASMCORE_GENERATOR_H__
#define __JRASMCORE_GENERATOR_H__

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
	size_t _instNameLenMax;
	static std::unique_ptr<Generator> _pGenerator;
public:
	inline Generator(size_t bytesInstMax, size_t instNameLenMax) :
		_bytesInstMax(bytesInstMax), _instNameLenMax(instNameLenMax) {}
	virtual ~Generator();
	inline static Generator &GetInstance() { return *_pGenerator; }
	inline size_t GetBytesInstMax() const { return _bytesInstMax; }
	inline size_t GetInstNameLenMax() const { return _instNameLenMax; }
	static void Initialize(Generator *pGenerator);
	bool ForwardAddress(Context &context, const Expr_Instruction *pExpr) const;
	bool Generate(Context &context, const Expr_Instruction *pExpr, Binary *pBuffDst) const;
	Expr *ComposeExpr_Save(
		Context &context, Expr *pExpr, Directive::SaveInfo &saveInfo) const;
	Expr *ComposeExpr_Restore(
		Context &context, Expr *pExpr, Directive::SaveInfo &saveInfo, const StringList &regNamesToRestore) const;
	static void DumpDisasmHelper(
		Integer addr, const Binary &buff, const char *strCode,
		FILE *fp, bool upperCaseFlag, size_t nColsPerLine, size_t nColsPerLineMax);
	virtual bool IsRegisterSymbol(const char *symbol) const = 0;
	virtual bool DoForwardAddress(Context &context, const Expr_Instruction *pExpr) const = 0;
	virtual bool DoGenerate(Context &context, const Expr_Instruction *pExpr, Binary &buffDst) const = 0;
	virtual Expr *DoComposeExpr_Save(
		Context &context, Expr *pExpr, Directive::SaveInfo &saveInfo) const = 0;
	virtual Expr *DoComposeExpr_Restore(
		Context &context, Expr *pExpr, Directive::SaveInfo &saveInfo, const StringList &regNamesToRestore) const = 0;
};

#endif
