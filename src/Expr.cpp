//=============================================================================
// Expr.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Expr
//-----------------------------------------------------------------------------
Expr::Expr(Type type) :
	_cntRef(1), _type(type), _pExprChildren(new ExprOwner()), _lineNo(0)
{
}

Expr::~Expr()
{
}

void Expr::AddChild(Expr *pExpr)
{
	_pExprChildren->push_back(pExpr);
}

bool Expr::PrepareLookupTable(Context &context)
{
	_pLookupTable.reset(context.GetLookupTable()->Reference());
	return true;
}

bool Expr::Generate(Context &context)
{
	ErrorLog::AddError(this, "invalid format");
	return false;
}

//-----------------------------------------------------------------------------
// ExprList
//-----------------------------------------------------------------------------
String ExprList::ToString() const
{
	String rtn;
	for (auto pExpr : *this) {
		if (!rtn.empty()) rtn += ",";
		rtn += pExpr->ToString();
	}
	return rtn;
}

void ExprList::Print() const
{
	for (auto pExpr : *this) {
		::printf("%s\n", pExpr->ToString().c_str());
	}
}

//-----------------------------------------------------------------------------
// ExprOwner
//-----------------------------------------------------------------------------
ExprOwner::~ExprOwner()
{
	Clear();
}

void ExprOwner::Clear()
{
	for (auto pExpr : *this) {
		Expr::Delete(pExpr);
	}
	clear();
}

//-----------------------------------------------------------------------------
// Expr_Number
//-----------------------------------------------------------------------------
String Expr_Number::ToString() const
{
	char buff[128];
	::sprintf_s(buff, "0x%x", _num);
	return buff;
}

//-----------------------------------------------------------------------------
// Expr_Symbol
//-----------------------------------------------------------------------------
String Expr_Symbol::ToString() const
{
	return _str;
}

//-----------------------------------------------------------------------------
// Expr_String
//-----------------------------------------------------------------------------
String Expr_String::ToString() const
{
	String str;
	str = "\"";
	str += _str;
	str += "\"";
	return str;
}

//-----------------------------------------------------------------------------
// Expr_BinOp
//-----------------------------------------------------------------------------
String Expr_BinOp::ToString() const
{
	String str;
	str = GetLeft()->ToString();
	str += " + ";
	str += GetRight()->ToString();
	return str;
}

//-----------------------------------------------------------------------------
// Expr_Bracket
//-----------------------------------------------------------------------------
String Expr_Bracket::ToString() const
{
	String str;
	str = "[";
	str += GetChildren().ToString();
	str += "]";
	return str;
}

//-----------------------------------------------------------------------------
// Expr_Parenthesis
//-----------------------------------------------------------------------------
String Expr_Parenthesis::ToString() const
{
	String str;
	str = "(";
	str += GetChildren().ToString();
	str += ")";
	return str;
}

//-----------------------------------------------------------------------------
// Expr_Label
//-----------------------------------------------------------------------------
String Expr_Label::ToString() const
{
	String str;
	str = _label;
	str += ":";
	return str;
}

bool Expr_Label::PrepareLookupTable(Context &context)
{
	if (!Expr::PrepareLookupTable(context)) return false;
	if (_pExprAssigned.IsNull()) {
		context.GetLookupTable()->Set(GetLabel(), context.GetAddress());
		return true;
	}
	return true;
}

bool Expr_Label::Generate(Context &context)
{
	// nothing to do
	return true;
}

//-----------------------------------------------------------------------------
// Expr_Inst
//-----------------------------------------------------------------------------
String Expr_Inst::ToString() const
{
	String str = _symbol;
	str += " ";
	str += GetChildren().ToString();
	return str;
}

bool Expr_Inst::PrepareLookupTable(Context &context)
{
	if (!Expr::PrepareLookupTable(context)) return false;
	UInt32 bytes = 0;
	context.GetGenerator()->CalcInstBytes(context, this, &bytes);
	context.ForwardAddress(bytes);
	return true;
}

bool Expr_Inst::Generate(Context &context)
{
	return context.GetGenerator()->Generate(context, this);
}
