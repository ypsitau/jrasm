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
	virtual bool HandleToken(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const = 0;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const = 0;
	virtual Expr *Reduce(Context &context, const Expr_Directive *pExpr) const;
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
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
class Directive_DB : public Directive {
public:
	inline Directive_DB() : Directive(".db") {}
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DSEG
//-----------------------------------------------------------------------------
class Directive_DSEG : public Directive {
public:
	inline Directive_DSEG() : Directive(".dseg") {}
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
class Directive_DW : public Directive {
public:
	inline Directive_DW() : Directive(".dw") {}
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ENDM
//-----------------------------------------------------------------------------
class Directive_ENDM : public Directive {
public:
	inline Directive_ENDM() : Directive(".endm") {}
	virtual bool HandleToken(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ENDP
//-----------------------------------------------------------------------------
class Directive_ENDP : public Directive {
public:
	inline Directive_ENDP() : Directive(".endp") {}
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
class Directive_EQU : public Directive {
public:
	inline Directive_EQU() : Directive(".equ") {}
	virtual bool HandleToken(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
	virtual Expr *Reduce(Context &context, const Expr_Directive *pExpr) const;
};

//-----------------------------------------------------------------------------
// Directive_FILENAMEJR
//-----------------------------------------------------------------------------
class Directive_FILENAMEJR : public Directive {
public:
	inline Directive_FILENAMEJR() : Directive(".filenamejr") {}
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_INCLUDE
//-----------------------------------------------------------------------------
class Directive_INCLUDE : public Directive {
public:
	inline Directive_INCLUDE() : Directive(".include") {}
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
class Directive_MACRO : public Directive {
public:
	inline Directive_MACRO() : Directive(".macro") {}
	virtual bool HandleToken(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
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
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ORG
//-----------------------------------------------------------------------------
class Directive_ORG : public Directive {
public:
	inline Directive_ORG() : Directive(".org") {}
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
class Directive_PCG : public Directive {
public:
	inline Directive_PCG() : Directive(".pcg") {}
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_PROC
//-----------------------------------------------------------------------------
class Directive_PROC : public Directive {
public:
	inline Directive_PROC() : Directive(".proc") {}
	virtual bool Prepare(Context &context, const Expr_Directive *pExpr) const;
	virtual bool Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const;
};

#endif
