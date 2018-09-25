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

void Expr::Print() const
{
	::printf("%s\n", ComposeSource(false).c_str());
}

bool Expr::OnPhaseResolve(Context &context)
{
	_pLookupTable.reset(context.GetLookupTable()->Reference());
	for (auto pExpr : *_pExprChildren) {
		if (!pExpr->OnPhaseResolve(context)) return false;
	}
	return true;
}

bool Expr::OnPhaseGenerate(Context &context) const
{
	// nothing to do
	return true;
}

bool Expr::OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
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

String ExprList::ComposeSource(const char *sep, bool upperCaseFlag) const
{
	String rtn;
	for (auto pExpr : *this) {
		if (!rtn.empty()) rtn += sep;
		rtn += pExpr->ComposeSource(upperCaseFlag);
	}
	return rtn;
}

void ExprList::Print(bool upperCaseFlag) const
{
	for (auto pExpr : *this) {
		::printf("%s\n", pExpr->ComposeSource(upperCaseFlag).c_str());
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

bool Expr_Root::OnPhaseResolve(Context &context)
{
	context.ResetSegment();
	context.SetPhase_Resolve();
	bool rtn = Expr::OnPhaseResolve(context);
	return rtn;
}

bool Expr_Root::OnPhaseGenerate(Context &context) const
{
	context.ResetSegment();
	context.SetPhase_Generate();
	for (auto pExpr : GetChildren()) {
		if (!pExpr->OnPhaseGenerate(context)) return false;
	}
	return true;
}

bool Expr_Root::OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	context.ResetSegment();
	context.SetPhase_Generate();
	for (auto pExpr : GetChildren()) {
		if (!pExpr->OnPhaseDisasm(context, fp, upperCaseFlag, nColsPerLine)) return false;
	}
	return true;
}

Expr *Expr_Root::Resolve(Context &context) const
{
	return Reference();
}

String Expr_Root::ComposeSource(bool upperCaseFlag) const
{
	return "";
}

//-----------------------------------------------------------------------------
// Expr_Number
//-----------------------------------------------------------------------------
const Expr::Type Expr_Number::TYPE = Expr::TYPE_Number;

String Expr_Number::ComposeSource(bool upperCaseFlag) const
{
	if (!_str.empty()) {
		if (_str[0] != '\'') return _str;
		String str;
		str += _str[1];
		str = MakeQuotedString(str.c_str(), '\'');
		str = String("'") + str + "'";
		return str;
	}
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

String Expr_String::ComposeSource(bool upperCaseFlag) const
{
	String str;
	str = "\"";
	str += MakeQuotedString(_str, '"'); // not affected by upperCaseFlag
	str += "\"";
	return str;
}

Expr *Expr_String::Resolve(Context &context) const
{
	return Reference();
}

//-----------------------------------------------------------------------------
// Expr_BitPattern
//-----------------------------------------------------------------------------
const Expr::Type Expr_BitPattern::TYPE = Expr::TYPE_BitPattern;

String Expr_BitPattern::ComposeSource(bool upperCaseFlag) const
{
	String str;
	str = "b\"";
	str += MakeQuotedString(_str, '"'); // not affected by upperCaseFlag
	str += "\"";
	return str;
}

Expr *Expr_BitPattern::Resolve(Context &context) const
{
	return Reference();
}

Binary Expr_BitPattern::GetBinary() const
{
	Binary buff;
	UInt8 data = 0x00;
	size_t nCols = 0;
	for (auto ch : _str) {
		data <<= 1;
		if (!(ch == ' ' || ch == '.' || ch == ',' || ch == '_' || ch == '-')) data |= 1;
		nCols++;
		if (nCols == 8) {
			buff += data;
			nCols = 0;
		}
	}
	return buff;
}

//-----------------------------------------------------------------------------
// Expr_BinOp
//-----------------------------------------------------------------------------
const Expr::Type Expr_BinOp::TYPE = Expr::TYPE_BinOp;

String Expr_BinOp::ComposeSource(bool upperCaseFlag) const
{
	String str;
	str = GetLeft()->ComposeSource(upperCaseFlag);
	str += _pOperator->GetSymbol();
	str += GetRight()->ComposeSource(upperCaseFlag);
	return str;
}

Expr *Expr_BinOp::Resolve(Context &context) const
{
	AutoPtr<Expr> pExprL(GetLeft()->Resolve(context));
	if (pExprL.IsNull()) return nullptr;
	AutoPtr<Expr> pExprR(GetRight()->Resolve(context));
	if (pExprR.IsNull()) return nullptr;
	return _pOperator->Resolve(context, pExprL.release(), pExprR.release());
}

//-----------------------------------------------------------------------------
// Expr_Bracket
//-----------------------------------------------------------------------------
const Expr::Type Expr_Bracket::TYPE = Expr::TYPE_Bracket;

String Expr_Bracket::ComposeSource(bool upperCaseFlag) const
{
	String str;
	str = "[";
	str += GetChildren().ComposeSource(",", upperCaseFlag);
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

String Expr_Brace::ComposeSource(bool upperCaseFlag) const
{
	String str;
	str = "{";
	str += GetChildren().ComposeSource(",", upperCaseFlag);
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

bool Expr_LabelDef::OnPhaseResolve(Context &context)
{
	if (!Expr::OnPhaseResolve(context)) return false;
	if (_pLookupTable->IsDefined(GetLabel())) {
		ErrorLog::AddError(this, "duplicated definition of label: %s", GetLabel());
		return false;
	}
	if (IsAssigned()) {
		_pLookupTable->Associate(GetLabel(), GetAssigned()->Reference(), _forceGlobalFlag);
	} else {
		_pLookupTable->Associate(GetLabel(), new Expr_Number(context.GetAddress()), _forceGlobalFlag);
	}
	return true;
}

bool Expr_LabelDef::OnPhaseGenerate(Context &context) const
{
	// nothing to do
	return true;
}

bool Expr_LabelDef::OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	String str = _label;
	str += ":";
	if (IsAssigned()) {
		str = JustifyLeft(str.c_str(), 9 + 3 * nColsPerLine) + " ";
		str += GetAssigned()->ComposeSource(upperCaseFlag);
	}
	::fprintf(fp, "%s\n", str.c_str());
	return true;
}

Expr *Expr_LabelDef::Resolve(Context &context) const
{
	return Reference();
}

String Expr_LabelDef::ComposeSource(bool upperCaseFlag) const
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
	if (context.CheckCircularReference(this)) return nullptr;
	if (Generator::GetInstance().IsRegisterSymbol(GetLabel())) return Reference();
	if (context.IsPhase_Preparation()) return new Expr_Number(0);
	const Expr *pExpr = Lookup(GetLabel());
	if (pExpr == nullptr) {
		ErrorLog::AddError(this, "undefined label: %s", GetLabel());
		return nullptr;
	}
	AutoPtr<Expr> pExprResolved(pExpr->Resolve(context));
	if (pExprResolved.IsNull()) return nullptr;
	if (!pExprResolved->IsTypeNumber()) {
		ErrorLog::AddError(this, "label %s is associated with something but number", GetLabel());
		return nullptr;
	}
	return pExprResolved.release();
}

String Expr_LabelRef::ComposeSource(bool upperCaseFlag) const
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

bool Expr_Instruction::OnPhaseResolve(Context &context)
{
	if (!Expr::OnPhaseResolve(context)) return false;
	UInt32 bytes = 0;
	Generator::GetInstance().CalcInstBytes(context, this, &bytes);
	return true;
}

bool Expr_Instruction::OnPhaseGenerate(Context &context) const
{
	return Generator::GetInstance().Generate(context, this);
}

bool Expr_Instruction::OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	Binary buffDst;
	UInt32 addr = context.GetAddress();
	if (!Generator::GetInstance().Generate(context, this, buffDst)) return false;
	DumpDisasmHelper(addr, buffDst, ComposeSource(upperCaseFlag).c_str(),
					 fp, upperCaseFlag, nColsPerLine, nColsPerLine);
	return true;
}

Expr *Expr_Instruction::Resolve(Context &context) const
{
	return Reference();
}

String Expr_Instruction::ComposeSource(bool upperCaseFlag) const
{
	String str = JustifyLeft(
		(upperCaseFlag? ToUpper(_symbol.c_str()) : ToLower(_symbol.c_str())).c_str(),
		Generator::GetInstance().GetInstNameLenMax());
	str += " ";
	str += GetChildren().ComposeSource(",", upperCaseFlag);
	return str;
}

//-----------------------------------------------------------------------------
// Expr_Directive
//-----------------------------------------------------------------------------
const Expr::Type Expr_Directive::TYPE = Expr::TYPE_Directive;

bool Expr_Directive::OnPhaseResolve(Context &context)
{
	if (!Expr::OnPhaseResolve(context)) return false;
	return _pDirective->OnPhaseResolve(context, this);
}

bool Expr_Directive::OnPhaseGenerate(Context &context) const
{
	return _pDirective->OnPhaseGenerate(context, this, context.GetBuffer());
}

bool Expr_Directive::OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	Binary buffDst;
	UInt32 addr = context.GetAddress();
	if (!_pDirective->OnPhaseGenerate(context, this, buffDst)) return false;
	DumpDisasmHelper(addr, buffDst, ComposeSource(upperCaseFlag).c_str(),
					 fp, upperCaseFlag, nColsPerLine / 2 * 2, nColsPerLine);
	return true;
}

Expr *Expr_Directive::Resolve(Context &context) const
{
	return _pDirective->Resolve(context, this);
}

String Expr_Directive::ComposeSource(bool upperCaseFlag) const
{
	const char *symbol = _pDirective->GetSymbol();
	String str = JustifyLeft(
		(upperCaseFlag? ToUpper(symbol) : ToLower(symbol)).c_str(),
		Generator::GetInstance().GetInstNameLenMax());
	str += " ";
	str += GetChildren().ComposeSource(",", upperCaseFlag);
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

String Expr_MacroBody::ComposeSource(bool upperCaseFlag) const
{
	String str = GetChildren().ComposeSource("\n", upperCaseFlag);
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

String Expr_MacroEntry::ComposeSource(bool upperCaseFlag) const
{
	String str = _symbol; // not affected by upperCaseFlag
	str += " ";
	str += GetChildren().ComposeSource(",", upperCaseFlag);
	return str;
}
