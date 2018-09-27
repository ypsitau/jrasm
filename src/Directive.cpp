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
	exprStack.back()->GetChildren().push_back(pExpr->Reference());
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

bool Directive::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
{
	// nothing to do
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
bool Directive_CSEG::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .CSEG needs no operands");
		return false;
	}
	context.SelectCodeSegment();
	return true;
}

bool Directive_CSEG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return OnPhaseSetupExprDict(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
bool Directive_DB::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
{
	return DoGenerate(context, pExpr, nullptr);
}

bool Directive_DB::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	if (!context.CheckRegionReady()) return false;
	if (pBuffDst == nullptr) pBuffDst = &context.GetBuffer();
	return DoGenerate(context, pExpr, pBuffDst);
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
bool Directive_DSEG::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .DSEG needs no operands");
		return false;
	}
	context.SelectDataSegment();
	return true;
}

bool Directive_DSEG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return OnPhaseSetupExprDict(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
bool Directive_DW::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
{
	context.ForwardAddress(static_cast<UInt32>(pExpr->GetOperands().size() * sizeof(UInt16)));
	return true;
}

bool Directive_DW::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	if (!context.CheckRegionReady()) return false;
	if (pBuffDst == nullptr) pBuffDst = &context.GetBuffer();
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
		(*pBuffDst) += static_cast<UInt8>(num >> 8);
		(*pBuffDst) += static_cast<UInt8>(num);
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

bool Directive_ENDM::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
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
bool Directive_ENDP::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ENDP needs no operands");
		return false;
	}
	if (!context.DoesExistLocalExprDict()) {
		ErrorLog::AddError(pExpr, "no matching .PROC directive");
		return false;
	}
	context.PopLocalExprDict();
	return true;
}

bool Directive_ENDP::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	// nothing to do
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ENDPCG
//-----------------------------------------------------------------------------
bool Directive_ENDPCG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
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
	Expr_SymbolDef *pExprSymbolDef = exprStack.back()->GetChildren().SeekSymbolDefToAssoc();
	if (pExprSymbolDef == nullptr) {
		pParser->AddError("directive .EQU must be preceded by a symbol");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	pExprSymbolDef->SetAssigned(pExpr->Reference());	// associate it to the preceding symbol
	exprStack.push_back(pExpr.release());
	return true;
}

bool Directive_EQU::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
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
bool Directive_FILENAME_JR::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
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
bool Directive_INCLUDE::OnPhaseInclude(Context &context, Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (operands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .INCLUDE takes one operand");
		return false;
	}
	const Expr *pExprLast = operands.back();
	if (!pExprLast->IsTypeString()) {
		ErrorLog::AddError(pExpr, "directive .INCLUDE takes a string value as its operand");
		return false;
	}
	const char *fileNameIncluded = dynamic_cast<const Expr_String *>(pExprLast)->GetString();
	String pathNameIncluded = JoinPathName(context.GetDirNameSrc(), fileNameIncluded);
	Parser parser(pathNameIncluded);
	if (!parser.ParseFile()) return false;
	pExpr->SetExprIncluded(parser.GetRoot()->Reference());
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

bool Directive_INCLUDE::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
{
	return pExpr->GetExprIncluded()->OnPhaseSetupExprDict(context);
}

bool Directive_INCLUDE::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return pExpr->GetExprIncluded()->OnPhaseGenerate(context, pBuffDst);
}

//-----------------------------------------------------------------------------
// Directive_ISEG
//-----------------------------------------------------------------------------
bool Directive_ISEG::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .ISEG needs no operands");
		return false;
	}
	context.SelectInternalSegment();
	return true;
}

bool Directive_ISEG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return OnPhaseSetupExprDict(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
bool Directive_MACRO::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	Expr_SymbolDef *pExprSymbolDef = exprStack.back()->GetChildren().SeekSymbolDefToAssoc();
	if (pExprSymbolDef == nullptr) {
		pParser->AddError("directive .MACRO must be preceded by a symbol");
		return false;
	}
	AutoPtr<Expr_MacroDecl> pExpr(new Expr_MacroDecl(pExprSymbolDef));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	pExprSymbolDef->SetAssigned(pExpr->Reference());				// associate it to the preceding symbol
	exprStack.push_back(pExpr->GetMacroBody()->Reference());	// for directives in the body
	exprStack.push_back(pExpr.release());						// for operands
	return true;
}

bool Directive_MACRO::OnPhaseDeclareMacro(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_MACRO::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_MML
//-----------------------------------------------------------------------------
bool Directive_MML::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
{
	//Handler handler(nullptr);
	return true;
}

bool Directive_MML::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	if (!context.CheckRegionReady()) return false;
	if (pBuffDst == nullptr) pBuffDst = &context.GetBuffer();
	Handler handler(pBuffDst);
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
bool Directive_ORG::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
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

bool Directive_ORG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return OnPhaseSetupExprDict(context, pExpr);
}

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
bool Directive_PCG::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_PCG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
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
	Expr_SymbolDef *pExprSymbolDef = exprStack.back()->GetChildren().SeekSymbolDefToAssoc();
	if (pExprSymbolDef != nullptr) {

	}
#endif
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(this));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr.release());
	return true;
}

bool Directive_PROC::OnPhaseSetupExprDict(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	if (!operands.empty()) {
		ErrorLog::AddError(pExpr, "directive .PROC needs no operands");
		return false;
	}
	context.PushLocalExprDict();
	return true;
}

bool Directive_PROC::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	// nothing to do
	return true;
}
