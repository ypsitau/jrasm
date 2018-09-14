//=============================================================================
// Expr.h
//=============================================================================
#ifndef __EXPR_H__
#define __EXPR_H__

#include "Common.h"

class ExprOwner;

//-----------------------------------------------------------------------------
// Expr
//-----------------------------------------------------------------------------
class Expr {
public:
	enum Type {
		TYPE_Inst,
		TYPE_Number,
		TYPE_Symbol,
		TYPE_String,
		TYPE_BinOp,
		TYPE_Bracket,
		TYPE_Parenthesis,
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
	bool IsType(Type type) const { return _type == type; }
	Type GetType() const { return _type; }
	void AddChild(Expr *pExpr);
	ExprOwner &GetChildren() { return *_pExprChildren; }
	const ExprOwner &GetChildren() const { return *_pExprChildren; }
	//virtual bool Reduce() = 0;
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
// Expr_Inst
//-----------------------------------------------------------------------------
class Expr_Inst : public Expr {
private:
	String _symbol;
public:
	inline Expr_Inst(const String &symbol) : Expr(TYPE_Inst), _symbol(symbol) {}
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline const ExprOwner &GetOperands() const { return GetChildren(); }
	virtual String ToString() const;
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
	UInt32 _num;
	bool _validNumFlag;
public:
	inline Expr_Symbol(const String &str) :
		Expr(TYPE_Symbol), _str(str), _num(0), _validNumFlag(false) {}
	inline void SetNumber(UInt32 num) { _num = num; _validNumFlag = true; }
	inline void InvalidateNumber() { _validNumFlag = false; }
	inline UInt32 GetNumber() const { return _num; }
	inline bool IsValidNumber() const { return _validNumFlag; }
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
public:
	inline Expr_BinOp(Expr *pExprL, Expr *pExprR) : Expr(TYPE_BinOp) {
		GetChildren().push_back(pExprL);
		GetChildren().push_back(pExprR);
	}
	const Expr *GetLeft() const { return GetChildren()[0]; }
	const Expr *GetRight() const { return GetChildren()[1]; }
	virtual String ToString() const;
};

class Expr_BinOp_Add : public Expr_BinOp {
public:
	inline Expr_BinOp_Add(Expr *pExprL, Expr *pExprR) :
		Expr_BinOp(pExprL, pExprR) {}
};

class Expr_BinOp_Sub : public Expr_BinOp {
public:
	inline Expr_BinOp_Sub(Expr *pExprL, Expr *pExprR) :
		Expr_BinOp(pExprL, pExprR) {}
};

class Expr_BinOp_Mul : public Expr_BinOp {
public:
	inline Expr_BinOp_Mul(Expr *pExprL, Expr *pExprR) :
		Expr_BinOp(pExprL, pExprR) {}
};

class Expr_BinOp_Div : public Expr_BinOp {
public:
	inline Expr_BinOp_Div(Expr *pExprL, Expr *pExprR) :
		Expr_BinOp(pExprL, pExprR) {}
};

class Expr_Bracket : public Expr {
public:
	inline Expr_Bracket() : Expr(TYPE_Bracket) {}
	virtual String ToString() const;
};

class Expr_Parenthesis : public Expr {
public:
	inline Expr_Parenthesis() : Expr(TYPE_Parenthesis) {}
	virtual String ToString() const;
};

#endif
