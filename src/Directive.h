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
class ExprStack;
class Expr;
class Expr_Directive;
class DirectiveOwner;

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
class Directive {
private:
	int _cntRef;
	String _symbol;
public:
	DeclareReferenceAccessor(Directive);
public:
	inline Directive(const String &symbol) : _cntRef(1), _symbol(symbol) {}
protected:
	virtual ~Directive();
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseInclude(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseDeclareMacro(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseExpandMacro(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const = 0;
	virtual Expr *Resolve(Context &context, const Expr_Directive *pExpr) const;
};

//-----------------------------------------------------------------------------
// DirectiveList
//-----------------------------------------------------------------------------
class DirectiveList : public std::vector<Directive *> {
public:
	const Directive *FindBySymbol(const char *symbol) const;
};

//-----------------------------------------------------------------------------
// DirectiveOwner
//-----------------------------------------------------------------------------
class DirectiveOwner : public DirectiveList {
public:
	~DirectiveOwner();
	void Clear();
};

//-----------------------------------------------------------------------------
// Directive_CSEG
//-----------------------------------------------------------------------------
class Directive_CSEG : public Directive {
public:
	inline Directive_CSEG() : Directive(".CSEG") {}
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
class Directive_DB : public Directive {
public:
	inline Directive_DB() : Directive(".DB") {}
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
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
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
class Directive_DW : public Directive {
public:
	inline Directive_DW() : Directive(".DW") {}
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
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
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
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
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
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
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ORG
//-----------------------------------------------------------------------------
class Directive_ORG : public Directive {
public:
	inline Directive_ORG() : Directive(".ORG") {}
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
class Directive_PCG : public Directive {
public:
	inline Directive_PCG() : Directive(".PCG") {}
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_PROC
//-----------------------------------------------------------------------------
class Directive_PROC : public Directive {
public:
	inline Directive_PROC() : Directive(".PROC") {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_Custom
//-----------------------------------------------------------------------------
class Directive_Custom : public Directive {
private:
	StringList _paramNames;
	AutoPtr<Expr> _pExprMacroBody;
public:
	Directive_Custom(const String &symbol, StringList::const_iterator pParamName,
					 StringList::const_iterator pParamNameEnd, Expr *pExprMacroBody);
	virtual bool OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

#endif
