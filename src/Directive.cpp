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

bool Directive::HandleToken(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
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
	_pDirectivesBuiltIn->push_back(new Directive_EQU());
	_pDirectivesBuiltIn->push_back(new Directive_INCLUDE());
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

Expr *Directive::Reduce(Context &context, const Expr_Directive *pExpr) const
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
	return true;
}

bool Directive_CSEG::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
bool Directive_DB::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	context.ForwardAddress(static_cast<UInt32>(pExpr->GetOperands().size() * sizeof(UInt8)));
	return true;
}

bool Directive_DB::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	for (auto pExprData : pExpr->GetOperands()) {
		AutoPtr<Expr> pExprReduced(pExprData->Reduce(context));
		if (!pExprReduced->IsTypeNumber()) {
			ErrorLog::AddError(pExpr, "elements of directive .db must be number value");
			return false;
		}
		UInt32 num = dynamic_cast<Expr_Number *>(pExprReduced.get())->GetNumber();
		if (num > 0xff) {
			ErrorLog::AddError(pExpr, "an element value of directive .db exceeds 8-bit range");
			return false;
		}
		buffDst += static_cast<UInt8>(num);
	}
	size_t bytes = pExpr->GetOperands().size() * sizeof(UInt8);
	context.ForwardAddress(static_cast<UInt32>(bytes));
	return true;
}

//-----------------------------------------------------------------------------
// Directive_DSEG
//-----------------------------------------------------------------------------
bool Directive_DSEG::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_DSEG::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return true;
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
	for (auto pExprData : pExpr->GetOperands()) {
		AutoPtr<Expr> pExprReduced(pExprData->Reduce(context));
		if (!pExprReduced->IsTypeNumber()) {
			ErrorLog::AddError(pExpr, "elements of directive .db must be number value");
			return false;
		}
		UInt32 num = dynamic_cast<Expr_Number *>(pExprReduced.get())->GetNumber();
		if (num > 0xffff) {
			ErrorLog::AddError(pExpr, "an element value of directive .db exceeds 16-bit range");
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
bool Directive_ENDM::HandleToken(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	if (exprStack.back()->IsTypeMacroBody()) {
		pParser->AddError("can't find corresponding directive .macro");
		return false;
	}
	Expr::Delete(exprStack.back());
	exprStack.pop_back();	// remove the EXPR_MacroBody instance from the stack
	return Directive::HandleToken(pParser, exprStack, pToken);
}

bool Directive_ENDM::Prepare(Context &context, const Expr_Directive *pExpr) const
{
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
	return true;
}

bool Directive_ENDP::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
bool Directive_EQU::HandleToken(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	Expr_LabelDef *pExprLabelDef = exprStack.back()->GetChildren().SeekLabelDefToAssoc();
	if (pExprLabelDef == nullptr) {
		pParser->AddError("directive .equ must be preceded by a label");
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
	return true;
}

bool Directive_EQU::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return true;
}

Expr *Directive_EQU::Reduce(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	// .equ data
	if (operands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .equ takes one operand");
		return nullptr;
	}
	return operands.back()->Reduce(context);
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
// Directive_MACRO
//-----------------------------------------------------------------------------
bool Directive_MACRO::HandleToken(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	Expr_LabelDef *pExprLabelDef = exprStack.back()->GetChildren().SeekLabelDefToAssoc();
	if (pExprLabelDef == nullptr) {
		pParser->AddError("directive .macro must be preceded by a label");
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
	Handler handler(nullptr);
	return true;
}

bool Directive_MML::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	Handler handler(&buffDst);
	MmlParser parser(handler);
	parser.Reset();
	for (auto pExprData : pExpr->GetOperands()) {
		AutoPtr<Expr> pExprReduced(pExprData->Reduce(context));
		if (!pExprReduced->IsTypeString()) {
			ErrorLog::AddError(pExpr, "elements of directive .mml must be string value");
			return false;
		}
		const char *str = dynamic_cast<Expr_String *>(pExprReduced.get())->GetString();
		for (const char *p = str; ; p++) {
			if (!parser.FeedChar(*p)) {
				ErrorLog::AddError(pExpr, "invalid MML format");
				return false;
			}
			if (*p == '\0') break;
		}
	}
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
	_bytes += 2;
}

void Directive_MML::Handler::MmlRest(MmlParser &parser, int length)
{
	UInt8 lengthDev = static_cast<UInt8>(0x60 * length / 256);
	UInt8 noteDev = 0x00;
	if (_pBuffDst != nullptr) {
		*_pBuffDst += lengthDev;
		*_pBuffDst += noteDev;
	}
	_bytes += 2;
}

//-----------------------------------------------------------------------------
// Directive_ORG
//-----------------------------------------------------------------------------
bool Directive_ORG::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	// .org data16
	if (operands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .org takes one operand");
		return false;
	}
	AutoPtr<Expr> pExprLast(operands.back()->Reduce(context));
	if (pExprLast.IsNull()) return false;
	if (!pExprLast->IsTypeNumber()) {
		ErrorLog::AddError(pExpr, "directive .org expects a number value for its operand");
		return false;
	}
	UInt32 num = dynamic_cast<const Expr_Number *>(pExprLast.get())->GetNumber();
	if (num > 0xffff) {
		ErrorLog::AddError(pExpr, "address value exceeds 16-bit range");
		return false;
	}
	context.StartChunk(static_cast<UInt16>(num));
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
	return true;
}

//-----------------------------------------------------------------------------
// Directive_PROC
//-----------------------------------------------------------------------------
bool Directive_PROC::Prepare(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_PROC::Generate(Context &context, const Expr_Directive *pExpr, Binary &buffDst) const
{
	return true;
}
