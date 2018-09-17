//=============================================================================
// Directive.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
std::unique_ptr<DirectiveOwner> Directive::_pDirectivesBuiltIn;

bool Directive::IsAssocToLabel() const
{
	return false;
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
bool Directive_CSEG::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_CSEG::Generate(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
bool Directive_DB::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_DB::Generate(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_DSEG
//-----------------------------------------------------------------------------
bool Directive_DSEG::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_DSEG::Generate(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
bool Directive_DW::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_DW::Generate(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ENDM
//-----------------------------------------------------------------------------
bool Directive_ENDM::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_ENDM::Generate(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ENDP
//-----------------------------------------------------------------------------
bool Directive_ENDP::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_ENDP::Generate(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
bool Directive_EQU::IsAssocToLabel() const
{
	return true;
}

bool Directive_EQU::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_EQU::Generate(Context &context, const Expr_Directive *pExpr) const
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
bool Directive_INCLUDE::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_INCLUDE::Generate(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
bool Directive_MACRO::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_MACRO::Generate(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_MML
//-----------------------------------------------------------------------------
bool Directive_MML::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_MML::Generate(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_ORG
//-----------------------------------------------------------------------------
bool Directive_ORG::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	const ExprList &operands = pExpr->GetOperands();
	// .org data16
	if (operands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .org takes one operand");
		return false;
	}
	AutoPtr<Expr> pExprLast(operands.back()->Reduce(context));
	if (pExprLast.IsNull()) return false;
	if (!pExprLast->IsType(Expr::TYPE_Number)) {
		ErrorLog::AddError(pExpr, "directive .org expects a number value for its operand");
		return false;
	}
	UInt32 num = dynamic_cast<const Expr_Number *>(pExprLast.get())->GetNumber();
	if (num > 0xffff) {
		ErrorLog::AddError(pExpr, "address value exceeds 16-bit range");
		return false;
	}
	context.SetAddress(static_cast<UInt16>(num));
	return true;
}

bool Directive_ORG::Generate(Context &context, const Expr_Directive *pExpr) const
{
	context.SetAddress(0);
	return true;
}

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
bool Directive_PCG::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_PCG::Generate(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_PROC
//-----------------------------------------------------------------------------
bool Directive_PROC::PrepareLookupTable(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}

bool Directive_PROC::Generate(Context &context, const Expr_Directive *pExpr) const
{
	return true;
}
