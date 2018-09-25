//=============================================================================
// Directive.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
Directive::~Directive()
{
}

bool Directive::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr.release());
	return true;
}

bool Directive::OnPhaseInclude(Context &context, const Expr_Directive *pExpr) const
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

bool Directive::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	// nothing to do
	return true;
}

Expr *Directive::Resolve(Context &context, const Expr_Directive *pExpr) const
{
	return pExpr->Reference();
}

//-----------------------------------------------------------------------------
// DirectiveList
//-----------------------------------------------------------------------------
const Directive *DirectiveList::FindBySymbol(const char *symbol) const
{
	for (auto pDirective : *this) {
		if (strcasecmp(pDirective->GetSymbol(), symbol) == 0) return pDirective;
	}
	return nullptr;
}

//-----------------------------------------------------------------------------
// DirectiveOwner
//-----------------------------------------------------------------------------
DirectiveOwner::~DirectiveOwner()
{
}

void DirectiveOwner::Clear()
{
	for (auto pDirective : *this) {
		Directive::Delete(pDirective);
	}
	clear();
}

//-----------------------------------------------------------------------------
// Directive_CSEG
//-----------------------------------------------------------------------------
bool Directive_CSEG::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .CSEG needs no operands");
		return false;
	}
	context.SelectCodeSegment();
	return true;
}

bool Directive_CSEG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return OnPhaseSetupLookup(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
bool Directive_DB::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	return DoGenerate(context, pExpr, nullptr);
}

bool Directive_DB::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	if (!context.CheckRegionReady()) return false;
	return DoGenerate(context, pExpr, &buffDst);
}

bool Directive_DB::DoGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	UInt32 bytes = 0;
	for (auto pExprData : pExpr->GetOperands()) {
		context.StartToResolve();
		AutoPtr<Expr> pExprResolved(pExprData->Resolve(context));
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
	context.ForwardAddress(static_cast<UInt32>(bytes));
	return true;
}

//-----------------------------------------------------------------------------
// Directive_DSEG
//-----------------------------------------------------------------------------
bool Directive_DSEG::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .DSEG needs no operands");
		return false;
	}
	context.SelectDataSegment();
	return true;
}

bool Directive_DSEG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return OnPhaseSetupLookup(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
bool Directive_DW::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	context.ForwardAddress(static_cast<UInt32>(pExpr->GetOperands().size() * sizeof(UInt16)));
	return true;
}

bool Directive_DW::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	if (!context.CheckRegionReady()) return false;
	for (auto pExprData : pExpr->GetOperands()) {
		context.StartToResolve();
		AutoPtr<Expr> pExprResolved(pExprData->Resolve(context));
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
		buffDst += static_cast<UInt8>(num >> 8);
		buffDst += static_cast<UInt8>(num);
	}
	size_t bytes = pExpr->GetOperands().size() * sizeof(UInt16);
	context.ForwardAddress(static_cast<UInt32>(bytes));
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ENDM
//-----------------------------------------------------------------------------
bool Directive_ENDM::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	if (!exprStack.back()->IsTypeMacroBody()) {
		pParser->AddError("no matching .MACRO directive");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetChildren().push_back(pExpr->Reference());
	Expr::Delete(exprStack.back());
	exprStack.pop_back();	// remove the EXPR_MacroBody instance from the stack
	exprStack.push_back(pExpr.release());
	return true;
}

bool Directive_ENDM::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ENDM needs no operands");
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ENDP
//-----------------------------------------------------------------------------
bool Directive_ENDP::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ENDP needs no operands");
		return false;
	}
	if (!context.DoesExistLocalLookupTable()) {
		ErrorLog::AddError(pExpr, "no matching .PROC directive");
		return false;
	}
	context.PopLocalLookupTable();
	return true;
}

bool Directive_ENDP::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return OnPhaseSetupLookup(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_ENDPCG
//-----------------------------------------------------------------------------
bool Directive_ENDPCG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ENDPCG needs no operands");
		return false;
	}
	if (!context.CheckRegionReady()) return false;
	return true;
}

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
bool Directive_EQU::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	Expr_LabelDef *pExprLabelDef = exprStack.back()->GetChildren().SeekLabelDefToAssoc();
	if (pExprLabelDef == nullptr) {
		pParser->AddError("directive .EQU must be preceded by a label");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	pExprLabelDef->SetAssigned(pExpr->Reference());	// associate it to the preceding label
	exprStack.push_back(pExpr.release());
	return true;
}

bool Directive_EQU::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	// nothing to do
	return true;
}

Expr *Directive_EQU::Resolve(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (operands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .EQU takes one operand");
		return nullptr;
	}
	return operands.back()->Resolve(context);
}

//-----------------------------------------------------------------------------
// Directive_FILENAME_JR
//-----------------------------------------------------------------------------
bool Directive_FILENAME_JR::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (operands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .FILENAME.JR takes one operand");
		return false;
	}
	context.StartToResolve();
	AutoPtr<Expr> pExprLast(operands.back()->Resolve(context));
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
bool Directive_INCLUDE::OnPhaseInclude(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_INCLUDE::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ISEG
//-----------------------------------------------------------------------------
bool Directive_ISEG::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ISEG needs no operands");
		return false;
	}
	context.SelectInternalSegment();
	return true;
}

bool Directive_ISEG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return OnPhaseSetupLookup(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
bool Directive_MACRO::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	AutoPtr<Expr_MacroEntry> pExpr(new Expr_MacroEntry());
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr->GetMacroBody()->Reference());	// for directives in the body
	exprStack.push_back(pExpr.release());						// for operands
	return true;
}

bool Directive_MACRO::OnPhaseDeclareMacro(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_MACRO::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_MML
//-----------------------------------------------------------------------------
bool Directive_MML::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	//Handler handler(nullptr);
	return true;
}

bool Directive_MML::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	if (!context.CheckRegionReady()) return false;
	Handler handler(&buffDst);
	MmlParser parser(handler);
	parser.Reset();
	for (auto pExprData : pExpr->GetOperands()) {
		context.StartToResolve();
		AutoPtr<Expr> pExprResolved(pExprData->Resolve(context));
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
bool Directive_ORG::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (operands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .ORG takes one operand");
		return false;
	}
	context.StartToResolve();
	AutoPtr<Expr> pExprLast(operands.back()->Resolve(context));
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

bool Directive_ORG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return OnPhaseSetupLookup(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
bool Directive_PCG::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_PCG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	if (!context.CheckRegionReady()) return false;
	return true;
}

//-----------------------------------------------------------------------------
// Directive_PROC
//-----------------------------------------------------------------------------
bool Directive_PROC::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
#if 0
	Expr_LabelDef *pExprLabelDef = exprStack.back()->GetChildren().SeekLabelDefToAssoc();
	if (pExprLabelDef != nullptr) {

	}
#endif
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr.release());
	return true;
}

bool Directive_PROC::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .PROC needs no operands");
		return false;
	}
	context.PushLocalLookupTable();
	return true;
}

bool Directive_PROC::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return OnPhaseSetupLookup(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_MacroInstance
//-----------------------------------------------------------------------------
Directive_MacroInstance::Directive_MacroInstance(
	const String &symbol, StringList::const_iterator pParamName,
	StringList::const_iterator pParamNameEnd, Expr *pExprMacroBody) :
	Directive(symbol), _pExprMacroBody(pExprMacroBody)
{
	_paramNames.insert(_paramNames.end(), pParamName, pParamNameEnd);
}

bool Directive_MacroInstance::OnPhaseSetupLookup(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_MacroInstance::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	if (!context.CheckRegionReady()) return false;
	return true;
}
