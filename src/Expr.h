//=============================================================================
// Expr.h
//=============================================================================
#ifndef __EXPR_H__
#define __EXPR_H__

#include "Directive.h"
#include "Context.h"
#include "Operator.h"

class ExprOwner;
class Expr_LabelDef;

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
		TYPE_LabelDef,
		TYPE_LabelRef,
		TYPE_Instruction,
		TYPE_Directive,
		TYPE_MacroBody,
		TYPE_MacroEntry,
	};
protected:
	int _cntRef;
	Type _type;
	std::auto_ptr<ExprOwner> _pExprChildren;
	AutoPtr<Context::LookupTable> _pLookupTable;
	AutoPtr<StringShared> _pFileNameSrc;
	int _lineNo;
public:
	DeclareReferenceAccessor(Expr);
public:
	Expr(Type type);
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
	inline bool IsTypeLabelDef() const { return IsType(TYPE_LabelDef); }
	inline bool IsTypeLabelRef() const { return IsType(TYPE_LabelRef); }
	inline bool IsTypeInstruction() const { return IsType(TYPE_Instruction); }
	inline bool IsTypeDirective() const { return IsType(TYPE_Directive); }
	inline bool IsTypeMacroBody() const { return IsType(TYPE_MacroBody); }
	inline bool IsTypeMacroEntry() const { return IsType(TYPE_MacroEntry); }
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
	inline bool IsLookupTableReady() const { return !_pLookupTable.IsNull(); }
	inline const Expr *Lookup(const char *label) const {
		return _pLookupTable.IsNull()? nullptr : _pLookupTable->Lookup(label);
	}
	bool IsTypeLabelDef(const char *label) const;
	bool IsTypeLabelRef(const char *label) const;
	bool IsTypeBinOp(const Operator *pOperator) const;
	void AddChild(Expr *pExpr);
	void Print() const;
	static void DumpDisasmHelper(
		UInt32 addr, const Binary &buff, const char *strCode,
		FILE *fp, bool upperCaseFlag, size_t nColsPerLine, size_t nColsPerLineMax);
	virtual bool OnPhaseSetupLookup(Context &context);
	virtual bool OnPhaseGenerate(Context &context) const;
	virtual bool OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const;
	virtual Expr *Resolve(Context &context) const = 0;
	virtual String ComposeSource(bool upperCaseFlag) const = 0;
};

//-----------------------------------------------------------------------------
// ExprList
//-----------------------------------------------------------------------------
class ExprList : public std::vector<Expr *> {
public:
	Expr_LabelDef *SeekLabelDefToAssoc();
	String ComposeSource(const char *sep, bool upperCaseFlag) const;
	void Print(bool upperCaseFlag) const;
	template<typename T_Expr> inline T_Expr *GetBack() {
		if (empty() || !back()->IsType(T_Expr::TYPE)) return nullptr;
		return dynamic_cast<T_Expr *>(back());
	}
};

//-----------------------------------------------------------------------------
// ExprOwner
//-----------------------------------------------------------------------------
class ExprOwner : public ExprList {
public:
	~ExprOwner();
	void Clear();
};

//-----------------------------------------------------------------------------
// ExprStack
//-----------------------------------------------------------------------------
class ExprStack : public ExprOwner {
};

//-----------------------------------------------------------------------------
// Expr_Root
//-----------------------------------------------------------------------------
class Expr_Root : public Expr {
public:
	static const Type TYPE;
public:
	inline Expr_Root() : Expr(TYPE) {}
	virtual bool OnPhaseSetupLookup(Context &context);
	virtual bool OnPhaseGenerate(Context &context) const;
	virtual bool OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const;
	virtual Expr *Resolve(Context &context) const;
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
	inline UInt32 GetNumber() const { return _num; }
	virtual Expr *Resolve(Context &context) const;
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
	inline const char *GetString() const { return _str.c_str(); }
	inline const String &GetStringSTL() const { return _str; }
	virtual Expr *Resolve(Context &context) const;
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
	inline const char *GetBitPattern() const { return _str.c_str(); }
	inline size_t GetBitPatternLen() const { return _str.size(); }
	Binary GetBinary() const;
	virtual Expr *Resolve(Context &context) const;
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
	inline const Expr *GetLeft() const { return GetChildren()[0]; }
	inline const Expr *GetRight() const { return GetChildren()[1]; }
	inline const Operator *GetOperator() const { return _pOperator; }
	virtual Expr *Resolve(Context &context) const;
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
	virtual Expr *Resolve(Context &context) const;
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
	virtual Expr *Resolve(Context &context) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_LabelDef
//-----------------------------------------------------------------------------
class Expr_LabelDef : public Expr {
private:
	String _label;
	bool _forceGlobalFlag;
public:
	static const Type TYPE;
public:
	inline Expr_LabelDef(const String &label, bool forceGlobalFlag) :
		Expr(TYPE), _label(label), _forceGlobalFlag(forceGlobalFlag) {}
	inline void SetAssigned(Expr *pExprAssigned) { GetChildren().push_back(pExprAssigned); }
	inline const Expr *GetAssigned() const { return GetChildren().back(); }
	inline bool IsAssigned() const { return !GetChildren().empty(); }
	inline const char *GetLabel() const { return _label.c_str(); }
	inline bool MatchCase(const char *label) const { return ::strcmp(_label.c_str(), label) == 0; }
	inline bool MatchICase(const char *label) const { return ::strcasecmp(_label.c_str(), label) == 0; }
	virtual bool OnPhaseSetupLookup(Context &context);
	virtual bool OnPhaseGenerate(Context &context) const;
	virtual bool OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const;
	virtual Expr *Resolve(Context &context) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_LabelRef
//-----------------------------------------------------------------------------
class Expr_LabelRef : public Expr {
private:
	String _label;
public:
	static const Type TYPE;
public:
	inline Expr_LabelRef(const String &label) : Expr(TYPE), _label(label) {}
	inline const char *GetLabel() const { return _label.c_str(); }
	inline bool MatchCase(const char *label) const { return ::strcmp(_label.c_str(), label) == 0; }
	inline bool MatchICase(const char *label) const { return ::strcasecmp(_label.c_str(), label) == 0; }
	virtual Expr *Resolve(Context &context) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_Instruction
//-----------------------------------------------------------------------------
class Expr_Instruction : public Expr {
private:
	String _symbol;
public:
	static const Type TYPE;
public:
	inline Expr_Instruction(const String &symbol) : Expr(TYPE), _symbol(symbol) {}
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline const ExprOwner &GetOperands() const { return GetChildren(); }
	virtual bool OnPhaseSetupLookup(Context &context);
	virtual bool OnPhaseGenerate(Context &context) const;
	virtual bool OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const;
	virtual Expr *Resolve(Context &context) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_Directive
//-----------------------------------------------------------------------------
class Expr_Directive : public Expr {
private:
	const Directive *_pDirective;
public:
	static const Type TYPE;
public:
	inline Expr_Directive(const Directive *pDirective) : Expr(TYPE), _pDirective(pDirective) {}
	inline const Directive *GetDirective() const { return _pDirective; }
	inline const ExprOwner &GetOperands() const { return GetChildren(); }
	virtual bool OnPhaseSetupLookup(Context &context);
	virtual bool OnPhaseGenerate(Context &context) const;
	virtual bool OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const;
	virtual Expr *Resolve(Context &context) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_MacroBody
//-----------------------------------------------------------------------------
class Expr_MacroBody : public Expr {
public:
	static const Type TYPE;
public:
	inline Expr_MacroBody() : Expr(TYPE) {}
	virtual Expr *Resolve(Context &context) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

//-----------------------------------------------------------------------------
// Expr_MacroEntry
//-----------------------------------------------------------------------------
class Expr_MacroEntry : public Expr {
private:
	String _symbol;
	AutoPtr<Expr_MacroBody> _pExprMacroBody;
public:
	static const Type TYPE;
public:
	inline Expr_MacroEntry(const String &symbol) :
		Expr(TYPE), _symbol(symbol), _pExprMacroBody(new Expr_MacroBody) {}
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline Expr_MacroBody *GetMacroBody() { return _pExprMacroBody.get(); }
	inline const Expr_MacroBody *GetMacroBody() const { return _pExprMacroBody.get(); }
	inline const ExprOwner &GetOperands() const { return GetChildren(); }
	virtual Expr *Resolve(Context &context) const;
	virtual String ComposeSource(bool upperCaseFlag) const;
};

#endif
