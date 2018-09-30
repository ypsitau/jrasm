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
class DirectiveFactory;

//-----------------------------------------------------------------------------
// DirectiveDict
//-----------------------------------------------------------------------------
class DirectiveDict : public std::map<String, const Directive *, LessThan_StringICase> {
public:
	inline DirectiveDict() {}
public:
	void Assign(const Directive *pDirective);
	const Directive *Lookup(const char *symbol) const;
};

//-----------------------------------------------------------------------------
// DirectiveFactoryDict
//-----------------------------------------------------------------------------
class DirectiveFactoryDict : public std::map<String, const DirectiveFactory *, LessThan_StringICase> {
public:
	inline DirectiveFactoryDict() {}
public:
	void Assign(const DirectiveFactory *pDirectiveFactory);
	const DirectiveFactory *Lookup(const char *symbol) const;
};

//-----------------------------------------------------------------------------
// DirectiveFactory
//-----------------------------------------------------------------------------
class DirectiveFactory {
private:
	String _symbol;
public:
	inline DirectiveFactory(const String &symbol) : _symbol(symbol) {}
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	virtual Directive *Create() const = 0;
	static const DirectiveFactory *Lookup(const char *symbol);
};

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
class Directive {
private:
	String _symbol;
	bool _childrenEvalFlag;
public:
	static const Directive *CSEG;
	static const Directive *DB;
	static const Directive *DSEG;
	static const Directive *DW;
	static const Directive *END;
	static const Directive *EQU;
	static const Directive *FILENAME_JR;
	static const Directive *INCLUDE;
	static const Directive *ISEG;
	static const Directive *MACRO;
	static const Directive *MML;
	static const Directive *ORG;
	static const Directive *PCGDATA;
	static const Directive *PCGPAGE;
	static const Directive *SCOPE;
private:
	static DirectiveDict _directiveDict;
	static DirectiveFactoryDict _directiveFactoryDict;
public:
	inline Directive(const String &symbol, bool childrenEvalFlag = true) :
		_symbol(symbol), _childrenEvalFlag(childrenEvalFlag) {}
protected:
	virtual ~Directive();
public:
	static void Initialize();
	static const Directive *Lookup(const char *symbol);
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline bool GetChildrenEvalFlag() const { return _childrenEvalFlag; }
	inline bool IsIdentical(const Directive *pDirective) const { return this == pDirective; }
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseInclude(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseDeclareMacro(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseExpandMacro(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
	virtual Expr *Resolve(Context &context, const Expr_Directive *pExpr) const;
};

//-----------------------------------------------------------------------------
// Directive_CSEG
//-----------------------------------------------------------------------------
class Directive_CSEG : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".CSEG") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_CSEG() : Directive(".CSEG") {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
class Directive_DB : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".DB") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_DB() : Directive(".DB") {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
	static bool DoDirective(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst, UInt32 *pBytes);
};

//-----------------------------------------------------------------------------
// Directive_DSEG
//-----------------------------------------------------------------------------
class Directive_DSEG : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".DSEG") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_DSEG() : Directive(".DSEG") {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
class Directive_DW : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".DW") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_DW() : Directive(".DW") {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_END
//-----------------------------------------------------------------------------
class Directive_END : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".END") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_END() : Directive(".END") {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
class Directive_EQU : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".EQU") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_EQU() : Directive(".EQU") {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual Expr *Resolve(Context &context, const Expr_Directive *pExpr) const;
};

//-----------------------------------------------------------------------------
// Directive_FILENAME_JR
//-----------------------------------------------------------------------------
class Directive_FILENAME_JR : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".FILENAME_JR") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_FILENAME_JR() : Directive(".FILENAME.JR") {}
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_INCLUDE
//-----------------------------------------------------------------------------
class Directive_INCLUDE : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".INCLUDE") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_INCLUDE() : Directive(".INCLUDE") {}
	virtual bool OnPhaseInclude(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseDeclareMacro(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseExpandMacro(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_ISEG
//-----------------------------------------------------------------------------
class Directive_ISEG : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".ISEG") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_ISEG() : Directive(".ISEG") {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
class Directive_MACRO : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".MACRO") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_MACRO() : Directive(".MACRO", false) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseDeclareMacro(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_MML
//-----------------------------------------------------------------------------
class Directive_MML : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".MML") {}
		virtual Directive *Create() const;
	};
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
	virtual bool OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_ORG
//-----------------------------------------------------------------------------
class Directive_ORG : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".ORG") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_ORG() : Directive(".ORG") {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
	static bool DoDirective(Context &context, const Expr_Directive *pExpr);
};

//-----------------------------------------------------------------------------
// Directive_PCGDATA
//-----------------------------------------------------------------------------
class Directive_PCGDATA : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".PCGDATA") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_PCGDATA() : Directive(".PCGDATA", false) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_PCGPAGE
//-----------------------------------------------------------------------------
class Directive_PCGPAGE : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".PCGPAGE") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_PCGPAGE() : Directive(".PCGPAGE", false) {}
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
};

//-----------------------------------------------------------------------------
// Directive_SCOPE
//-----------------------------------------------------------------------------
class Directive_SCOPE : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".SCOPE") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_SCOPE() : Directive(".SCOPE", false) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const;
	virtual bool OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const;
	virtual bool OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

#endif
