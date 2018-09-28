//=============================================================================
// Expr.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Expr
//-----------------------------------------------------------------------------
Expr::Expr(Type type) :
	_cntRef(1), _type(type), _pExprOperands(new ExprOwner()), _pExprChildren(new ExprOwner()), _lineNo(0)
{
}

Expr::Expr(Type type, ExprOwner *pExprOperands, ExprOwner *pExprChildren) :
	_cntRef(1), _type(type), _pExprOperands(pExprOperands), _pExprChildren(pExprChildren), _lineNo(0)
{
}

Expr::Expr(const Expr &expr) :
	_cntRef(1), _type(expr._type),
	_pExprOperands(expr._pExprOperands->Clone()), _pExprChildren(expr._pExprChildren->Clone()),
	_pFileNameSrc(expr._pFileNameSrc->Reference()), _lineNo(expr._lineNo)
{
	// don't copy _pExprDict, which should be different for each Expr instance.
}

Expr::~Expr()
{
}

bool Expr::IsTypeLabel(const char *symbol) const
{
	return IsTypeLabel() &&
		::strcasecmp(dynamic_cast<const Expr_Label *>(this)->GetSymbol(), symbol) == 0;
}

bool Expr::IsTypeSymbol(const char *symbol) const
{
	return IsTypeSymbol() &&
		::strcasecmp(dynamic_cast<const Expr_Symbol *>(this)->GetSymbol(), symbol) == 0;
}

bool Expr::IsTypeBinOp(const Operator *pOperator) const
{
	return IsTypeBinOp() &&
		dynamic_cast<const Expr_BinOp *>(this)->GetOperator()->IsIdentical(pOperator);
}

bool Expr::IsTypeDirective(const Directive *pDirective) const
{
	return IsTypeDirective() &&
		dynamic_cast<const Expr_Directive *>(this)->GetDirective()->IsIdentical(pDirective);
}

void Expr::Print() const
{
	::printf("%s\n", ComposeSource(false).c_str());
}

bool Expr::OnPhaseInclude(Context &context)
{
	return GetExprOperands().OnPhaseInclude(context) &&
		GetExprChildren().OnPhaseInclude(context);
}

bool Expr::OnPhaseDeclareMacro(Context &context)
{
	return GetExprOperands().OnPhaseDeclareMacro(context) &&
		GetExprChildren().OnPhaseDeclareMacro(context);
}

bool Expr::OnPhaseExpandMacro(Context &context)
{
	return GetExprOperands().OnPhaseExpandMacro(context) &&
		GetExprChildren().OnPhaseExpandMacro(context);
}

bool Expr::OnPhaseSetupExprDict(Context &context)
{
	_pExprDict.reset(context.GetExprDictCurrent().Reference());
	return GetExprOperands().OnPhaseSetupExprDict(context) &&
		GetExprChildren().OnPhaseSetupExprDict(context);
}

bool Expr::OnPhaseGenerate(Context &context, Binary *pBuffDst) const
{
	return GetExprOperands().OnPhaseGenerate(context, pBuffDst) &&
		GetExprChildren().OnPhaseGenerate(context, pBuffDst);
}

bool Expr::OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const
{
	return GetExprOperands().OnPhaseDisasm(context, disasmDumper, indentLevelCode) &&
		GetExprChildren().OnPhaseDisasm(context, disasmDumper, indentLevelCode);
}

//-----------------------------------------------------------------------------
// ExprList
//-----------------------------------------------------------------------------
Expr_Label *ExprList::SeekLabelToAssoc()
{
	if (empty() || !back()->IsTypeLabel()) return nullptr;
	Expr_Label *pExprLabel = dynamic_cast<Expr_Label *>(back());
	return pExprLabel->IsAssigned()? nullptr : pExprLabel;
}

String ExprList::ComposeSource(bool upperCaseFlag, const char *sep) const
{
	String rtn;
	for (auto pExpr : *this) {
		if (!rtn.empty()) rtn += sep;
		rtn += pExpr->ComposeSource(upperCaseFlag);
	}
	return rtn;
}

bool ExprList::OnPhaseInclude(Context &context)
{
	for (auto pExpr : *this) {
		if (!pExpr->OnPhaseInclude(context)) return false;
	}
	return true;
}

bool ExprList::OnPhaseDeclareMacro(Context &context)
{
	for (auto pExpr : *this) {
		if (!pExpr->OnPhaseDeclareMacro(context)) return false;
	}
	return true;
}

bool ExprList::OnPhaseExpandMacro(Context &context)
{
	for (auto pExpr : *this) {
		if (!pExpr->OnPhaseExpandMacro(context)) return false;
	}
	return true;
}

bool ExprList::OnPhaseSetupExprDict(Context &context)
{
	for (auto pExpr : *this) {
		if (!pExpr->OnPhaseSetupExprDict(context)) return false;
	}
	return true;
}

bool ExprList::OnPhaseGenerate(Context &context, Binary *pBuffDst) const
{
	for (auto pExpr : *this) {
		if (!pExpr->OnPhaseGenerate(context, pBuffDst)) return false;
	}
	return true;
}

bool ExprList::OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const
{
	for (auto pExpr : *this) {
		if (!pExpr->OnPhaseDisasm(context, disasmDumper, indentLevelCode)) return false;
	}
	return true;
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

ExprOwner *ExprOwner::Clone() const
{
	AutoPtr<ExprOwner> pExprOwner(new ExprOwner());
	pExprOwner->reserve(size());
	for (auto pExpr : *this) {
		pExprOwner->push_back(pExpr->Clone());
	}
	return pExprOwner.release();
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

void ExprDict::Assign(const String &symbol, Expr *pExpr, bool forceGlobalFlag)
{
	ExprDict *pExprDict = forceGlobalFlag? GetGlobal() : this;
	pExprDict->insert(std::make_pair(symbol, pExpr));
}

bool ExprDict::IsAssigned(const char *symbol) const
{
	return find(symbol) != end();
}

const Expr *ExprDict::Lookup(const char *symbol) const
{
	const_iterator iter = find(symbol);
	if (iter != end()) return iter->second;
	return _pExprDictParent.IsNull()? nullptr : _pExprDictParent->Lookup(symbol);
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

Expr *Expr_Root::Resolve(Context &context) const
{
	return Reference();
}

Expr *Expr_Root::Clone() const
{
	return new Expr_Root(*this);
}

Expr *Expr_Root::Substitute(const ExprDict &exprDict) const
{
	AutoPtr<Expr> pExprRtn(new Expr_Root(
							   GetExprOperands().Substitute(exprDict),
							   GetExprChildren().Substitute(exprDict)));
	pExprRtn->DeriveSourceInfo(this);
	return pExprRtn.release();
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

Expr *Expr_Number::Clone() const
{
	return new Expr_Number(*this);
}

Expr *Expr_Number::Substitute(const ExprDict &exprDict) const
{
	return Clone();
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

Expr *Expr_String::Clone() const
{
	return new Expr_String(*this);
}

Expr *Expr_String::Substitute(const ExprDict &exprDict) const
{
	return Clone();
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

Expr *Expr_BitPattern::Clone() const
{
	return new Expr_BitPattern(*this);
}

Expr *Expr_BitPattern::Substitute(const ExprDict &exprDict) const
{
	return Clone();
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

Expr *Expr_BinOp::Clone() const
{
	return new Expr_BinOp(*this);
}

Expr *Expr_BinOp::Substitute(const ExprDict &exprDict) const
{
	AutoPtr<Expr> pExprRtn(new Expr_BinOp(
							   GetOperator(),
							   GetExprOperands().Substitute(exprDict),
							   GetExprChildren().Substitute(exprDict)));
	pExprRtn->DeriveSourceInfo(this);
	return pExprRtn.release();
}

//-----------------------------------------------------------------------------
// Expr_Bracket
//-----------------------------------------------------------------------------
const Expr::Type Expr_Bracket::TYPE = Expr::TYPE_Bracket;

String Expr_Bracket::ComposeSource(bool upperCaseFlag) const
{
	String str;
	str = "[";
	str += GetExprOperands().ComposeSource(upperCaseFlag, ",");
	str += "]";
	return str;
}

Expr *Expr_Bracket::Resolve(Context &context) const
{
	AutoPtr<Expr_Bracket> pExprRtn(new Expr_Bracket());
	pExprRtn->DeriveSourceInfo(this);
	for (auto pExprOperand : GetExprOperands()) {
		AutoPtr<Expr> pExprOperandResolved(pExprOperand->Resolve(context));
		if (pExprOperandResolved.IsNull()) return nullptr;
		pExprRtn->GetExprOperands().push_back(pExprOperandResolved.release());
	}
	return pExprRtn.release();
}

Expr *Expr_Bracket::Clone() const
{
	return new Expr_Bracket(*this);
}

Expr *Expr_Bracket::Substitute(const ExprDict &exprDict) const
{
	AutoPtr<Expr> pExprRtn(new Expr_Bracket(
							   GetExprOperands().Substitute(exprDict),
							   GetExprChildren().Substitute(exprDict)));
	pExprRtn->DeriveSourceInfo(this);
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
	str += GetExprOperands().ComposeSource(upperCaseFlag, ",");
	str += "}";
	return str;
}

Expr *Expr_Brace::Resolve(Context &context) const
{
	AutoPtr<Expr_Brace> pExprRtn(new Expr_Brace());
	pExprRtn->DeriveSourceInfo(this);
	for (auto pExprOperand : GetExprOperands()) {
		AutoPtr<Expr> pExprOperandResolved(pExprOperand->Resolve(context));
		if (pExprOperandResolved.IsNull()) return nullptr;
		pExprRtn->GetExprOperands().push_back(pExprOperandResolved.release());
	}
	return pExprRtn.release();
}

Expr *Expr_Brace::Clone() const
{
	return new Expr_Brace(*this);
}

Expr *Expr_Brace::Substitute(const ExprDict &exprDict) const
{
	AutoPtr<Expr> pExprRtn(new Expr_Brace(
							   GetExprOperands().Substitute(exprDict),
							   GetExprChildren().Substitute(exprDict)));
	pExprRtn->DeriveSourceInfo(this);
	return pExprRtn.release();
}

//-----------------------------------------------------------------------------
// Expr_Label
//-----------------------------------------------------------------------------
const Expr::Type Expr_Label::TYPE = Expr::TYPE_Label;

#if 0
bool Expr_Label::OnPhaseDeclareMacro(Context &context)
{
	return IsAssigned()? GetAssigned()->OnPhaseDeclareMacro(context) : true;
}
#endif

bool Expr_Label::OnPhaseSetupExprDict(Context &context)
{
	if (!Expr::OnPhaseSetupExprDict(context)) return false;
	if (_pExprDict->IsAssigned(GetSymbol())) {
		ErrorLog::AddError(this, "duplicated definition of symbol: %s", GetSymbol());
		return false;
	}
	if (IsAssigned()) {
		_pExprDict->Assign(GetSymbol(), GetAssigned()->Reference(), _forceGlobalFlag);
	} else {
		AutoPtr<Expr> pExpr(new Expr_Number(context.GetAddress()));
		pExpr->DeriveSourceInfo(this);
		_pExprDict->Assign(GetSymbol(), pExpr.release(), _forceGlobalFlag);
	}
	return true;
}

#if 0
bool Expr_Label::OnPhaseGenerate(Context &context, Binary *pBuffDst) const
{
	// nothing to do
	return true;
}
#endif

bool Expr_Label::OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const
{
	if (IsAssigned() && GetAssigned()->IsTypeMacroDecl()) {
		return GetAssigned()->OnPhaseDisasm(context, disasmDumper, indentLevelCode);
	}
	String strLabel = MakeSource(_symbol.c_str(), _forceGlobalFlag);
	if (IsAssigned()) {
		disasmDumper.DumpLabelAndCode(
			strLabel.c_str(),
			GetAssigned()->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	} else {
		disasmDumper.DumpLabel(strLabel.c_str(), indentLevelCode);
	}
	return true;
}

Expr *Expr_Label::Resolve(Context &context) const
{
	return Reference();
}

Expr *Expr_Label::Clone() const
{
	return new Expr_Label(*this);
}

Expr *Expr_Label::Substitute(const ExprDict &exprDict) const
{
	return Clone();
}

String Expr_Label::ComposeSource(bool upperCaseFlag) const
{
	String str = MakeSource(_symbol.c_str(), _forceGlobalFlag);
	return str;
}

String Expr_Label::MakeSource(const char *symbol, bool forceGlobalFlag)
{
	String str;
	str = symbol;
	str += forceGlobalFlag? "::" : ":";
	return str;
}

//-----------------------------------------------------------------------------
// Expr_Symbol
//-----------------------------------------------------------------------------
const Expr::Type Expr_Symbol::TYPE = Expr::TYPE_Symbol;

Expr *Expr_Symbol::Resolve(Context &context) const
{
	if (context.CheckCircularReference(this)) return nullptr;
	if (Generator::GetInstance().IsRegisterSymbol(GetSymbol())) return Reference();
	if (!context.IsPhase(Context::PHASE_Generate)) {
		AutoPtr<Expr> pExprRtn(new Expr_Number(0));
		pExprRtn->DeriveSourceInfo(this);
		return pExprRtn.release();
	}
	//::printf("ref: %p\n", _pExprDict.get());
	const Expr *pExpr = _pExprDict->Lookup(GetSymbol());
	if (pExpr == nullptr) {
		ErrorLog::AddError(this, "undefined symbol: %s", GetSymbol());
		return nullptr;
	}
	AutoPtr<Expr> pExprResolved(pExpr->Resolve(context));
	if (pExprResolved.IsNull()) return nullptr;
	if (!pExprResolved->IsTypeNumber()) {
		ErrorLog::AddError(this, "symbol %s must be associated with a number", GetSymbol());
		return nullptr;
	}
	return pExprResolved.release();
}

Expr *Expr_Symbol::Clone() const
{
	return new Expr_Symbol(*this);
}

Expr *Expr_Symbol::Substitute(const ExprDict &exprDict) const
{
	const Expr *pExpr = exprDict.Lookup(GetSymbol());
	return (pExpr == nullptr)? Clone() : pExpr->Clone();
}

String Expr_Symbol::ComposeSource(bool upperCaseFlag) const
{
	if (Generator::GetInstance().IsRegisterSymbol(_symbol.c_str())) {
		return upperCaseFlag? ToUpper(_symbol.c_str()) : ToLower(_symbol.c_str());
	}
	return _symbol;
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
	const ExprList &exprList = GetExprOperands();
	StringList::const_iterator pParamName = paramNames.begin();
	ExprList::const_iterator ppExpr = exprList.begin();
	for ( ; pParamName != paramNames.end() && ppExpr != exprList.end(); pParamName++, ppExpr++) {
		const String &paramName = *pParamName;
		const Expr *pExpr = *ppExpr;
		pExprDict->Assign(paramName, pExpr->Reference(), false);
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
		context.PushLocalExprDict();	// .PROC
		for (auto pExpr : *_pExprsExpanded) {
			//pExpr->Print();
			if (!(rtn = pExpr->OnPhaseSetupExprDict(context))) break;
			//::printf("%p\n", pExpr->GetExprDict());
		}
		context.PopLocalExprDict();		// .ENDP
	}
	return rtn;
}

bool Expr_Instruction::OnPhaseGenerate(Context &context, Binary *pBuffDst) const
{
	bool rtn = true;
	if (_pExprsExpanded.IsNull()) {
		rtn = Generator::GetInstance().Generate(context, this, pBuffDst);
	} else {
		for (auto pExpr : *_pExprsExpanded) {
			if (!(rtn = pExpr->OnPhaseGenerate(context, pBuffDst))) break;
		}
	}
	return rtn;
}

bool Expr_Instruction::OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const
{
	bool rtn = true;
	bool upperCaseFlag = disasmDumper.GetUpperCaseFlag();
	if (_pExprsExpanded.IsNull()) {
		Binary buffDst;
		UInt32 addr = context.GetAddress();
		if ((rtn = Generator::GetInstance().Generate(context, this, &buffDst))) {
			disasmDumper.DumpDataAndCode(addr, buffDst, ComposeSource(upperCaseFlag).c_str(), indentLevelCode);
		}
	} else {
		disasmDumper.DumpCode(ComposeSource(upperCaseFlag).c_str(), indentLevelCode);
		for (auto pExpr : *_pExprsExpanded) {
			if (!(rtn = pExpr->OnPhaseDisasm(context, disasmDumper, indentLevelCode + 1))) break;
		}
	}
	return rtn;
}

Expr *Expr_Instruction::Resolve(Context &context) const
{
	return Reference();
}

Expr *Expr_Instruction::Clone() const
{
	return new Expr_Instruction(*this);
}

Expr *Expr_Instruction::Substitute(const ExprDict &exprDict) const
{
	AutoPtr<Expr> pExprRtn(new Expr_Instruction(
							   GetSymbol(),
							   GetExprOperands().Substitute(exprDict),
							   GetExprChildren().Substitute(exprDict)));
	pExprRtn->DeriveSourceInfo(this);
	return pExprRtn.release();
}

String Expr_Instruction::ComposeSource(bool upperCaseFlag) const
{
	String str = JustifyLeft(
		(upperCaseFlag? ToUpper(_symbol.c_str()) : ToLower(_symbol.c_str())).c_str(),
		Generator::GetInstance().GetInstNameLenMax());
	str += " ";
	str += GetExprOperands().ComposeSource(upperCaseFlag, ",");
	return str;
}

//-----------------------------------------------------------------------------
// Expr_Directive
//-----------------------------------------------------------------------------
const Expr::Type Expr_Directive::TYPE = Expr::TYPE_Directive;

bool Expr_Directive::OnPhaseInclude(Context &context)
{
	if (!Expr::OnPhaseInclude(context)) return false;
	return _pDirective->OnPhaseInclude(context, this);
}

bool Expr_Directive::OnPhaseDeclareMacro(Context &context)
{
	if (!Expr::OnPhaseDeclareMacro(context)) return false;
	return _pDirective->OnPhaseDeclareMacro(context, this);
}

bool Expr_Directive::OnPhaseExpandMacro(Context &context)
{
	if (!Expr::OnPhaseExpandMacro(context)) return false;
	return _pDirective->OnPhaseExpandMacro(context, this);
}

bool Expr_Directive::OnPhaseSetupExprDict(Context &context)
{
	if (!Expr::OnPhaseSetupExprDict(context)) return false;
	return _pDirective->OnPhaseSetupExprDict(context, this);
}

bool Expr_Directive::OnPhaseGenerate(Context &context, Binary *pBuffDst) const
{
	if (!Expr::OnPhaseGenerate(context, pBuffDst)) return false;
	return _pDirective->OnPhaseGenerate(context, this, pBuffDst);
}

bool Expr_Directive::OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const
{
	if (!Expr::OnPhaseDisasm(context, disasmDumper, indentLevelCode)) return false;
	return _pDirective->OnPhaseDisasm(context, this, disasmDumper, indentLevelCode);
}

Expr *Expr_Directive::Resolve(Context &context) const
{
	return _pDirective->Resolve(context, this);
}

Expr *Expr_Directive::Clone() const
{
	AutoPtr<Expr> pExprRtn(new Expr_Directive(*this));
	pExprRtn->DeriveSourceInfo(this);
	return pExprRtn.release();
}

Expr *Expr_Directive::Substitute(const ExprDict &exprDict) const
{
	AutoPtr<Expr> pExprRtn(new Expr_Directive(
							   GetDirective(),
							   GetExprOperands().Substitute(exprDict),
							   GetExprChildren().Substitute(exprDict)));
	pExprRtn->DeriveSourceInfo(this);
	return pExprRtn.release();
}

String Expr_Directive::ComposeSource(bool upperCaseFlag) const
{
	const char *symbol = _pDirective->GetSymbol();
	String str = JustifyLeft(
		(upperCaseFlag? ToUpper(symbol) : ToLower(symbol)).c_str(),
		Generator::GetInstance().GetInstNameLenMax());
	str += " ";
	str += GetExprOperands().ComposeSource(upperCaseFlag, ",");
	return str;
}

//-----------------------------------------------------------------------------
// Expr_MacroDecl
//-----------------------------------------------------------------------------
const Expr::Type Expr_MacroDecl::TYPE = Expr::TYPE_MacroDecl;

bool Expr_MacroDecl::OnPhaseDeclareMacro(Context &context)
{
	const ExprList &operands = GetExprOperands();
	AutoPtr<Macro> pMacro(new Macro(_symbol, GetExprChildren().Reference()));
	StringList &paramNames = pMacro->GetParamNames();
	paramNames.reserve(operands.size());
	for (auto pExpr : operands) {
		if (!pExpr->IsTypeSymbol()) {
			ErrorLog::AddError(this, "directive .MACRO takes a list of parameter names");
			return false;
		}
		paramNames.push_back(dynamic_cast<const Expr_Symbol *>(pExpr)->GetSymbol());
	}
	context.GetMacroDict().Assign(pMacro.release());
	return true;
}

bool Expr_MacroDecl::OnPhaseSetupExprDict(Context &context)
{
	// suppress evaluation of children
	return true;
}

bool Expr_MacroDecl::OnPhaseGenerate(Context &context, Binary *pBuffDst) const
{
	// suppress evaluation of children
	return true;
}

bool Expr_MacroDecl::OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const
{
	// suppress evaluation of children
	return true;
}

Expr *Expr_MacroDecl::Resolve(Context &context) const
{
	return Reference();
}

Expr *Expr_MacroDecl::Clone() const
{
	return new Expr_MacroDecl(*this);
}

Expr *Expr_MacroDecl::Substitute(const ExprDict &exprDict) const
{
	return Clone();
}

String Expr_MacroDecl::ComposeSource(bool upperCaseFlag) const
{
	String str = upperCaseFlag? ".MACRO" : ".macro";
	str += ' ';
	str += GetExprOperands().ComposeSource(upperCaseFlag, ",");
	return str;
}
