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
	_pExprChildren->PrepareLookupTable(context);
	return true;
}

bool Expr::Generate(Context &context)
{
	// nothing to do
	return true;
}

//-----------------------------------------------------------------------------
// ExprList
//-----------------------------------------------------------------------------
Expr_LabelDef *ExprList::SeekLabelDefToAssoc()
{
	Expr_LabelDef *pExprLabelDef = GetBack<Expr_LabelDef>();
	return (pExprLabelDef == nullptr || pExprLabelDef->IsAssigned())? nullptr : pExprLabelDef;
}

bool ExprList::PrepareLookupTable(Context &context)
{
	for (auto pExpr : *this) {
		pExpr->PrepareLookupTable(context);
	}
	return true;
}

String ExprList::ToString(const char *sep) const
{
	String rtn;
	for (auto pExpr : *this) {
		if (!rtn.empty()) rtn += sep;
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
// Expr_Root
//-----------------------------------------------------------------------------
const Expr::Type Expr_Root::TYPE = Expr::TYPE_Root;

bool Expr_Root::Generate(Context &context)
{
	for (auto pExpr : GetChildren()) {
		pExpr->Generate(context);
	}
	return true;
}

Expr *Expr_Root::Reduce(Context &context) const
{
	return Reference();
}

String Expr_Root::ToString() const
{
	return "";
}

//-----------------------------------------------------------------------------
// Expr_Number
//-----------------------------------------------------------------------------
const Expr::Type Expr_Number::TYPE = Expr::TYPE_Number;

String Expr_Number::ToString() const
{
	char buff[128];
	::sprintf_s(buff, "0x%x", _num);
	return buff;
}

Expr *Expr_Number::Reduce(Context &context) const
{
	return Reference();
}

//-----------------------------------------------------------------------------
// Expr_String
//-----------------------------------------------------------------------------
const Expr::Type Expr_String::TYPE = Expr::TYPE_String;

String Expr_String::ToString() const
{
	String str;
	str = "\"";
	str += _str;
	str += "\"";
	return str;
}

Expr *Expr_String::Reduce(Context &context) const
{
	return Reference();
}

//-----------------------------------------------------------------------------
// Expr_BinOp
//-----------------------------------------------------------------------------
const Expr::Type Expr_BinOp::TYPE = Expr::TYPE_BinOp;

String Expr_BinOp::ToString() const
{
	String str;
	str = GetLeft()->ToString();
	str += " + ";
	str += GetRight()->ToString();
	return str;
}

Expr *Expr_BinOp::Reduce(Context &context) const
{
	AutoPtr<Expr> pExprL(GetLeft()->Reduce(context));
	if (pExprL.IsNull()) return nullptr;
	AutoPtr<Expr> pExprR(GetRight()->Reduce(context));
	if (pExprR.IsNull()) return nullptr;
	
	return Reference();
}

//-----------------------------------------------------------------------------
// Expr_Bracket
//-----------------------------------------------------------------------------
const Expr::Type Expr_Bracket::TYPE = Expr::TYPE_Bracket;

String Expr_Bracket::ToString() const
{
	String str;
	str = "[";
	str += GetChildren().ToString(",");
	str += "]";
	return str;
}

Expr *Expr_Bracket::Reduce(Context &context) const
{
	AutoPtr<Expr_Bracket> pExprRtn(new Expr_Bracket());
	pExprRtn->DeriveSourceInfo(this);
	for (auto pExprChild : GetChildren()) {
		AutoPtr<Expr> pExprChildReduced(pExprChild->Reduce(context));
		if (pExprChildReduced.IsNull()) return nullptr;
		pExprRtn->AddChild(pExprChildReduced.release());
	}
	return pExprRtn.release();
}

//-----------------------------------------------------------------------------
// Expr_Parenthesis
//-----------------------------------------------------------------------------
const Expr::Type Expr_Parenthesis::TYPE = Expr::TYPE_Parenthesis;

String Expr_Parenthesis::ToString() const
{
	String str;
	str = "(";
	str += GetChildren().ToString(",");
	str += ")";
	return str;
}

Expr *Expr_Parenthesis::Reduce(Context &context) const
{
	AutoPtr<Expr_Parenthesis> pExprRtn(new Expr_Parenthesis());
	pExprRtn->DeriveSourceInfo(this);
	for (auto pExprChild : GetChildren()) {
		AutoPtr<Expr> pExprChildReduced(pExprChild->Reduce(context));
		if (pExprChildReduced.IsNull()) return nullptr;
		pExprRtn->AddChild(pExprChildReduced.release());
	}
	return pExprRtn.release();
}

//-----------------------------------------------------------------------------
// Expr_LabelDef
//-----------------------------------------------------------------------------
const Expr::Type Expr_LabelDef::TYPE = Expr::TYPE_LabelDef;

bool Expr_LabelDef::PrepareLookupTable(Context &context)
{
	if (!Expr::PrepareLookupTable(context)) return false;
	UInt32 num = 0;
	if (IsAssigned()) {
		AutoPtr<Expr> pExprAssigned(GetAssigned()->Reduce(context));
		if (pExprAssigned.IsNull()) return false;
		if (!pExprAssigned->IsType(Expr::TYPE_Number)) {
			ErrorLog::AddError(this, "number must be specified for label assignment");
			return false;
		}
		num = dynamic_cast<Expr_Number *>(pExprAssigned.get())->GetNumber();
	} else {
		num = context.GetAddress();
	}
	Context::LookupTable *pLookupTable = context.GetLookupTable();
	if (pLookupTable->IsDefined(GetLabel())) {
		ErrorLog::AddError(this, "duplicated definition of label: %s", GetLabel());
		return false;
	}
	pLookupTable->Set(GetLabel(), num);
	return true;
}

bool Expr_LabelDef::Generate(Context &context)
{
	// nothing to do
	return true;
}

Expr *Expr_LabelDef::Reduce(Context &context) const
{
	return Reference();
}

String Expr_LabelDef::ToString() const
{
	String str;
	str = _label;
	str += ":";
	return str;
}

//-----------------------------------------------------------------------------
// Expr_LabelRef
//-----------------------------------------------------------------------------
const Expr::Type Expr_LabelRef::TYPE = Expr::TYPE_LabelRef;

Expr *Expr_LabelRef::Reduce(Context &context) const
{
	UInt32 num = 0;
	if (context.GetGenerator()->IsRegisterSymbol(GetLabel())) {
		ErrorLog::AddError(this, "register %s can't be specified here", GetLabel());
		return nullptr;
	}
	if (IsLookupTableReady()) {
		bool foundFlag = false;
		num = Lookup(GetLabel(), &foundFlag);
		if (!foundFlag) {
			ErrorLog::AddError(this, "undefined label: %s", GetLabel());
			return nullptr;
		}
	}
	return new Expr_Number(num);
}

String Expr_LabelRef::ToString() const
{
	return _label;
}

//-----------------------------------------------------------------------------
// Expr_Instruction
//-----------------------------------------------------------------------------
const Expr::Type Expr_Instruction::TYPE = Expr::TYPE_Instruction;

bool Expr_Instruction::PrepareLookupTable(Context &context)
{
	if (!Expr::PrepareLookupTable(context)) return false;
	UInt32 bytes = 0;
	context.GetGenerator()->CalcInstBytes(context, this, &bytes);
	return true;
}

bool Expr_Instruction::Generate(Context &context)
{
	return context.GetGenerator()->Generate(context, this);
}

Expr *Expr_Instruction::Reduce(Context &context) const
{
	return Reference();
}

String Expr_Instruction::ToString() const
{
	String str = _symbol;
	str += " ";
	str += GetChildren().ToString(",");
	return str;
}

//-----------------------------------------------------------------------------
// Expr_Directive
//-----------------------------------------------------------------------------
const Expr::Type Expr_Directive::TYPE = Expr::TYPE_Directive;

bool Expr_Directive::PrepareLookupTable(Context &context)
{
	if (!Expr::PrepareLookupTable(context)) return false;
	return _pDirective->PrepareLookupTable(context, this);
}

bool Expr_Directive::Generate(Context &context)
{
	return _pDirective->Generate(context, this);
}

Expr *Expr_Directive::Reduce(Context &context) const
{
	return _pDirective->Reduce(context, this);
}

String Expr_Directive::ToString() const
{
	String str = _pDirective->GetSymbol();
	str += " ";
	str += GetChildren().ToString(",");
	return str;
}

//-----------------------------------------------------------------------------
// Expr_MacroBody
//-----------------------------------------------------------------------------
const Expr::Type Expr_MacroBody::TYPE = Expr::TYPE_MacroBody;

Expr *Expr_MacroBody::Reduce(Context &context) const
{
	return Reference();
}

String Expr_MacroBody::ToString() const
{
	String str = GetChildren().ToString("\n");
	str += "\n";
	return str;
}

//-----------------------------------------------------------------------------
// Expr_MacroEntry
//-----------------------------------------------------------------------------
const Expr::Type Expr_MacroEntry::TYPE = Expr::TYPE_MacroEntry;

Expr *Expr_MacroEntry::Reduce(Context &context) const
{
	return Reference();
}

String Expr_MacroEntry::ToString() const
{
	String str = _symbol;
	str += " ";
	str += GetChildren().ToString(",");
	return str;
}
