//=============================================================================
// Directive.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
const Directive *Directive::CSEG		= nullptr;
const Directive *Directive::DB			= nullptr;
const Directive *Directive::DSEG		= nullptr;
const Directive *Directive::DW			= nullptr;
const Directive *Directive::ENDM		= nullptr;
const Directive *Directive::ENDP		= nullptr;
const Directive *Directive::ENDPCG		= nullptr;
const Directive *Directive::EQU			= nullptr;
const Directive *Directive::FILENAME_JR	= nullptr;
const Directive *Directive::INCLUDE		= nullptr;
const Directive *Directive::ISEG		= nullptr;
const Directive *Directive::MACRO		= nullptr;
const Directive *Directive::MML			= nullptr;
const Directive *Directive::ORG			= nullptr;
const Directive *Directive::PCG			= nullptr;
const Directive *Directive::PROC		= nullptr;

DirectiveDict Directive::_directiveDict;

Directive::~Directive()
{
}

void Directive::Initialize()
{
	_directiveDict.Assign(CSEG			= new Directive_CSEG());
	_directiveDict.Assign(DB			= new Directive_DB());
	_directiveDict.Assign(DSEG			= new Directive_DSEG());
	_directiveDict.Assign(DW			= new Directive_DW());
	_directiveDict.Assign(ENDM			= new Directive_ENDM());
	_directiveDict.Assign(ENDP			= new Directive_ENDP());
	_directiveDict.Assign(ENDPCG		= new Directive_ENDPCG());
	_directiveDict.Assign(EQU			= new Directive_EQU());
	_directiveDict.Assign(FILENAME_JR	= new Directive_FILENAME_JR());
	_directiveDict.Assign(INCLUDE		= new Directive_INCLUDE());
	_directiveDict.Assign(ISEG			= new Directive_ISEG());
	_directiveDict.Assign(MACRO			= new Directive_MACRO());
	_directiveDict.Assign(MML			= new Directive_MML());
	_directiveDict.Assign(ORG			= new Directive_ORG());
	_directiveDict.Assign(PCG			= new Directive_PCG());
	_directiveDict.Assign(PROC			= new Directive_PROC());
}

const Directive *Directive::Lookup(const char *symbol)
{
	return _directiveDict.Lookup(symbol);
}

bool Directive::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetExprChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr.release());
	return true;
}

bool Directive::OnPhaseInclude(Context &context, Expr_Directive *pExpr) const
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseDeclareMacro(Context &context, const Expr_Directive *pExpr) const
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseExpandMacro(Context &context, const Expr_Directive *pExpr) const
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseAssignSymbol(Context &context, const Expr_Directive *pExpr) const
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
							  DisasmDumper &disasmDumper, int indentLevelCode) const
{
	Binary buffDst;
	UInt32 addr = context.GetAddress();
	if (!OnPhaseGenerate(context, pExpr, &buffDst)) return false;
	disasmDumper.DumpDataAndCode(
		addr, buffDst,
		pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	return true;
}

Expr *Directive::Resolve(Context &context, const Expr_Directive *pExpr) const
{
	return pExpr->Reference();
}

//-----------------------------------------------------------------------------
// DirectiveDict
//-----------------------------------------------------------------------------
void DirectiveDict::Assign(const Directive *pDirective)
{
	insert(std::make_pair(pDirective->GetSymbol(), pDirective));
}

const Directive *DirectiveDict::Lookup(const char *symbol) const
{
	const_iterator iter = find(symbol);
	return (iter == end())? nullptr : iter->second;
}

//-----------------------------------------------------------------------------
// Directive_CSEG
//-----------------------------------------------------------------------------
bool Directive_CSEG::OnPhaseAssignSymbol(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .CSEG needs no operands");
		return false;
	}
	context.SelectCodeSegment();
	return true;
}

bool Directive_CSEG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return OnPhaseAssignSymbol(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
bool Directive_DB::OnPhaseAssignSymbol(Context &context, const Expr_Directive *pExpr) const
{
	UInt32 bytes = 0;
	if (!Generate(context, pExpr, nullptr, &bytes)) return false;
	context.ForwardAddress(bytes);
	return true;
}

bool Directive_DB::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	if (!context.CheckSegmentRegionReady()) return false;
	if (pBuffDst == nullptr) pBuffDst = &context.GetSegmentBuffer();
	UInt32 bytes = 0;
	if (!Generate(context, pExpr, pBuffDst, &bytes)) return false;
	context.ForwardAddress(bytes);
	return true;
}

bool Directive_DB::Generate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst, UInt32 *pBytes)
{
	UInt32 bytes = 0;
	for (auto pExprOperand : pExpr->GetExprOperands()) {
		context.StartToResolve();
		AutoPtr<Expr> pExprResolved(pExprOperand->Resolve(context));
		if (pExprResolved.IsNull()) return false;
		if (pExprResolved->IsTypeNumber()) {
			UInt32 num = dynamic_cast<Expr_Number *>(pExprResolved.get())->GetNumber();
			if (num > 0xff) {
				ErrorLog::AddError(pExpr, "an element value of directive .DB exceeds 8-bit range");
				return false;
			}
			if (pBuffDst != nullptr) *pBuffDst += static_cast<UInt8>(num);
			bytes++;
		} else if (pExprResolved->IsTypeString()) {
			const String &str = dynamic_cast<Expr_String *>(pExprResolved.get())->GetStringSTL();
			for (auto ch : str) {
				if (pBuffDst != nullptr) *pBuffDst += static_cast<UInt8>(ch);
				bytes++;
			}
		} else if (pExprResolved->IsTypeBitPattern()) {
			Expr_BitPattern *pExprEx = dynamic_cast<Expr_BitPattern *>(pExprResolved.get());
			size_t len = pExprEx->GetBitPatternLen();
			if (len % 8 != 0) {
				ErrorLog::AddError(pExpr, "the bit pattern length is %zu, not a multiple of eight", len);
				return false;
			}
			Binary buff = pExprEx->GetBinary();
			if (pBuffDst != nullptr) *pBuffDst += buff;
			bytes += static_cast<UInt32>(buff.size());
		} else {
			ErrorLog::AddError(pExpr, "elements of directive .DB must be number or string value");
			return false;
		}
	}
	*pBytes = bytes;
	return true;
}

//-----------------------------------------------------------------------------
// Directive_DSEG
//-----------------------------------------------------------------------------
bool Directive_DSEG::OnPhaseAssignSymbol(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .DSEG needs no operands");
		return false;
	}
	context.SelectDataSegment();
	return true;
}

bool Directive_DSEG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return OnPhaseAssignSymbol(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
bool Directive_DW::OnPhaseAssignSymbol(Context &context, const Expr_Directive *pExpr) const
{
	context.ForwardAddress(static_cast<UInt32>(pExpr->GetExprOperands().size() * sizeof(UInt16)));
	return true;
}

bool Directive_DW::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	if (!context.CheckSegmentRegionReady()) return false;
	if (pBuffDst == nullptr) pBuffDst = &context.GetSegmentBuffer();
	for (auto pExprOperand : pExpr->GetExprOperands()) {
		context.StartToResolve();
		AutoPtr<Expr> pExprResolved(pExprOperand->Resolve(context));
		if (pExprResolved.IsNull()) return false;
		if (!pExprResolved->IsTypeNumber()) {
			ErrorLog::AddError(pExpr, "elements of directive .DW must be number value");
			return false;
		}
		UInt32 num = dynamic_cast<Expr_Number *>(pExprResolved.get())->GetNumber();
		if (num > 0xffff) {
			ErrorLog::AddError(pExpr, "an element value of directive .DW exceeds 16-bit range");
			return false;
		}
		(*pBuffDst) += static_cast<UInt8>(num >> 8);
		(*pBuffDst) += static_cast<UInt8>(num);
	}
	size_t bytes = pExpr->GetExprOperands().size() * sizeof(UInt16);
	context.ForwardAddress(static_cast<UInt32>(bytes));
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ENDM
//-----------------------------------------------------------------------------
bool Directive_ENDM::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	if (!exprStack.back()->IsTypeDirective(Directive::MACRO)) {
		pParser->AddError("no matching .MACRO directive");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetExprChildren().push_back(pExpr->Reference());
	Expr::Delete(exprStack.back());
	exprStack.pop_back();	// remove the Expr_Directive instance from the stack
	exprStack.push_back(pExpr.release());
	return true;
}

bool Directive_ENDM::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ENDM needs no operands");
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ENDP
//-----------------------------------------------------------------------------
bool Directive_ENDP::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	if (!exprStack.back()->IsTypeDirective(Directive::PROC)) {
		pParser->AddError("no matching .PROC directive");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetExprChildren().push_back(pExpr->Reference());
	Expr::Delete(exprStack.back());
	exprStack.pop_back();	// remove the Expr_Directive instance from the stack
	exprStack.push_back(pExpr.release());
	return true;
}

bool Directive_ENDP::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ENDP needs no operands");
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ENDPCG
//-----------------------------------------------------------------------------
bool Directive_ENDPCG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ENDPCG needs no operands");
		return false;
	}
	if (!context.CheckSegmentRegionReady()) return false;
	return true;
}

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
bool Directive_EQU::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	Expr_Label *pExprLabel = exprStack.back()->GetExprChildren().SeekLabelToAssoc();
	if (pExprLabel == nullptr) {
		pParser->AddError("directive .EQU must be preceded by a symbol");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	pExprLabel->SetAssigned(pExpr->Reference());	// associate it to the preceding symbol
	exprStack.push_back(pExpr.release());
	return true;
}

Expr *Directive_EQU::Resolve(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .EQU takes one operand");
		return nullptr;
	}
	return exprOperands.back()->Resolve(context);
}

//-----------------------------------------------------------------------------
// Directive_FILENAME_JR
//-----------------------------------------------------------------------------
bool Directive_FILENAME_JR::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .FILENAME.JR takes one operand");
		return false;
	}
	context.StartToResolve();
	AutoPtr<Expr> pExprLast(exprOperands.back()->Resolve(context));
	if (pExprLast.IsNull()) return false;
	if (!pExprLast->IsTypeString()) {
		ErrorLog::AddError(pExpr, "directive .FIENAME.JR takes a string value as its operand");
		return false;
	}
	const char *fileNameJR = dynamic_cast<const Expr_String *>(pExprLast.get())->GetString();
	if (::strlen(fileNameJR) > 16) {
		ErrorLog::AddError(pExpr, "the length of FILENAME.JR must be up to 16 characters");
		return false;
	}
	context.SetFileNameJR(fileNameJR);
	return true;
}

//-----------------------------------------------------------------------------
// Directive_INCLUDE
//-----------------------------------------------------------------------------
bool Directive_INCLUDE::OnPhaseInclude(Context &context, Expr_Directive *pExpr) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .INCLUDE takes one operand");
		return false;
	}
	const Expr *pExprLast = exprOperands.back();
	if (!pExprLast->IsTypeString()) {
		ErrorLog::AddError(pExpr, "directive .INCLUDE takes a string value as its operand");
		return false;
	}
	const char *fileNameIncluded = dynamic_cast<const Expr_String *>(pExprLast)->GetString();
	String pathNameIncluded = JoinPathName(context.GetDirNameSrc(), fileNameIncluded);
	Parser parser(pathNameIncluded);
	if (!parser.ParseFile()) return false;
	AutoPtr<Expr> pExprRoot(parser.GetRoot()->Reference());
	if (!pExprRoot->OnPhaseInclude(context)) return false;
	pExpr->SetExprIncluded(pExprRoot.release());
	return true;
}

bool Directive_INCLUDE::OnPhaseDeclareMacro(Context &context, const Expr_Directive *pExpr) const
{
	return pExpr->GetExprIncluded()->OnPhaseDeclareMacro(context);
}

bool Directive_INCLUDE::OnPhaseExpandMacro(Context &context, const Expr_Directive *pExpr) const
{
	return pExpr->GetExprIncluded()->OnPhaseExpandMacro(context);
}

bool Directive_INCLUDE::OnPhaseAssignSymbol(Context &context, const Expr_Directive *pExpr) const
{
	return pExpr->GetExprIncluded()->OnPhaseAssignSymbol(context);
}

bool Directive_INCLUDE::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return pExpr->GetExprIncluded()->OnPhaseGenerate(context, pBuffDst);
}

bool Directive_INCLUDE::OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
									  DisasmDumper &disasmDumper, int indentLevelCode) const
{
	disasmDumper.DumpCode(pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	return pExpr->GetExprIncluded()->OnPhaseDisasm(context, disasmDumper, indentLevelCode + 1);
}

//-----------------------------------------------------------------------------
// Directive_ISEG
//-----------------------------------------------------------------------------
bool Directive_ISEG::OnPhaseAssignSymbol(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ISEG needs no operands");
		return false;
	}
	context.SelectInternalSegment();
	return true;
}

bool Directive_ISEG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return OnPhaseAssignSymbol(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
bool Directive_MACRO::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	Expr_Label *pExprLabel = exprStack.back()->GetExprChildren().SeekLabelToAssoc();
	if (pExprLabel == nullptr) {
		pParser->AddError("directive .MACRO must be preceded by a label");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Directive::MACRO));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	pExpr->SetSymbol(pExprLabel->GetSymbol(), pExprLabel->GetForceGlobalFlag());
	pExprLabel->SetAssigned(pExpr->Reference());	// associate it to the preceding symbol
	exprStack.push_back(pExpr->Reference());		// for children
	exprStack.push_back(pExpr.release());			// for operands
	return true;
}

bool Directive_MACRO::OnPhaseDeclareMacro(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	AutoPtr<Macro> pMacro(new Macro(pExpr->GetSymbol(), pExpr->GetExprChildren().Reference()));
	StringList &paramNames = pMacro->GetParamNames();
	paramNames.reserve(exprOperands.size());
	for (auto pExpr : exprOperands) {
		if (!pExpr->IsTypeSymbol()) {
			ErrorLog::AddError(pExpr, "directive .MACRO takes a list of parameter names");
			return false;
		}
		paramNames.push_back(dynamic_cast<const Expr_Symbol *>(pExpr)->GetSymbol());
	}
	context.GetMacroDict().Assign(pMacro.release());
	return true;
}

bool Directive_MACRO::OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
									DisasmDumper &disasmDumper, int indentLevelCode) const
{
	// suppress disasm dump
	return true;
}

//-----------------------------------------------------------------------------
// Directive_MML
//-----------------------------------------------------------------------------
bool Directive_MML::OnPhaseAssignSymbol(Context &context, const Expr_Directive *pExpr) const
{
	//Handler handler(nullptr);
	return true;
}

bool Directive_MML::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	if (!context.CheckSegmentRegionReady()) return false;
	if (pBuffDst == nullptr) pBuffDst = &context.GetSegmentBuffer();
	Handler handler(pBuffDst);
	MmlParser parser(handler);
	parser.Reset();
	for (auto pExprOperand : pExpr->GetExprOperands()) {
		context.StartToResolve();
		AutoPtr<Expr> pExprResolved(pExprOperand->Resolve(context));
		if (pExprResolved.IsNull()) return false;
		if (!pExprResolved->IsTypeString()) {
			ErrorLog::AddError(pExpr, "elements of directive .MML must be string value");
			return false;
		}
		const char *str = dynamic_cast<Expr_String *>(pExprResolved.get())->GetString();
		for (const char *p = str; ; p++) {
			if (!parser.FeedChar(*p)) {
				ErrorLog::AddError(pExpr, "invalid MML format");
				return false;
			}
			if (*p == '\0') break;
		}
	}
	context.ForwardAddress(handler.GetBytesSum());
	return true;
}

void Directive_MML::Handler::MmlNote(MmlParser &parser, unsigned char note, int length)
{
	UInt8 lengthDev = static_cast<UInt8>(0x60 * length / 256);
	UInt8 noteDev = note + 0x0d - 0x30;
	if (_pBuffDst != nullptr) {
		*_pBuffDst += lengthDev;
		*_pBuffDst += noteDev;
	}
	_bytesSum += 2;
}

void Directive_MML::Handler::MmlRest(MmlParser &parser, int length)
{
	UInt8 lengthDev = static_cast<UInt8>(0x60 * length / 256);
	UInt8 noteDev = 0x00;
	if (_pBuffDst != nullptr) {
		*_pBuffDst += lengthDev;
		*_pBuffDst += noteDev;
	}
	_bytesSum += 2;
}

//-----------------------------------------------------------------------------
// Directive_ORG
//-----------------------------------------------------------------------------
bool Directive_ORG::OnPhaseAssignSymbol(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .ORG takes one operand");
		return false;
	}
	context.StartToResolve();
	AutoPtr<Expr> pExprLast(exprOperands.back()->Resolve(context));
	if (pExprLast.IsNull()) return false;
	if (!pExprLast->IsTypeNumber()) {
		ErrorLog::AddError(pExpr, "directive .ORG takes a number value as its operand");
		return false;
	}
	UInt32 num = dynamic_cast<const Expr_Number *>(pExprLast.get())->GetNumber();
	if (num > 0xffff) {
		ErrorLog::AddError(pExpr, "address value exceeds 16-bit range");
		return false;
	}
	context.StartRegion(static_cast<UInt16>(num));
	return true;
}

bool Directive_ORG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return OnPhaseAssignSymbol(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
bool Directive_PCG::OnPhaseAssignSymbol(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_PCG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	if (!context.CheckSegmentRegionReady()) return false;
	return true;
}

bool Directive_PCG::OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
								  DisasmDumper &disasmDumper, int indentLevelCode) const
{
	// suppress disasm dump
	return true;
}

//-----------------------------------------------------------------------------
// Directive_PROC
//-----------------------------------------------------------------------------
bool Directive_PROC::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetExprChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr->Reference());		// for children
	exprStack.push_back(pExpr.release());			// for operands
	return true;
}

bool Directive_PROC::OnPhaseAssignSymbol(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .PROC needs no operands");
		return false;
	}
	context.PushLocalExprDict();
	bool rtn = const_cast<Expr_Directive *>(pExpr)->GetExprChildren().OnPhaseAssignSymbol(context);
	context.PopLocalExprDict();
	return rtn;
}

bool Directive_PROC::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return pExpr->GetExprChildren().OnPhaseGenerate(context, pBuffDst);
}

bool Directive_PROC::OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
								   DisasmDumper &disasmDumper, int indentLevelCode) const
{
	return pExpr->GetExprChildren().OnPhaseDisasm(context, disasmDumper, indentLevelCode);
}
