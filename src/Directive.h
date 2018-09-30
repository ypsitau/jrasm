//=============================================================================
// Directive.h
//=============================================================================
#ifndef __DIRECTIVE_H__
#define __DIRECTIVE_H__

#include "Common.h"
#include "MmlParser.h"

class DisasmDumper;
class Expr;
class Expr_Directive;
class ExprStack;
class Token;
class Parser;
class Context;
class Directive;
class DirectiveFactory;

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
	inline bool IsIdentical(const DirectiveFactory *pDirectiveFactory) const {
		return this == pDirectiveFactory;
	}
	virtual Directive *Create() const = 0;
	static const DirectiveFactory *Lookup(const char *symbol);
};

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
class Directive {
private:
	int _cntRef;
	const DirectiveFactory *_pDirectiveFactory;
	bool _childrenEvalFlag;
public:
	static const DirectiveFactory *CSEG;
	static const DirectiveFactory *DB;
	static const DirectiveFactory *DSEG;
	static const DirectiveFactory *DW;
	static const DirectiveFactory *END;
	static const DirectiveFactory *EQU;
	static const DirectiveFactory *FILENAME_JR;
	static const DirectiveFactory *INCLUDE;
	static const DirectiveFactory *ISEG;
	static const DirectiveFactory *MACRO;
	static const DirectiveFactory *MML;
	static const DirectiveFactory *ORG;
	static const DirectiveFactory *PCGDATA;
	static const DirectiveFactory *PCGPAGE;
	static const DirectiveFactory *SCOPE;
private:
	static DirectiveFactoryDict _directiveFactoryDict;
public:
	DeclareReferenceAccessor(Directive);
public:
	inline Directive(const DirectiveFactory *pDirectiveFactory, bool childrenEvalFlag = true) :
		_cntRef(1), _pDirectiveFactory(pDirectiveFactory), _childrenEvalFlag(childrenEvalFlag) {}
protected:
	virtual ~Directive();
public:
	static void Initialize();
	inline static const DirectiveFactory *LookupFactory(const char *symbol) {
		return _directiveFactoryDict.Lookup(symbol);
	}
	inline const char *GetSymbol() const { return _pDirectiveFactory->GetSymbol(); }
	inline bool GetChildrenEvalFlag() const { return _childrenEvalFlag; }
	inline const DirectiveFactory *GetDirectiveFactory() const { return _pDirectiveFactory; }
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhaseInclude(Context &context, Expr *pExpr);
	virtual bool OnPhaseDeclareMacro(Context &context, Expr *pExpr);
	virtual bool OnPhaseExpandMacro(Context &context, Expr *pExpr);
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
	virtual Expr *Resolve(Context &context, const Expr *pExpr) const;
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
	inline Directive_CSEG() : Directive(CSEG) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
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
	inline Directive_DB() : Directive(DB) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	static bool DoDirective(Context &context, const Expr *pExpr, Binary *pBuffDst, UInt32 *pBytes);
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
	inline Directive_DSEG() : Directive(DSEG) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
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
	inline Directive_DW() : Directive(DW) {}
virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
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
	inline Directive_END() : Directive(END) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
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
	inline Directive_EQU() : Directive(EQU) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual Expr *Resolve(Context &context, const Expr *pExpr) const;
};

//-----------------------------------------------------------------------------
// Directive_FILENAME_JR
//-----------------------------------------------------------------------------
class Directive_FILENAME_JR : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".FILENAME.JR") {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_FILENAME_JR() : Directive(FILENAME_JR) {}
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
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
private:
	AutoPtr<Expr> _pExprIncluded;
public:
	inline Directive_INCLUDE() : Directive(INCLUDE) {}
	virtual bool OnPhaseInclude(Context &context, Expr *pExpr);
	virtual bool OnPhaseDeclareMacro(Context &context, Expr *pExpr);
	virtual bool OnPhaseExpandMacro(Context &context, Expr *pExpr);
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
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
	inline Directive_ISEG() : Directive(ISEG) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
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
private:
	String _symbol;
	bool _forceGlobalFlag;
public:
	inline Directive_MACRO() : Directive(MACRO, false) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhaseDeclareMacro(Context &context, Expr *pExpr);
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
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
	inline Directive_MML() : Directive(MML) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
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
	inline Directive_ORG() : Directive(ORG) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	static bool DoDirective(Context &context, const Expr *pExpr);
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
	inline Directive_PCGDATA() : Directive(PCGDATA, false) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
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
	inline Directive_PCGPAGE() : Directive(PCGPAGE, false) {}
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
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
	inline Directive_SCOPE() : Directive(SCOPE, false) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

#endif
