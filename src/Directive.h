//=============================================================================
// Directive.h
//=============================================================================
#ifndef __DIRECTIVE_H__
#define __DIRECTIVE_H__

#include "Expr.h"
#include "MmlParser.h"

class Token;
class Parser;
class Context;
class Directive;

//-----------------------------------------------------------------------------
// DirectiveList
//-----------------------------------------------------------------------------
class DirectiveList : public std::vector<const Directive *> {
public:
	inline void Assign(const Directive *pDirective) { push_back(pDirective); }
	const Directive *FindBySymbol(const char *symbol) const;
};

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
class Directive {
private:
	String _symbol;
public:
	static const Directive *CSEG;
	static const Directive *DB;
	static const Directive *DSEG;
	static const Directive *DW;
	static const Directive *ENDM;
	static const Directive *ENDP;
	static const Directive *ENDPCG;
	static const Directive *EQU;
	static const Directive *FILENAME_JR;
	static const Directive *INCLUDE;
	static const Directive *ISEG;
	static const Directive *MACRO;
	static const Directive *MML;
	static const Directive *ORG;
	static const Directive *PCG;
	static const Directive *PROC;
private:
	static DirectiveList _directiveList;
public:
	inline Directive(const String &symbol) : _symbol(symbol) {}
protected:
	virtual ~Directive();
public:
	static void Initialize();
	static const Directive *Lookup(const char *symbol);
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseInclude(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseDeclareMacro(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseExpandMacro(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const = 0;
	virtual Expr *Resolve(Context &context, const Expr_Directive *pExpr) const;
};

//-----------------------------------------------------------------------------
// Directive_CSEG
//-----------------------------------------------------------------------------
class Directive_CSEG : public Directive {
public:
	inline Directive_CSEG() : Directive(".CSEG") {}
	virtual bool OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
class Directive_DB : public Directive {
public:
	inline Directive_DB() : Directive(".DB") {}
	virtual bool OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
private:
	bool DoGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DSEG
//-----------------------------------------------------------------------------
class Directive_DSEG : public Directive {
public:
	inline Directive_DSEG() : Directive(".DSEG") {}
	virtual bool OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
class Directive_DW : public Directive {
public:
	inline Directive_DW() : Directive(".DW") {}
	virtual bool OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ENDM
//-----------------------------------------------------------------------------
class Directive_ENDM : public Directive {
public:
	inline Directive_ENDM() : Directive(".ENDM") {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ENDP
//-----------------------------------------------------------------------------
class Directive_ENDP : public Directive {
public:
	inline Directive_ENDP() : Directive(".ENDP") {}
	virtual bool OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ENDPCG
//-----------------------------------------------------------------------------
class Directive_ENDPCG : public Directive {
public:
	inline Directive_ENDPCG() : Directive(".ENDPCG") {}
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
class Directive_EQU : public Directive {
public:
	inline Directive_EQU() : Directive(".EQU") {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
	virtual Expr *Resolve(Context &context, const Expr_Directive *pExpr) const;
};

//-----------------------------------------------------------------------------
// Directive_FILENAME_JR
//-----------------------------------------------------------------------------
class Directive_FILENAME_JR : public Directive {
public:
	inline Directive_FILENAME_JR() : Directive(".FILENAME.JR") {}
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_INCLUDE
//-----------------------------------------------------------------------------
class Directive_INCLUDE : public Directive {
public:
	inline Directive_INCLUDE() : Directive(".INCLUDE") {}
	virtual bool OnPhaseInclude(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ISEG
//-----------------------------------------------------------------------------
class Directive_ISEG : public Directive {
public:
	inline Directive_ISEG() : Directive(".ISEG") {}
	virtual bool OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
class Directive_MACRO : public Directive {
public:
	inline Directive_MACRO() : Directive(".MACRO") {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseDeclareMacro(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_MML
//-----------------------------------------------------------------------------
class Directive_MML : public Directive {
public:
	class Handler : public MmlParser::Handler {
	private:
		Binary *_pBuffDst;
		UInt32 _bytesSum;
	public:
		Handler(Binary *pBuffDst) : _pBuffDst(pBuffDst), _bytesSum(0) {}
		virtual void MmlNote(MmlParser &parser, unsigned char note, int length);
		virtual void MmlRest(MmlParser &parser, int length);
		inline UInt32 GetBytesSum() const { return _bytesSum; }
	};
public:
	inline Directive_MML() : Directive(".MML") {}
	virtual bool OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ORG
//-----------------------------------------------------------------------------
class Directive_ORG : public Directive {
public:
	inline Directive_ORG() : Directive(".ORG") {}
	virtual bool OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
class Directive_PCG : public Directive {
public:
	inline Directive_PCG() : Directive(".PCG") {}
	virtual bool OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_PROC
//-----------------------------------------------------------------------------
class Directive_PROC : public Directive {
public:
	inline Directive_PROC() : Directive(".PROC") {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

#endif
