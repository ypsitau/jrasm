//=============================================================================
// Directive.h
//=============================================================================
#ifndef __DIRECTIVE_H__
#define __DIRECTIVE_H__

#include "Common.h"
#include "MmlParser.h"

class Token;
class Parser;
class ExprStack;
class Expr_Directive;
class DirectiveOwner;

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
class Directive {
private:
	String _symbol;
	static std::unique_ptr<DirectiveOwner> _pDirectivesBuiltIn;
public:
	inline Directive(const String &symbol) : _symbol(symbol) {}
	virtual ~Directive();
	static void Initialize();
	static const Directive *FindBuiltIn(const char *symbol);
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	virtual bool CreateExpr(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const = 0;
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
	inline Directive_CSEG() : Directive(".cseg") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
class Directive_DB : public Directive {
public:
	inline Directive_DB() : Directive(".db") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
private:
	bool DoGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DSEG
//-----------------------------------------------------------------------------
class Directive_DSEG : public Directive {
public:
	inline Directive_DSEG() : Directive(".dseg") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
class Directive_DW : public Directive {
public:
	inline Directive_DW() : Directive(".dw") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ENDM
//-----------------------------------------------------------------------------
class Directive_ENDM : public Directive {
public:
	inline Directive_ENDM() : Directive(".endm") {}
	virtual bool CreateExpr(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ENDP
//-----------------------------------------------------------------------------
class Directive_ENDP : public Directive {
public:
	inline Directive_ENDP() : Directive(".endp") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ENDPCG
//-----------------------------------------------------------------------------
class Directive_ENDPCG : public Directive {
public:
	inline Directive_ENDPCG() : Directive(".endpcg") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
class Directive_EQU : public Directive {
public:
	inline Directive_EQU() : Directive(".equ") {}
	virtual bool CreateExpr(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
	virtual Expr *Resolve(Context &context, const Expr_Directive *pExpr) const;
};

//-----------------------------------------------------------------------------
// Directive_FILENAME_JR
//-----------------------------------------------------------------------------
class Directive_FILENAME_JR : public Directive {
public:
	inline Directive_FILENAME_JR() : Directive(".filename.jr") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_INCLUDE
//-----------------------------------------------------------------------------
class Directive_INCLUDE : public Directive {
public:
	inline Directive_INCLUDE() : Directive(".include") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ISEG
//-----------------------------------------------------------------------------
class Directive_ISEG : public Directive {
public:
	inline Directive_ISEG() : Directive(".iseg") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
class Directive_MACRO : public Directive {
public:
	inline Directive_MACRO() : Directive(".macro") {}
	virtual bool CreateExpr(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
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
	inline Directive_MML() : Directive(".mml") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ORG
//-----------------------------------------------------------------------------
class Directive_ORG : public Directive {
public:
	inline Directive_ORG() : Directive(".org") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
class Directive_PCG : public Directive {
public:
	inline Directive_PCG() : Directive(".pcg") {}
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_PROC
//-----------------------------------------------------------------------------
class Directive_PROC : public Directive {
public:
	inline Directive_PROC() : Directive(".proc") {}
	virtual bool CreateExpr(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseResolve(Context &context, const Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

#endif
