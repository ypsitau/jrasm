//=============================================================================
// Directive.h
//=============================================================================
#ifndef __JRASMCORE_DIRECTIVE_H__
#define __JRASMCORE_DIRECTIVE_H__

#include "Common.h"
#include "MMLParser.h"
#include "PCGData.h"
#include "PCGPage.h"

class Context;
class Directive;
class DirectiveFactory;
class DisasmDumper;
class Expr;
class ExprStack;
class Parser;
class Token;

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
	bool _childrenEvalFlag;
	bool _groupingFlag;
public:
	inline DirectiveFactory(const String &symbol, bool childrenEvalFlag, bool groupingFlag) :
		_symbol(symbol), _childrenEvalFlag(childrenEvalFlag), _groupingFlag(groupingFlag) {}
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline bool IsIdentical(const DirectiveFactory *pDirectiveFactory) const {
		return this == pDirectiveFactory;
	}
	inline bool GetChildrenEvalFlag() const { return _childrenEvalFlag; }
	inline bool GetGroupingFlag() const { return _groupingFlag; }
	virtual Directive *Create() const = 0;
	static const DirectiveFactory *Lookup(const char *symbol);
};

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
class Directive {
public:
	class SaveInfo {
	private:
		int _cntRef;
		int _iSavePoint;
		StringList _regNamesToSave;
		StringISet _regNamesToRestore;
	public:
		DeclareReferenceAccessor(SaveInfo);
	public:
		SaveInfo() : _cntRef(1), _iSavePoint(0) {}
	private:
		inline ~SaveInfo() {}
	public:
		inline void SetSavePoint(int iSavePoint) { _iSavePoint = iSavePoint; }
		inline int GetSavePoint() const { return _iSavePoint; }
		inline const StringList &GetRegNamesToSave() const { return _regNamesToSave; }
		inline bool IsFirstRegNameToSave(const char *regName) const {
			return std::find(_regNamesToSave.begin(), _regNamesToSave.end(), regName) == _regNamesToSave.end();
		}
		inline bool IsFirstRegNameToRestore(const char *regName) const {
			return _regNamesToRestore.find(regName) == _regNamesToRestore.end();
		}
		inline void AddRegNameToSave(const char *regName) {
			_regNamesToSave.push_back(ToLower(regName));
		}
		inline void AddRegNameToRestore(const char *regName) {
			_regNamesToRestore.insert(ToLower(regName));
		}
		String MakeLabel(const char *regName) const;
		bool CheckValidation(const Expr *pExpr) const;
	};
private:
	int _cntRef;
	const DirectiveFactory *_pDirectiveFactory;
public:
	static const DirectiveFactory *CSEG;
	static const DirectiveFactory *DB;
	static const DirectiveFactory *DSEG;
	static const DirectiveFactory *DS;
	static const DirectiveFactory *DW;
	static const DirectiveFactory *END;
	static const DirectiveFactory *EQU;
	static const DirectiveFactory *FILENAME_JR;
	static const DirectiveFactory *INCLUDE;
	static const DirectiveFactory *MACRO;
	static const DirectiveFactory *MML;
	static const DirectiveFactory *ORG;
	static const DirectiveFactory *PCG;
	static const DirectiveFactory *PCGPAGE;
	static const DirectiveFactory *RESTORE;
	static const DirectiveFactory *SAVE;
	static const DirectiveFactory *SCOPE;
	static const DirectiveFactory *STRUCT;
private:
	static DirectiveFactoryDict _directiveFactoryDict;
public:
	DeclareReferenceAccessor(Directive);
public:
	inline Directive(const DirectiveFactory *pDirectiveFactory) :
		_cntRef(1), _pDirectiveFactory(pDirectiveFactory) {}
protected:
	virtual ~Directive();
public:
	static void Initialize();
	inline static const DirectiveFactory *LookupFactory(const char *symbol) {
		return _directiveFactoryDict.Lookup(symbol);
	}
	inline const char *GetSymbol() const { return _pDirectiveFactory->GetSymbol(); }
	inline bool GetChildrenEvalFlag() const { return _pDirectiveFactory->GetChildrenEvalFlag(); }
	inline bool GetGroupingFlag() const { return _pDirectiveFactory->GetGroupingFlag(); }
	inline const DirectiveFactory *GetDirectiveFactory() const { return _pDirectiveFactory; }
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhasePreprocess(Context &context, Expr *pExpr);
	virtual bool OnPhaseAssignMacro(Context &context, Expr *pExpr);
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
		inline Factory() : DirectiveFactory(".CSEG", true, false) {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_CSEG() : Directive(CSEG) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
class Directive_DB : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".DB", true, false) {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_DB() : Directive(DB) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
	static bool DoDirective(Context &context, const Expr *pExpr, Binary *pBuffDst, Integer *pBytes);
};

//-----------------------------------------------------------------------------
// Directive_DS
//-----------------------------------------------------------------------------
class Directive_DS : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".DS", true, false) {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_DS() : Directive(DS) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
	bool DoDirective(Context &context, const Expr *pExpr, Binary *pBuffDst, Integer *pBytes) const;
};

//-----------------------------------------------------------------------------
// Directive_DSEG
//-----------------------------------------------------------------------------
class Directive_DSEG : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".DSEG", true, false) {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_DSEG() : Directive(DSEG) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
class Directive_DW : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".DW", true, false) {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_DW() : Directive(DW) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_END
//-----------------------------------------------------------------------------
class Directive_END : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".END", true, false) {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_END() : Directive(END) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
class Directive_EQU : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".EQU", true, false) {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_EQU() : Directive(EQU) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual Expr *Resolve(Context &context, const Expr *pExpr) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_FILENAME_JR
//-----------------------------------------------------------------------------
class Directive_FILENAME_JR : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".FILENAME.JR", true, false) {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_FILENAME_JR() : Directive(FILENAME_JR) {}
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_INCLUDE
//-----------------------------------------------------------------------------
class Directive_INCLUDE : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".INCLUDE", true, false) {}
		virtual Directive *Create() const;
	};
private:
	AutoPtr<Expr> _pExprIncluded;
public:
	inline Directive_INCLUDE() : Directive(INCLUDE) {}
	virtual bool OnPhasePreprocess(Context &context, Expr *pExpr);
	virtual bool OnPhaseAssignMacro(Context &context, Expr *pExpr);
	virtual bool OnPhaseExpandMacro(Context &context, Expr *pExpr);
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
class Directive_MACRO : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".MACRO", false, true) {}
		virtual Directive *Create() const;
	};
private:
	String _symbol;
	bool _forceGlobalFlag;
public:
	inline Directive_MACRO() : Directive(MACRO) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhaseAssignMacro(Context &context, Expr *pExpr);
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
		inline Factory() : DirectiveFactory(".MML", true, false) {}
		virtual Directive *Create() const;
	};
public:
	class Handler : public MMLParser::Handler {
	private:
		Binary &_buff;
	public:
		Handler(Binary &buff) : _buff(buff) {}
		virtual bool OnMMLNote(MMLParser &parser, int note, int length);
		virtual bool OnMMLRest(MMLParser &parser, int length);
	};
private:
	Binary _buff;
public:
	inline Directive_MML() : Directive(MML) {}
	virtual bool OnPhasePreprocess(Context &context, Expr *pExpr);
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_ORG
//-----------------------------------------------------------------------------
class Directive_ORG : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".ORG", true, false) {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_ORG() : Directive(ORG) {}
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
	static bool DoDirective(Context &context, const Expr *pExpr);
};

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
class Directive_PCG : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".PCG", false, true) {}
		virtual Directive *Create() const;
	};
private:
	AutoPtr<PCGData> _pPCGData;
	AutoPtr<Expr> _pExprGenerated;
public:
	inline Directive_PCG() : Directive(PCG) {}
	static bool ExtractParams(Context &context, const Expr *pExpr, String *pSymbol,
							  int *pWdChar, int *pHtChar, int *pStepX, int *pStepY,
							  std::unique_ptr<PCGColorOwner> *ppPCGColorOwner, Binary &buff);
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhasePreprocess(Context &context, Expr *pExpr);
	virtual bool OnPhaseAssignMacro(Context &context, Expr *pExpr);
	virtual bool OnPhaseExpandMacro(Context &context, Expr *pExpr);
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
		inline Factory() : DirectiveFactory(".PCGPAGE", false, true) {}
		virtual Directive *Create() const;
	};
private:
	AutoPtr<PCGPage> _pPCGPage;
	AutoPtr<Expr> _pExprGenerated;
public:
	inline Directive_PCGPAGE() : Directive(PCGPAGE) {}
	inline void SetPCGPage(PCGPage *pPCGPage) { _pPCGPage.reset(pPCGPage); }
	inline PCGPage *GetPCGPage() { return _pPCGPage.get(); }
	inline const PCGPage *GetPCGPage() const { return _pPCGPage.get(); }
	static bool ExtractParams(const Expr *pExpr, String *pSymbol,
							  std::unique_ptr<PCGRangeOwner> *ppPCGRangeOwner);
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhasePreprocess(Context &context, Expr *pExpr);
	virtual bool OnPhaseAssignMacro(Context &context, Expr *pExpr);
	virtual bool OnPhaseExpandMacro(Context &context, Expr *pExpr);
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_RESTORE
//-----------------------------------------------------------------------------
class Directive_RESTORE : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".RESTORE", false, true) {}
		virtual Directive *Create() const;
	};
private:
	AutoPtr<SaveInfo> _pSaveInfo;
public:
	inline Directive_RESTORE() : Directive(RESTORE) {}
	inline void SetSaveInfo(SaveInfo *pSaveInfo) { _pSaveInfo.reset(pSaveInfo); }
	inline SaveInfo &GetSaveInfo() { return *_pSaveInfo; }
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhasePreprocess(Context &context, Expr *pExpr);
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_SAVE
//-----------------------------------------------------------------------------
class Directive_SAVE : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".SAVE", false, true) {}
		virtual Directive *Create() const;
	};
private:
	AutoPtr<SaveInfo> _pSaveInfo;
public:
	inline Directive_SAVE() : Directive(SAVE), _pSaveInfo(new SaveInfo()) {}
	inline SaveInfo &GetSaveInfo() { return *_pSaveInfo; }
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhasePreprocess(Context &context, Expr *pExpr);
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_SCOPE
//-----------------------------------------------------------------------------
class Directive_SCOPE : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".SCOPE", false, true) {}
		virtual Directive *Create() const;
	};
public:
	inline Directive_SCOPE() : Directive(SCOPE) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// Directive_STRUCT
//-----------------------------------------------------------------------------
class Directive_STRUCT : public Directive {
public:
	class Factory : public DirectiveFactory {
	public:
		inline Factory() : DirectiveFactory(".STRUCT", false, true) {}
		virtual Directive *Create() const;
	};
private:
	String _symbol;
	bool _forceGlobalFlag;
public:
	inline Directive_STRUCT() : Directive(STRUCT) {}
	virtual bool OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken);
	virtual bool OnPhaseAssignSymbol(Context &context, Expr *pExpr);
	virtual bool OnPhaseDisasm(Context &context, const Expr *pExpr,
							   DisasmDumper &disasmDumper, int indentLevelCode) const;
};

#endif
