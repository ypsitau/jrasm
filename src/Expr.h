//=============================================================================
// Expr.h
//=============================================================================
#ifndef __EXPR_H__
#define __EXPR_H__

#include "Operator.h"
#include "DisasmDumper.h"

class Directive;
class ExprOwner;
class Expr;
class Expr_SymbolDef;

//-----------------------------------------------------------------------------
// ExprDict
//-----------------------------------------------------------------------------
class ExprDict : public std::map<String, Expr *, LessThan_StringICase> {
private:
	int _cntRef;
	AutoPtr<ExprDict> _pExprDictParent;
public:
	DeclareReferenceAccessor(ExprDict);
public:
	inline ExprDict(ExprDict *pExprDictParent = nullptr) :
	_cntRef(1), _pExprDictParent(pExprDictParent) {}
private:
	~ExprDict();
public:
	inline ExprDict *GetParent() { return _pExprDictParent.get(); }
	void Assign(const String &symbol, Expr *pExpr, bool forceGlobalFlag);
	bool IsAssigned(const char *symbol) const;
	const Expr *Lookup(const char *symbol) const;
	ExprDict *GetGlobal();
	inline const ExprDict *GetGlobal() const {
		return const_cast<ExprDict *>(this)->GetGlobal();
	}
};

//-----------------------------------------------------------------------------
// ExprList
//-----------------------------------------------------------------------------
class ExprList : public std::vector<Expr *> {
public:
	Expr_SymbolDef *SeekSymbolDefToAssoc();
	String ComposeSource(bool upperCaseFlag, const char *sep) const;
	void Print(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// ExprOwner
//-----------------------------------------------------------------------------
class ExprOwner : public ExprList {
private:
	int _cntRef;
public:
	DeclareReferenceAccessor(ExprOwner);
public:
	inline ExprOwner() : _cntRef(1) {}
protected:
	~ExprOwner();
public:
	void Clear();
	ExprOwner *Clone() const;
	ExprOwner *Substitute(const ExprDict &exprDict) const;
};

//-----------------------------------------------------------------------------
// Expr
//-----------------------------------------------------------------------------
class Expr {
public:
	enum Type {
		TYPE_Root,
		TYPE_Number,
		TYPE_String,
		TYPE_BitPattern,
		TYPE_BinOp,
		TYPE_Bracket,
		TYPE_Brace,
		TYPE_SymbolDef,
		TYPE_SymbolRef,
		TYPE_Instruction,
		TYPE_Directive,
		TYPE_Group,
		TYPE_MacroDecl,
	};
public:
protected:
	int _cntRef;
	Type _type;
	AutoPtr<ExprOwner> _pExprChildren;
	AutoPtr<ExprDict> _pExprDict;
	AutoPtr<StringShared> _pFileNameSrc;
	int _lineNo;
public:
	DeclareReferenceAccessor(Expr);
public:
	Expr(Type type);
	Expr(Type type, ExprOwner *pExprChildren);
	Expr(const Expr &expr);
protected:
	virtual ~Expr();
public:
	inline bool IsType(Type type) const { return _type == type; }
	inline bool IsTypeRoot() const { return IsType(TYPE_Root); }
	inline bool IsTypeNumber() const { return IsType(TYPE_Number); }
	inline bool IsTypeString() const { return IsType(TYPE_String); }
	inline bool IsTypeBitPattern() const { return IsType(TYPE_BitPattern); }
	inline bool IsTypeBinOp() const { return IsType(TYPE_BinOp); }
	inline bool IsTypeBracket() const { return IsType(TYPE_Bracket); }
	inline bool IsTypeBrace() const { return IsType(TYPE_Brace); }
	inline bool IsTypeSymbolDef() const { return IsType(TYPE_SymbolDef); }
	inline bool IsTypeSymbolRef() const { return IsType(TYPE_SymbolRef); }
	inline bool IsTypeInstruction() const { return IsType(TYPE_Instruction); }
	inline bool IsTypeDirective() const { return IsType(TYPE_Directive); }
	inline bool IsTypeGroup() const { return IsType(TYPE_Group); }
	inline bool IsTypeMacroDecl() const { return IsType(TYPE_MacroDecl); }
	inline Type GetType() const { return _type; }
	inline ExprOwner &GetChildren() { return *_pExprChildren; }
	inline const ExprOwner &GetChildren() const { return *_pExprChildren; }
	inline void SetSourceInfo(StringShared *pFileNameSrc, int lineNo) {
		_pFileNameSrc.reset(pFileNameSrc), _lineNo = lineNo;
	}
	inline bool HasSourceInfo() const { return !_pFileNameSrc.IsNull(); }
	inline void DeriveSourceInfo(const Expr *pExpr) {
		_pFileNameSrc.reset(pExpr->_pFileNameSrc->Reference());
		_lineNo = pExpr->_lineNo;
	}
	inline const char *GetFileNameSrc() const {
		return _pFileNameSrc.IsNull()? "" : _pFileNameSrc->GetString();
	}
	inline int GetLineNo() const { return _lineNo; }
	inline bool IsExprDictReady() const { return !_pExprDict.IsNull(); }
	inline const ExprDict *GetExprDict() const { return _pExprDict.get(); }
	bool IsTypeSymbolDef(const char *symbol) const;
	bool IsTypeSymbolRef(const char *symbol) const;
	bool IsTypeBinOp(const Operator *pOperator) const;
	void AddChild(Expr *pExpr);
	void Print() const;
	virtual bool OnPhaseInclude(Context &context);
	virtual bool OnPhaseDeclareMacro(Context &context);
	virtual bool OnPhaseExpandMacro(Context &context);
	virtual bool OnPhaseSetupExprDict(Context &context);
	virtual bool OnPhaseGenerate(Context &context, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const;
	virtual Expr *Resolve(Context &context) const = 0;
	virtual Expr *Clone() const = 0;
	virtual Expr *Substitute(const ExprDict &exprDict) const = 0;
	virtual String ComposeSource(bool upperCaseFlag) const = 0;
};

//-----------------------------------------------------------------------------
// ExprStack
//-----------------------------------------------------------------------------
class ExprStack : public ExprOwner {
};

//-----------------------------------------------------------------------------
// ExprDictList
//-----------------------------------------------------------------------------
class ExprDictList : public std::vector<ExprDict *> {
};

//-----------------------------------------------------------------------------
// ExprDictList
//-----------------------------------------------------------------------------
class ExprDictOwner : public ExprDictList {
public:
	~ExprDictOwner();
	void Clear();
};

//-----------------------------------------------------------------------------
// ExprDictOwner
//-----------------------------------------------------------------------------
typedef ExprDictOwner ExprDictStack;

//-----------------------------------------------------------------------------
// Expr_Root
//-----------------------------------------------------------------------------
class Expr_Root : public Expr {
public:
	static const Type TYPE;
public:
	inline Expr_Root() : Expr(TYPE) {}
	inline Expr_Root(ExprOwner *pExprChildren) : Expr(TYPE, pExprChildren) {}
	inline Expr_Root(const Expr_Root &expr) : Expr(expr) {}
	virtual bool OnPhaseInclude(Context &context);
	virtual bool OnPhaseDeclareMacro(Context &context);
	virtual bool OnPhaseExpandMacro(Context &context);
	virtual bool OnPhaseSetupExprDict(Context &context);
	virtual bool OnPhaseGenerate(Context &context, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const;
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_Group
//-----------------------------------------------------------------------------
class Expr_Group : public Expr {
public:
	static const Type TYPE;
public:
	inline Expr_Group() : Expr(TYPE) {}
	inline Expr_Group(const Expr_Group &expr) : Expr(expr) {}
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_Number
//-----------------------------------------------------------------------------
class Expr_Number : public Expr {
private:
	String _str;
	UInt32 _num;
public:
	static const Type TYPE;
public:
	inline Expr_Number(UInt32 num) : Expr(TYPE), _num(num) {}
	inline Expr_Number(const String &str, UInt32 num) : Expr(TYPE), _str(str), _num(num) {}
	inline Expr_Number(const Expr_Number &expr) : Expr(expr), _str(expr._str), _num(expr._num) {}
	inline UInt32 GetNumber() const { return _num; }
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_String
//-----------------------------------------------------------------------------
class Expr_String : public Expr {
private:
	String _str;
public:
	static const Type TYPE;
public:
	inline Expr_String(const String &str) : Expr(TYPE), _str(str) {}
	inline Expr_String(const Expr_String &expr) : Expr(expr), _str(expr._str) {}
	inline const char *GetString() const { return _str.c_str(); }
	inline const String &GetStringSTL() const { return _str; }
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_BitPattern
//-----------------------------------------------------------------------------
class Expr_BitPattern : public Expr {
private:
	String _str;
public:
	static const Type TYPE;
public:
	inline Expr_BitPattern(const String &str) : Expr(TYPE), _str(str) {}
	inline Expr_BitPattern(const Expr_BitPattern &expr) : Expr(expr), _str(expr._str) {}
	inline const char *GetBitPattern() const { return _str.c_str(); }
	inline size_t GetBitPatternLen() const { return _str.size(); }
	Binary GetBinary() const;
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_BinOp
//-----------------------------------------------------------------------------
class Expr_BinOp : public Expr {
private:
	const Operator *_pOperator;
public:
	static const Type TYPE;
public:
	inline Expr_BinOp(const Operator *pOperator, Expr *pExprL, Expr *pExprR) :
			Expr(TYPE), _pOperator(pOperator) {
		GetChildren().push_back(pExprL);
		GetChildren().push_back(pExprR);
	}
	inline Expr_BinOp(const Operator *pOperator, ExprOwner *pExprChildren) :			
		Expr(TYPE, pExprChildren), _pOperator(pOperator) {}
	inline Expr_BinOp(const Expr_BinOp &expr) : Expr(expr), _pOperator(expr._pOperator) {}
	inline const Expr *GetLeft() const { return GetChildren()[0]; }
	inline const Expr *GetRight() const { return GetChildren()[1]; }
	inline const Operator *GetOperator() const { return _pOperator; }
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_Bracket
//-----------------------------------------------------------------------------
class Expr_Bracket : public Expr {
public:
	static const Type TYPE;
public:
	inline Expr_Bracket() : Expr(TYPE) {}
	inline Expr_Bracket(ExprOwner *pExprChildren) : Expr(TYPE, pExprChildren) {}
	inline Expr_Bracket(const Expr_Bracket &expr) : Expr(expr) {}
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_Brace
//-----------------------------------------------------------------------------
class Expr_Brace : public Expr {
public:
	static const Type TYPE;
public:
	inline Expr_Brace() : Expr(TYPE) {}
	inline Expr_Brace(ExprOwner *pExprChildren) : Expr(TYPE, pExprChildren) {}
	inline Expr_Brace(const Expr_Brace &expr) : Expr(expr) {}
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_SymbolDef
//-----------------------------------------------------------------------------
class Expr_SymbolDef : public Expr {
private:
	String _symbol;
	bool _forceGlobalFlag;
public:
	static const Type TYPE;
public:
	inline Expr_SymbolDef(const String &symbol, bool forceGlobalFlag) :
		Expr(TYPE), _symbol(symbol), _forceGlobalFlag(forceGlobalFlag) {}
	inline Expr_SymbolDef(const Expr_SymbolDef &expr) :
		Expr(expr), _symbol(expr._symbol), _forceGlobalFlag(expr._forceGlobalFlag) {}
	inline void SetAssigned(Expr *pExprAssigned) { GetChildren().push_back(pExprAssigned); }
	inline Expr *GetAssigned() { return GetChildren().back(); }
	inline const Expr *GetAssigned() const { return GetChildren().back(); }
	inline bool IsAssigned() const { return !GetChildren().empty(); }
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline bool GetForceGlobalFlag() const { return _forceGlobalFlag; }
	inline bool MatchCase(const char *symbol) const { return ::strcmp(_symbol.c_str(), symbol) == 0; }
	inline bool MatchICase(const char *symbol) const { return ::strcasecmp(_symbol.c_str(), symbol) == 0; }
	virtual bool OnPhaseDeclareMacro(Context &context);
	virtual bool OnPhaseSetupExprDict(Context &context);
	virtual bool OnPhaseGenerate(Context &context, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const;
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
	static String MakeSource(const char *symbol, bool forceGlobalFlag);
};

//-----------------------------------------------------------------------------
// Expr_SymbolRef
//-----------------------------------------------------------------------------
class Expr_SymbolRef : public Expr {
private:
	String _symbol;
public:
	static const Type TYPE;
public:
	inline Expr_SymbolRef(const String &symbol) : Expr(TYPE), _symbol(symbol) {}
	inline Expr_SymbolRef(const Expr_SymbolRef &expr) : Expr(expr), _symbol(expr._symbol) {}
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline bool MatchCase(const char *symbol) const { return ::strcmp(_symbol.c_str(), symbol) == 0; }
	inline bool MatchICase(const char *symbol) const { return ::strcasecmp(_symbol.c_str(), symbol) == 0; }
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_Instruction
//-----------------------------------------------------------------------------
class Expr_Instruction : public Expr {
private:
	String _symbol;
	AutoPtr<ExprOwner> _pExprsExpanded;
public:
	static const Type TYPE;
public:
	inline Expr_Instruction(const String &symbol) : Expr(TYPE), _symbol(symbol) {}
	inline Expr_Instruction(const String &symbol, ExprOwner *pExprChildren) :
		Expr(TYPE, pExprChildren), _symbol(symbol) {}
	inline Expr_Instruction(const Expr_Instruction &expr) :
		Expr(expr), _symbol(expr._symbol),
		_pExprsExpanded(expr._pExprsExpanded.IsNull()? nullptr : expr._pExprsExpanded->Clone()) {}
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline const ExprOwner &GetOperands() const { return GetChildren(); }
	virtual bool OnPhaseExpandMacro(Context &context);
	virtual bool OnPhaseSetupExprDict(Context &context);
	virtual bool OnPhaseGenerate(Context &context, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const;
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_Directive
//-----------------------------------------------------------------------------
class Expr_Directive : public Expr {
private:
	const Directive *_pDirective;
	AutoPtr<Expr> _pExprIncluded;	// only used by .INCLUDE
public:
	static const Type TYPE;
public:
	inline Expr_Directive(const Directive *pDirective) : Expr(TYPE), _pDirective(pDirective) {}
	inline Expr_Directive(const Directive *pDirective, ExprOwner *pExprChildren) :
		Expr(TYPE, pExprChildren), _pDirective(pDirective) {}
	inline Expr_Directive(const Expr_Directive &expr) : Expr(expr), _pDirective(expr._pDirective) {}
	inline const Directive *GetDirective() const { return _pDirective; }
	inline const ExprOwner &GetOperands() const { return GetChildren(); }
	inline void SetExprIncluded(Expr *pExprIncluded) { _pExprIncluded.reset(pExprIncluded); }
	inline Expr *GetExprIncluded() const { return _pExprIncluded.get(); }
	virtual bool OnPhaseInclude(Context &context);
	virtual bool OnPhaseDeclareMacro(Context &context);
	virtual bool OnPhaseExpandMacro(Context &context);
	virtual bool OnPhaseSetupExprDict(Context &context);
	virtual bool OnPhaseGenerate(Context &context, Binary *pBuffDst) const;
	virtual bool OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const;
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_MacroDecl
//-----------------------------------------------------------------------------
class Expr_MacroDecl : public Expr {
private:
	String _symbol;
	bool _forceGlobalFlag;
	AutoPtr<Expr_Group> _pExprGroup;
public:
	static const Type TYPE;
public:
	inline Expr_MacroDecl(const Expr_SymbolDef *pExprSymbolDef) :
		Expr(TYPE), _symbol(pExprSymbolDef->GetSymbol()),
		_forceGlobalFlag(pExprSymbolDef->GetForceGlobalFlag()), _pExprGroup(new Expr_Group()) {}
	inline Expr_MacroDecl(const Expr_MacroDecl &expr) :
		Expr(expr), _symbol(expr._symbol), _forceGlobalFlag(expr._forceGlobalFlag),
		_pExprGroup(dynamic_cast<Expr_Group *>(expr._pExprGroup->Clone())) {}
	inline const char *GetSymbol() const {
		return dynamic_cast<Expr_SymbolRef *>(GetChildren().front())->GetSymbol();
	}
	inline Expr_Group *GetGroup() { return _pExprGroup.get(); }
	inline const Expr_Group *GetGroup() const { return _pExprGroup.get(); }
	inline const ExprOwner &GetOperands() const { return GetChildren(); }
	virtual bool OnPhaseDeclareMacro(Context &context);
	virtual bool OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const;
	virtual Expr *Resolve(Context &context) const;
	virtual Expr *Clone() const;
	virtual Expr *Substitute(const ExprDict &exprDict) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

#endif
