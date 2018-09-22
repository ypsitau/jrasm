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

bool Expr::IsTypeLabelDef(const char *label) const
{
	return IsTypeLabelDef() &&
		::strcasecmp(dynamic_cast<const Expr_LabelDef *>(this)->GetLabel(), label) == 0;
}

bool Expr::IsTypeLabelRef(const char *label) const
{
	return IsTypeLabelRef() &&
		::strcasecmp(dynamic_cast<const Expr_LabelRef *>(this)->GetLabel(), label) == 0;
}

bool Expr::IsTypeBinOp(const Operator *pOperator) const
{
	return IsTypeBinOp() &&
		dynamic_cast<const Expr_BinOp *>(this)->GetOperator()->IsIdentical(pOperator);
}

void Expr::AddChild(Expr *pExpr)
{
	_pExprChildren->push_back(pExpr);
}

bool Expr::Prepare(Context &context)
{
	_pLookupTable.reset(context.GetLookupTable()->Reference());
	_pExprChildren->Prepare(context);
	return true;
}

bool Expr::Generate(Context &context) const
{
	// nothing to do
	return true;
}

bool Expr::DumpDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	// nothing to do
	return true;
}

void Expr::DumpDisasmHelper(UInt32 addr, const Binary &buff, const char *strCode,
							FILE *fp, bool upperCaseFlag, size_t nColsPerLine, size_t nColsPerLineMax)
{
	const char *formatData = upperCaseFlag? " %02X" : " %02x";
	const char *formatHead = upperCaseFlag? "    %04X%s  %s\n" : "    %04x%s  %s\n";
	String str;
	size_t iCol = 0;
	size_t iLine = 0;
	for (auto data : buff) {
		char buff[16];
		::sprintf_s(buff, formatData, static_cast<UInt8>(data));
		str += buff;
		iCol++;
		if (iCol == nColsPerLine) {
			::fprintf(fp, formatHead, addr, JustifyLeft(str.c_str(), 3 * nColsPerLineMax).c_str(),
					  (iLine == 0)? strCode : "");
			str.clear();
			addr += static_cast<UInt32>(iCol);
			iCol = 0;
			iLine++;
		}
	}
	if (iCol > 0) {
		::fprintf(fp, formatHead, addr, JustifyLeft(str.c_str(), 3 * nColsPerLineMax).c_str(),
				  (iLine == 0)? strCode : "");
	}
}

//-----------------------------------------------------------------------------
// ExprList
//-----------------------------------------------------------------------------
Expr_LabelDef *ExprList::SeekLabelDefToAssoc()
{
	Expr_LabelDef *pExprLabelDef = GetBack<Expr_LabelDef>();
	return (pExprLabelDef == nullptr || pExprLabelDef->IsAssigned())? nullptr : pExprLabelDef;
}

bool ExprList::Prepare(Context &context)
{
	for (auto pExpr : *this) {
		if (!pExpr->Prepare(context)) return false;
	}
	return true;
}

String ExprList::ToString(const char *sep, bool upperCaseFlag) const
{
	String rtn;
	for (auto pExpr : *this) {
		if (!rtn.empty()) rtn += sep;
		rtn += pExpr->ToString(upperCaseFlag);
	}
	return rtn;
}

void ExprList::Print(bool upperCaseFlag) const
{
	for (auto pExpr : *this) {
		::printf("%s\n", pExpr->ToString(upperCaseFlag).c_str());
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

bool Expr_Root::Prepare(Context &context)
{
	bool rtn = true;
	context.SetPreparationFlag(true);
	for (int i = 0; i < 2; i++) {
		if (!(rtn = Expr::Prepare(context))) break;
	}
	context.SetPreparationFlag(false);
	return rtn;
}

bool Expr_Root::Generate(Context &context) const
{
	context.ResetSegment();
	for (auto pExpr : GetChildren()) {
		if (!pExpr->Generate(context)) return false;
	}
	return true;
}

bool Expr_Root::DumpDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	context.ResetSegment();
	for (auto pExpr : GetChildren()) {
		if (!pExpr->DumpDisasm(context, fp, upperCaseFlag, nColsPerLine)) return false;
	}
	return true;
}

Expr *Expr_Root::Resolve(Context &context) const
{
	return Reference();
}

String Expr_Root::ToString(bool upperCaseFlag) const
{
	return "";
}

//-----------------------------------------------------------------------------
// Expr_Number
//-----------------------------------------------------------------------------
const Expr::Type Expr_Number::TYPE = Expr::TYPE_Number;

String Expr_Number::ToString(bool upperCaseFlag) const
{
	if (!_str.empty()) return _str;
	const char *format =
		(_num < 0x100)? (upperCaseFlag? "0x%02X" : "0x%02x") :
		(_num < 0x10000)? (upperCaseFlag? "0x%04X" : "0x%04x") :
		(upperCaseFlag? "0x%08X" : "0x%08x");
	char buff[128];
	::sprintf_s(buff, format, _num);
	return buff;
}

Expr *Expr_Number::Resolve(Context &context) const
{
	return Reference();
}

//-----------------------------------------------------------------------------
// Expr_String
//-----------------------------------------------------------------------------
const Expr::Type Expr_String::TYPE = Expr::TYPE_String;

String Expr_String::ToString(bool upperCaseFlag) const
{
	String str;
	str = "\"";
	str += _str; // not affected by upperCaseFlag
	str += "\"";
	return str;
}

Expr *Expr_String::Resolve(Context &context) const
{
	return Reference();
}

//-----------------------------------------------------------------------------
// Expr_BinOp
//-----------------------------------------------------------------------------
const Expr::Type Expr_BinOp::TYPE = Expr::TYPE_BinOp;

String Expr_BinOp::ToString(bool upperCaseFlag) const
{
	String str;
	str = GetLeft()->ToString(upperCaseFlag);
	str += _pOperator->GetSymbol();
	str += GetRight()->ToString(upperCaseFlag);
	return str;
}

Expr *Expr_BinOp::Resolve(Context &context) const
{
	AutoPtr<Expr> pExprL(GetLeft()->Resolve(context));
	if (pExprL.IsNull()) return nullptr;
	AutoPtr<Expr> pExprR(GetRight()->Resolve(context));
	if (pExprR.IsNull()) return nullptr;
	return _pOperator->Resolve(pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Expr_Bracket
//-----------------------------------------------------------------------------
const Expr::Type Expr_Bracket::TYPE = Expr::TYPE_Bracket;

String Expr_Bracket::ToString(bool upperCaseFlag) const
{
	String str;
	str = "[";
	str += GetChildren().ToString(",", upperCaseFlag);
	str += "]";
	return str;
}

Expr *Expr_Bracket::Resolve(Context &context) const
{
	AutoPtr<Expr_Bracket> pExprRtn(new Expr_Bracket());
	pExprRtn->DeriveSourceInfo(this);
	for (auto pExprChild : GetChildren()) {
		AutoPtr<Expr> pExprChildResolved(pExprChild->Resolve(context));
		if (pExprChildResolved.IsNull()) return nullptr;
		pExprRtn->AddChild(pExprChildResolved.release());
	}
	return pExprRtn.release();
}

//-----------------------------------------------------------------------------
// Expr_Brace
//-----------------------------------------------------------------------------
const Expr::Type Expr_Brace::TYPE = Expr::TYPE_Brace;

String Expr_Brace::ToString(bool upperCaseFlag) const
{
	String str;
	str = "{";
	str += GetChildren().ToString(",", upperCaseFlag);
	str += "}";
	return str;
}

Expr *Expr_Brace::Resolve(Context &context) const
{
	AutoPtr<Expr_Brace> pExprRtn(new Expr_Brace());
	pExprRtn->DeriveSourceInfo(this);
	for (auto pExprChild : GetChildren()) {
		AutoPtr<Expr> pExprChildResolved(pExprChild->Resolve(context));
		if (pExprChildResolved.IsNull()) return nullptr;
		pExprRtn->AddChild(pExprChildResolved.release());
	}
	return pExprRtn.release();
}

//-----------------------------------------------------------------------------
// Expr_LabelDef
//-----------------------------------------------------------------------------
const Expr::Type Expr_LabelDef::TYPE = Expr::TYPE_LabelDef;

bool Expr_LabelDef::Prepare(Context &context)
{
	if (!Expr::Prepare(context)) return false;
	UInt32 num = 0;
	if (IsAssigned()) {
		AutoPtr<Expr> pExprAssigned(GetAssigned()->Resolve(context));
		if (pExprAssigned.IsNull()) return false;
		if (!pExprAssigned->IsTypeNumber()) {
			ErrorLog::AddError(this, "number must be specified for label assignment");
			return false;
		}
		num = dynamic_cast<Expr_Number *>(pExprAssigned.get())->GetNumber();
	} else {
		num = context.GetAddress();
	}
	Context::LookupTable *pLookupTable = context.GetLookupTable();
	if (pLookupTable->IsDefined(GetLabel())) {
		//ErrorLog::AddError(this, "duplicated definition of label: %s", GetLabel());
		//return false;
	}
	pLookupTable->Set(GetLabel(), num);
	return true;
}

bool Expr_LabelDef::Generate(Context &context) const
{
	// nothing to do
	return true;
}

bool Expr_LabelDef::DumpDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	String str = _label;
	str += ":";
	if (IsAssigned()) {
		str = JustifyLeft(str.c_str(), 9 + 3 * nColsPerLine) + " ";
		str += GetAssigned()->ToString(upperCaseFlag);
	}
	::fprintf(fp, "%s\n", str.c_str());
	return true;
}

Expr *Expr_LabelDef::Resolve(Context &context) const
{
	return Reference();
}

String Expr_LabelDef::ToString(bool upperCaseFlag) const
{
	String str;
	str = _label; // not affected by upperCaseFlag
	str += ":";
	return str;
}

//-----------------------------------------------------------------------------
// Expr_LabelRef
//-----------------------------------------------------------------------------
const Expr::Type Expr_LabelRef::TYPE = Expr::TYPE_LabelRef;

Expr *Expr_LabelRef::Resolve(Context &context) const
{
	if (Generator::GetInstance().IsRegisterSymbol(GetLabel())) return Reference();
	bool foundFlag = false;
	UInt32 num = Lookup(GetLabel(), &foundFlag);
	if (!context.GetPreparationFlag() && !foundFlag) {
		ErrorLog::AddError(this, "undefined label: %s", GetLabel());
		return nullptr;
	}
	return new Expr_Number(num);
}

String Expr_LabelRef::ToString(bool upperCaseFlag) const
{
	if (Generator::GetInstance().IsRegisterSymbol(_label.c_str())) {
		return upperCaseFlag? ToUpper(_label.c_str()) : ToLower(_label.c_str());
	}
	return _label;
}

//-----------------------------------------------------------------------------
// Expr_Instruction
//-----------------------------------------------------------------------------
const Expr::Type Expr_Instruction::TYPE = Expr::TYPE_Instruction;

bool Expr_Instruction::Prepare(Context &context)
{
	if (!Expr::Prepare(context)) return false;
	UInt32 bytes = 0;
	Generator::GetInstance().CalcInstBytes(context, this, &bytes);
	return true;
}

bool Expr_Instruction::Generate(Context &context) const
{
	return Generator::GetInstance().Generate(context, this);
}

bool Expr_Instruction::DumpDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	Binary buffDst;
	UInt32 addr = context.GetAddress();
	if (!Generator::GetInstance().Generate(context, this, buffDst)) return false;
	DumpDisasmHelper(addr, buffDst, ToString(upperCaseFlag).c_str(),
					 fp, upperCaseFlag, nColsPerLine, nColsPerLine);
	return true;
}

Expr *Expr_Instruction::Resolve(Context &context) const
{
	return Reference();
}

String Expr_Instruction::ToString(bool upperCaseFlag) const
{
	String str = JustifyLeft(
		(upperCaseFlag? ToUpper(_symbol.c_str()) : ToLower(_symbol.c_str())).c_str(),
		Generator::GetInstance().GetInstNameLenMax());
	str += " ";
	str += GetChildren().ToString(",", upperCaseFlag);
	return str;
}

//-----------------------------------------------------------------------------
// Expr_Directive
//-----------------------------------------------------------------------------
const Expr::Type Expr_Directive::TYPE = Expr::TYPE_Directive;

bool Expr_Directive::Prepare(Context &context)
{
	if (!Expr::Prepare(context)) return false;
	return _pDirective->Prepare(context, this);
}

bool Expr_Directive::Generate(Context &context) const
{
	return _pDirective->Generate(context, this, context.GetBuffer());
}

bool Expr_Directive::DumpDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	Binary buffDst;
	UInt32 addr = context.GetAddress();
	if (!_pDirective->Generate(context, this, buffDst)) return false;
	DumpDisasmHelper(addr, buffDst, ToString(upperCaseFlag).c_str(),
					 fp, upperCaseFlag, nColsPerLine / 2 * 2, nColsPerLine);
	return true;
}

Expr *Expr_Directive::Resolve(Context &context) const
{
	return _pDirective->Resolve(context, this);
}

String Expr_Directive::ToString(bool upperCaseFlag) const
{
	const char *symbol = _pDirective->GetSymbol();
	String str = JustifyLeft(
		(upperCaseFlag? ToUpper(symbol) : ToLower(symbol)).c_str(),
		Generator::GetInstance().GetInstNameLenMax());
	str += " ";
	str += GetChildren().ToString(",", upperCaseFlag);
	return str;
}

//-----------------------------------------------------------------------------
// Expr_MacroBody
//-----------------------------------------------------------------------------
const Expr::Type Expr_MacroBody::TYPE = Expr::TYPE_MacroBody;

Expr *Expr_MacroBody::Resolve(Context &context) const
{
	return Reference();
}

String Expr_MacroBody::ToString(bool upperCaseFlag) const
{
	String str = GetChildren().ToString("\n", upperCaseFlag);
	str += "\n";
	return str;
}

//-----------------------------------------------------------------------------
// Expr_MacroEntry
//-----------------------------------------------------------------------------
const Expr::Type Expr_MacroEntry::TYPE = Expr::TYPE_MacroEntry;

Expr *Expr_MacroEntry::Resolve(Context &context) const
{
	return Reference();
}

String Expr_MacroEntry::ToString(bool upperCaseFlag) const
{
	String str = _symbol; // not affected by upperCaseFlag
	str += " ";
	str += GetChildren().ToString(",", upperCaseFlag);
	return str;
}
