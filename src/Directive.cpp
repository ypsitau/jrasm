//=============================================================================
// Directive.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
std::unique_ptr<DirectiveOwner> Directive::_pDirectivesBuiltIn;

Directive::~Directive()
{
}

bool Directive::CreateExpr(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr.release());
	return true;
}

void Directive::Initialize()
{
	_pDirectivesBuiltIn.reset(new DirectiveOwner());
	_pDirectivesBuiltIn->push_back(new Directive_CSEG());
	_pDirectivesBuiltIn->push_back(new Directive_DB());
	_pDirectivesBuiltIn->push_back(new Directive_DSEG());
	_pDirectivesBuiltIn->push_back(new Directive_DW());
	_pDirectivesBuiltIn->push_back(new Directive_ENDM());
	_pDirectivesBuiltIn->push_back(new Directive_ENDP());
	_pDirectivesBuiltIn->push_back(new Directive_ENDPCG());
	_pDirectivesBuiltIn->push_back(new Directive_EQU());
	_pDirectivesBuiltIn->push_back(new Directive_FILENAME_JR());
	_pDirectivesBuiltIn->push_back(new Directive_INCLUDE());
	_pDirectivesBuiltIn->push_back(new Directive_ISEG());
	_pDirectivesBuiltIn->push_back(new Directive_MACRO());
	_pDirectivesBuiltIn->push_back(new Directive_MML());
	_pDirectivesBuiltIn->push_back(new Directive_ORG());
	_pDirectivesBuiltIn->push_back(new Directive_PCG());
	_pDirectivesBuiltIn->push_back(new Directive_PROC());
}

const Directive *Directive::FindBuiltIn(const char *symbol)
{
	return _pDirectivesBuiltIn->FindBySymbol(symbol);
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
		delete pDirective;
	}
	clear();
}

//-----------------------------------------------------------------------------
// Directive_CSEG
//-----------------------------------------------------------------------------
bool Directive_CSEG::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .CSEG takes no operands");
		return false;
	}
	context.SelectCodeSegment();
	return true;
}

bool Directive_CSEG::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return Prepare(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
bool Directive_DB::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	//*********
	context.ForwardAddress(static_cast<UInt32>(pExpr->GetOperands().size() * sizeof(UInt8)));

	return true;
}

bool Directive_DB::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	if (!context.CheckRegionReady()) return false;
	size_t bytes = 0;
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
			buffDst += static_cast<UInt8>(num);
			bytes++;
		} else if (pExprResolved->IsTypeString()) {
			const String &str = dynamic_cast<Expr_String *>(pExprResolved.get())->GetStringSTL();
			for (auto ch : str) {
				buffDst += static_cast<UInt8>(ch);
				bytes++;
			}
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
bool Directive_DSEG::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .DSEG takes no operands");
		return false;
	}
	context.SelectDataSegment();
	return true;
}

bool Directive_DSEG::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return Prepare(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
bool Directive_DW::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	context.ForwardAddress(static_cast<UInt32>(pExpr->GetOperands().size() * sizeof(UInt16)));
	return true;
}

bool Directive_DW::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	if (!context.CheckRegionReady()) return false;
	for (auto pExprData : pExpr->GetOperands()) {
		context.StartToResolve();
		AutoPtr<Expr> pExprResolved(pExprData->Resolve(context));
		if (pExprResolved.IsNull()) return false;
		if (!pExprResolved->IsTypeNumber()) {
			ErrorLog::AddError(pExpr, "elements of directive .DB must be number value");
			return false;
		}
		UInt32 num = dynamic_cast<Expr_Number *>(pExprResolved.get())->GetNumber();
		if (num > 0xffff) {
			ErrorLog::AddError(pExpr, "an element value of directive .DB exceeds 16-bit range");
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
bool Directive_ENDM::CreateExpr(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	if (!exprStack.back()->IsTypeMacroBody()) {
		pParser->AddError("no matching .MACRO directive");
		return false;
	}
	Expr::Delete(exprStack.back());
	exprStack.pop_back();	// remove the EXPR_MacroBody instance from the stack
	return Directive::CreateExpr(pParser, exprStack, pToken);
}

bool Directive_ENDM::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ENDM takes no operands");
		return false;
	}
	return true;
}

bool Directive_ENDM::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ENDP
//-----------------------------------------------------------------------------
bool Directive_ENDP::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ENDP takes no operands");
		return false;
	}
	if (!context.DoesExistLocalLookupTable()) {
		ErrorLog::AddError(pExpr, "no matching .PROC directive");
		return false;
	}
	context.PopLocalLookupTable();
	return true;
}

bool Directive_ENDP::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return Prepare(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_ENDPCG
//-----------------------------------------------------------------------------
bool Directive_ENDPCG::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ENDPCG takes no operands");
		return false;
	}
	return true;
}

bool Directive_ENDPCG::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	if (!context.CheckRegionReady()) return false;
	return true;
}

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
bool Directive_EQU::CreateExpr(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
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

bool Directive_EQU::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	// nothing to do
	return true;
}

bool Directive_EQU::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
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
bool Directive_FILENAME_JR::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_FILENAME_JR::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
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
bool Directive_INCLUDE::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	
	return true;
}

bool Directive_INCLUDE::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ISEG
//-----------------------------------------------------------------------------
bool Directive_ISEG::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ISEG takes no operands");
		return false;
	}
	context.SelectInternalSegment();
	return true;
}

bool Directive_ISEG::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return Prepare(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
bool Directive_MACRO::CreateExpr(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	Expr_LabelDef *pExprLabelDef = exprStack.back()->GetChildren().SeekLabelDefToAssoc();
	if (pExprLabelDef == nullptr) {
		pParser->AddError("directive .MACRO must be preceded by a label");
		return false;
	}
	AutoPtr<Expr_MacroEntry> pExpr(new Expr_MacroEntry(pExprLabelDef->GetLabel()));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	pExprLabelDef->SetAssigned(pExpr->Reference());	// associate it to the preceding label
	exprStack.push_back(pExpr->GetMacroBody()->Reference());	// for directives in the body
	exprStack.push_back(pExpr.release());						// for operands
	return true;
}

bool Directive_MACRO::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_MACRO::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_MML
//-----------------------------------------------------------------------------
bool Directive_MML::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	//Handler handler(nullptr);
	return true;
}

bool Directive_MML::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
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
bool Directive_ORG::Prepare(Context &context, const Expr_Directive *pExpr) const
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

bool Directive_ORG::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return Prepare(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
bool Directive_PCG::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_PCG::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	if (!context.CheckRegionReady()) return false;
	return true;
}

//-----------------------------------------------------------------------------
// Directive_PROC
//-----------------------------------------------------------------------------
bool Directive_PROC::CreateExpr(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
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

bool Directive_PROC::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	context.PushLocalLookupTable();
	return true;
}

bool Directive_PROC::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return Prepare(context, pExpr);
}
