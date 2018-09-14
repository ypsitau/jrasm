//=============================================================================
// Expr.h
//=============================================================================
#ifndef __EXPR_H__
#define __EXPR_H__

#include "Operator.h"

class ExprOwner;

//-----------------------------------------------------------------------------
// Expr
//-----------------------------------------------------------------------------
class Expr {
public:
	enum Type {
		TYPE_Number,
		TYPE_Symbol,
		TYPE_String,
		TYPE_BinOp,
		TYPE_Bracket,
		TYPE_Parenthesis,
		TYPE_Label,
		TYPE_Inst,
	};
private:
	int _cntRef;
	Type _type;
	std::auto_ptr<ExprOwner> _pExprChildren;
public:
	DeclareReferenceAccessor(Expr);
public:
	Expr(Type type);
protected:
	virtual ~Expr();
public:
	inline bool IsType(Type type) const { return _type == type; }
	inline Type GetType() const { return _type; }
	inline ExprOwner &GetChildren() { return *_pExprChildren; }
	inline const ExprOwner &GetChildren() const { return *_pExprChildren; }
	void AddChild(Expr *pExpr);
	virtual bool Resolve(Context &context);
	virtual bool Generate(Context &context);
	virtual String ToString() const = 0;
};

//-----------------------------------------------------------------------------
// ExprList
//-----------------------------------------------------------------------------
class ExprList : public std::vector<Expr *> {
public:
	String ToString() const;
	void Print() const;
};

//-----------------------------------------------------------------------------
// ExprStack
//-----------------------------------------------------------------------------
typedef std::vector<Expr *> ExprStack;

//-----------------------------------------------------------------------------
// ExprOwner
//-----------------------------------------------------------------------------
class ExprOwner : public ExprList {
public:
	~ExprOwner();
	void Clear();
};

//-----------------------------------------------------------------------------
// Expr_Number
//-----------------------------------------------------------------------------
class Expr_Number : public Expr {
private:
	UInt32 _num;
public:
	inline Expr_Number(UInt32 num) : Expr(TYPE_Number), _num(num) {}
	inline UInt32 GetNumber() const { return _num; }
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Expr_Symbol
//-----------------------------------------------------------------------------
class Expr_Symbol : public Expr {
private:
	String _str;
public:
	inline Expr_Symbol(const String &str) : Expr(TYPE_Symbol), _str(str) {}
	inline bool MatchCase(const char *str) const { return ::strcmp(_str.c_str(), str) == 0; }
	inline bool MatchICase(const char *str) const { return ::strcasecmp(_str.c_str(), str) == 0; }
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Expr_String
//-----------------------------------------------------------------------------
class Expr_String : public Expr {
private:
	String _str;
public:
	inline Expr_String(const String &str) : Expr(TYPE_String), _str(str) {}
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Expr_BinOp
//-----------------------------------------------------------------------------
class Expr_BinOp : public Expr {
private:
	const Operator *_pOperator;
public:
	inline Expr_BinOp(const Operator *pOperator, Expr *pExprL, Expr *pExprR) :
			Expr(TYPE_BinOp), _pOperator(pOperator) {
		GetChildren().push_back(pExprL);
		GetChildren().push_back(pExprR);
	}
	const Expr *GetLeft() const { return GetChildren()[0]; }
	const Expr *GetRight() const { return GetChildren()[1]; }
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Expr_Bracket
//-----------------------------------------------------------------------------
class Expr_Bracket : public Expr {
public:
	inline Expr_Bracket() : Expr(TYPE_Bracket) {}
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Expr_Parenthesis
//-----------------------------------------------------------------------------
class Expr_Parenthesis : public Expr {
public:
	inline Expr_Parenthesis() : Expr(TYPE_Parenthesis) {}
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Expr_Label
//-----------------------------------------------------------------------------
class Expr_Label : public Expr {
private:
	String _str;
public:
	inline Expr_Label(const String &str) : Expr(TYPE_Label), _str(str) {}
	inline bool MatchCase(const char *str) const { return ::strcmp(_str.c_str(), str) == 0; }
	inline bool MatchICase(const char *str) const { return ::strcasecmp(_str.c_str(), str) == 0; }
	virtual bool Resolve(Context &context);
	virtual bool Generate(Context &context);
	virtual String ToString() const;
};

//-----------------------------------------------------------------------------
// Expr_Inst
//-----------------------------------------------------------------------------
class Expr_Inst : public Expr {
private:
	String _symbol;
public:
	inline Expr_Inst(const String &symbol) : Expr(TYPE_Inst), _symbol(symbol) {}
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline const ExprOwner &GetOperands() const { return GetChildren(); }
	virtual bool Resolve(Context &context);
	virtual bool Generate(Context &context);
	virtual String ToString() const;
};

#endif
