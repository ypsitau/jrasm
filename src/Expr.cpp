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

bool Expr::OnPhaseInclude(Context &context)
{
	// nothing to do
	return true;
}

bool Expr::OnPhaseDeclareMacro(Context &context)
{
	// nothing to do
	return true;
}

bool Expr::OnPhaseExpandMacro(Context &context)
{
	// nothing to do
	return true;
}

bool Expr::OnPhaseSetupExprDict(Context &context)
{
	_pExprDict.reset(context.GetExprDictCurrent().Reference());
	for (auto pExpr : GetChildren()) {
		if (!pExpr->OnPhaseSetupExprDict(context)) return false;
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
	if (empty() || !back()->IsTypeLabelDef()) return nullptr;
	Expr_LabelDef *pExprLabelDef = dynamic_cast<Expr_LabelDef *>(back());
	return pExprLabelDef->IsAssigned()? nullptr : pExprLabelDef;
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

ExprOwner *ExprOwner::Substitute(const ExprDict &exprDict) const
{
	AutoPtr<ExprOwner> pExprOwner(new ExprOwner());
	pExprOwner->reserve(size());
	for (auto pExpr : *this) {
		pExprOwner->push_back(pExpr->Substitute(exprDict));
	}
	return pExprOwner.release();
}

//-----------------------------------------------------------------------------
// ExprDict
//-----------------------------------------------------------------------------
ExprDict::~ExprDict()
{
	for (auto iter : *this) {
		Expr::Delete(iter.second);
	}
}

void ExprDict::Associate(const String &label, Expr *pExpr, bool forceGlobalFlag)
{
	ExprDict *pExprDict = forceGlobalFlag? GetGlobal() : this;
	pExprDict->insert(std::make_pair(label, pExpr));
}

bool ExprDict::IsDefined(const char *label) const
{
	return find(label) != end();
}

const Expr *ExprDict::Lookup(const char *label) const
{
	const_iterator iter = find(label);
	if (iter != end()) return iter->second;
	return _pExprDictParent.IsNull()? nullptr : _pExprDictParent->Lookup(label);
}

ExprDict *ExprDict::GetGlobal()
{
	ExprDict *pExprDict = this;
	for ( ; pExprDict->GetParent() != nullptr; pExprDict = pExprDict->GetParent()) ;
	return pExprDict;
}

//-----------------------------------------------------------------------------
// ExprDictOwner
//-----------------------------------------------------------------------------
ExprDictOwner::~ExprDictOwner()
{
	Clear();
}

void ExprDictOwner::Clear()
{
	for (auto pExprDict : *this) {
		ExprDict::Delete(pExprDict);
	}
	clear();
}

//-----------------------------------------------------------------------------
// Expr_Root
//-----------------------------------------------------------------------------
const Expr::Type Expr_Root::TYPE = Expr::TYPE_Root;

bool Expr_Root::OnPhaseInclude(Context &context)
{
	for (auto pExpr : GetChildren()) {
		if (!pExpr->OnPhaseInclude(context)) return false;
	}
	return true;
}

bool Expr_Root::OnPhaseDeclareMacro(Context &context)
{
	for (auto pExpr : GetChildren()) {
		if (!pExpr->OnPhaseDeclareMacro(context)) return false;
	}
	return true;
}

bool Expr_Root::OnPhaseExpandMacro(Context &context)
{
	for (auto pExpr : GetChildren()) {
		if (!pExpr->OnPhaseExpandMacro(context)) return false;
	}
	return true;
}

bool Expr_Root::OnPhaseSetupExprDict(Context &context)
{
	context.ResetSegment();
	bool rtn = Expr::OnPhaseSetupExprDict(context);
	return rtn;
}

bool Expr_Root::OnPhaseGenerate(Context &context) const
{
	context.ResetSegment();
	for (auto pExpr : GetChildren()) {
		if (!pExpr->OnPhaseGenerate(context)) return false;
	}
	return true;
}

bool Expr_Root::OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	context.ResetSegment();
	for (auto pExpr : GetChildren()) {
		if (!pExpr->OnPhaseDisasm(context, fp, upperCaseFlag, nColsPerLine)) return false;
	}
	return true;
}

Expr *Expr_Root::Resolve(Context &context) const
{
	return Reference();
}

Expr *Expr_Root::Substitute(const ExprDict &exprDict) const
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

Expr *Expr_Number::Substitute(const ExprDict &exprDict) const
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

Expr *Expr_String::Substitute(const ExprDict &exprDict) const
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

Expr *Expr_BitPattern::Substitute(const ExprDict &exprDict) const
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

Expr *Expr_BinOp::Substitute(const ExprDict &exprDict) const
{
	return Reference();
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

Expr *Expr_Bracket::Substitute(const ExprDict &exprDict) const
{
	return Reference();
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

Expr *Expr_Brace::Substitute(const ExprDict &exprDict) const
{
	return Reference();
}

//-----------------------------------------------------------------------------
// Expr_LabelDef
//-----------------------------------------------------------------------------
const Expr::Type Expr_LabelDef::TYPE = Expr::TYPE_LabelDef;

bool Expr_LabelDef::OnPhaseSetupExprDict(Context &context)
{
	if (!Expr::OnPhaseSetupExprDict(context)) return false;
	if (_pExprDict->IsDefined(GetLabel())) {
		ErrorLog::AddError(this, "duplicated definition of label: %s", GetLabel());
		return false;
	}
	if (IsAssigned()) {
		_pExprDict->Associate(GetLabel(), GetAssigned()->Reference(), _forceGlobalFlag);
	} else {
		_pExprDict->Associate(GetLabel(), new Expr_Number(context.GetAddress()), _forceGlobalFlag);
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

Expr *Expr_LabelDef::Substitute(const ExprDict &exprDict) const
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
	if (!context.IsPhase(Context::PHASE_Generate)) return new Expr_Number(0);
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

Expr *Expr_LabelRef::Substitute(const ExprDict &exprDict) const
{
	return Reference();
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

bool Expr_Instruction::OnPhaseExpandMacro(Context &context)
{
	const Macro *pMacro = context.GetMacroDict().Lookup(GetSymbol());
	if (pMacro == nullptr) return true;
	AutoPtr<ExprDict> pExprDict(new ExprDict());
	const StringList &paramNames = pMacro->GetParamNames();
	const ExprList &exprList = GetOperands();
	StringList::const_iterator pParamName = paramNames.begin();
	ExprList::const_iterator ppExpr = exprList.begin();
	for ( ; pParamName != paramNames.end() && ppExpr != exprList.end(); pParamName++, ppExpr++) {
		const String &paramName = *pParamName;
		const Expr *pExpr = *ppExpr;
		pExprDict->Associate(paramName, pExpr->Reference(), false);
	}
	if (pParamName != paramNames.end()) {
		ErrorLog::AddError(this, "too few parameters for %s", pMacro->GetSymbol());
		return false;
	}
	if (ppExpr != exprList.end()) {
		ErrorLog::AddError(this, "too many parameters for %s", pMacro->GetSymbol());
		return false;
	}
	_pExprsExpanded.reset(pMacro->GetExprOwner().Substitute(*pExprDict));
	return true;
}

bool Expr_Instruction::OnPhaseSetupExprDict(Context &context)
{
	if (!Expr::OnPhaseSetupExprDict(context)) return false;
	bool rtn = true;
	if (_pExprsExpanded.IsNull()) {
		rtn = Generator::GetInstance().ForwardAddress(context, this);
	} else {
		context.PushLocalExprDict();	// .proc
		for (auto pExpr : *_pExprsExpanded) {
			if (!(rtn = pExpr->OnPhaseSetupExprDict(context))) break;
		}
		context.PopLocalExprDict();		// .endp
	}
	return rtn;
}

bool Expr_Instruction::OnPhaseGenerate(Context &context) const
{
	bool rtn = true;
	if (_pExprsExpanded.IsNull()) {
		rtn = Generator::GetInstance().Generate(context, this);
	} else {
		for (auto pExpr : *_pExprsExpanded) {
			if (!(rtn = pExpr->OnPhaseGenerate(context))) break;
		}
	}
	return rtn;
}

bool Expr_Instruction::OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	bool rtn = true;
	if (_pExprsExpanded.IsNull()) {
		Binary buffDst;
		UInt32 addr = context.GetAddress();
		if ((rtn = Generator::GetInstance().Generate(context, this, buffDst))) {
			DumpDisasmHelper(addr, buffDst, ComposeSource(upperCaseFlag).c_str(),
							 fp, upperCaseFlag, nColsPerLine, nColsPerLine);
		}
	} else {
		String paddingLeft = MakePadding(9 + 3 * nColsPerLine + 1);
		::fprintf(fp, "%s%s\n", paddingLeft.c_str(), ComposeSource(upperCaseFlag).c_str());
		for (auto pExpr : *_pExprsExpanded) {
			if (!(rtn = pExpr->OnPhaseDisasm(context, fp, upperCaseFlag, nColsPerLine))) break;
		}
	}
	return rtn;
}

Expr *Expr_Instruction::Resolve(Context &context) const
{
	return Reference();
}

Expr *Expr_Instruction::Substitute(const ExprDict &exprDict) const
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

bool Expr_Directive::OnPhaseInclude(Context &context)
{
	return _pDirective->OnPhaseInclude(context, this);
}

bool Expr_Directive::OnPhaseDeclareMacro(Context &context)
{
	return _pDirective->OnPhaseDeclareMacro(context, this);
}

bool Expr_Directive::OnPhaseExpandMacro(Context &context)
{
	return _pDirective->OnPhaseExpandMacro(context, this);
}

bool Expr_Directive::OnPhaseSetupExprDict(Context &context)
{
	if (!Expr::OnPhaseSetupExprDict(context)) return false;
	return _pDirective->OnPhaseSetupExprDict(context, this);
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

Expr *Expr_Directive::Substitute(const ExprDict &exprDict) const
{
	return Reference();
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

Expr *Expr_MacroBody::Substitute(const ExprDict &exprDict) const
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
// Expr_MacroDecl
//-----------------------------------------------------------------------------
const Expr::Type Expr_MacroDecl::TYPE = Expr::TYPE_MacroDecl;

bool Expr_MacroDecl::OnPhaseDeclareMacro(Context &context)
{
	const ExprList &operands = GetOperands();
	if (operands.empty()) {
		ErrorLog::AddError(this, "directive .MACRO needs at least one operand");
		return false;
	}
	StringList labels;
	for (auto pExpr : operands) {
		if (!pExpr->IsTypeLabelRef()) {
			ErrorLog::AddError(this, "directive .MACRO takes a list of labels as its operands");
			return false;
		}
		labels.push_back(dynamic_cast<const Expr_LabelRef *>(pExpr)->GetLabel());
	}
	AutoPtr<Macro> pMacro(new Macro(labels[0], labels.begin() + 1, labels.end(),
									GetMacroBody()->GetChildren().Reference()));
	context.GetMacroDict().Associate(pMacro->GetSymbol(), pMacro.release());
	return true;
}

bool Expr_MacroDecl::OnPhaseDisasm(Context &context, FILE *fp, bool upperCaseFlag, size_t nColsPerLine) const
{
	String paddingLeft = MakePadding(9 + 3 * nColsPerLine + 1);
	::fprintf(fp, "%s%s\n", paddingLeft.c_str(), ComposeSource(upperCaseFlag).c_str());
	for (auto pExpr : GetMacroBody()->GetChildren()) {
		::fprintf(fp, "%s%s\n", paddingLeft.c_str(), pExpr->ComposeSource(upperCaseFlag).c_str());
	}
	return true;
}

Expr *Expr_MacroDecl::Resolve(Context &context) const
{
	return Reference();
}

Expr *Expr_MacroDecl::Substitute(const ExprDict &exprDict) const
{
	return Reference();
}

String Expr_MacroDecl::ComposeSource(bool upperCaseFlag) const
{
	String str = upperCaseFlag? ".MACRO" : ".macro";
	str += ' ';
	str += GetChildren().ComposeSource(",", upperCaseFlag);
	return str;
}
