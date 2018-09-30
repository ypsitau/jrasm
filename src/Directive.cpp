//=============================================================================
// Directive.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// DirectiveFactory
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DirectiveFactoryDict
//-----------------------------------------------------------------------------
void DirectiveFactoryDict::Assign(const DirectiveFactory *pDirectiveFactory)
{
	insert(std::make_pair(pDirectiveFactory->GetSymbol(), pDirectiveFactory));
}

const DirectiveFactory *DirectiveFactoryDict::Lookup(const char *symbol) const
{
	const_iterator iter = find(symbol);
	return (iter == end())? nullptr : iter->second;
}

//-----------------------------------------------------------------------------
// Directive
//-----------------------------------------------------------------------------
DirectiveDict Directive::_directiveDict;
DirectiveFactoryDict Directive::_directiveFactoryDict;

const DirectiveFactory *Directive::CSEG			= nullptr;
const DirectiveFactory *Directive::DB			= nullptr;
const DirectiveFactory *Directive::DSEG			= nullptr;
const DirectiveFactory *Directive::DW			= nullptr;
const DirectiveFactory *Directive::END			= nullptr;
const DirectiveFactory *Directive::EQU			= nullptr;
const DirectiveFactory *Directive::FILENAME_JR	= nullptr;
const DirectiveFactory *Directive::INCLUDE		= nullptr;
const DirectiveFactory *Directive::ISEG			= nullptr;
const DirectiveFactory *Directive::MACRO		= nullptr;
const DirectiveFactory *Directive::MML			= nullptr;
const DirectiveFactory *Directive::ORG			= nullptr;
const DirectiveFactory *Directive::PCGDATA		= nullptr;
const DirectiveFactory *Directive::PCGPAGE		= nullptr;
const DirectiveFactory *Directive::SCOPE		= nullptr;

Directive::~Directive()
{
}

void Directive::Initialize()
{
	_directiveFactoryDict.Assign(CSEG			= new Directive_CSEG::Factory());
	_directiveFactoryDict.Assign(DB				= new Directive_DB::Factory());
	_directiveFactoryDict.Assign(DSEG			= new Directive_DSEG::Factory());
	_directiveFactoryDict.Assign(DW				= new Directive_DW::Factory());
	_directiveFactoryDict.Assign(END			= new Directive_END::Factory());
	_directiveFactoryDict.Assign(EQU			= new Directive_EQU::Factory());
	_directiveFactoryDict.Assign(FILENAME_JR	= new Directive_FILENAME_JR::Factory());
	_directiveFactoryDict.Assign(INCLUDE		= new Directive_INCLUDE::Factory());
	_directiveFactoryDict.Assign(ISEG			= new Directive_ISEG::Factory());
	_directiveFactoryDict.Assign(MACRO			= new Directive_MACRO::Factory());
	_directiveFactoryDict.Assign(MML			= new Directive_MML::Factory());
	_directiveFactoryDict.Assign(ORG			= new Directive_ORG::Factory());
	_directiveFactoryDict.Assign(PCGDATA		= new Directive_PCGDATA::Factory());
	_directiveFactoryDict.Assign(PCGPAGE		= new Directive_PCGPAGE::Factory());
	_directiveFactoryDict.Assign(SCOPE			= new Directive_SCOPE::Factory());
}

bool Directive::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
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

bool Directive::OnPhaseDeclareMacro(Context &context, Expr_Directive *pExpr) const
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseExpandMacro(Context &context, Expr_Directive *pExpr) const
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const
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
Directive *Directive_CSEG::Factory::Create() const
{
	return new Directive_CSEG();
}

bool Directive_CSEG::OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const
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
	context.SelectCodeSegment();
	return true;
}

//-----------------------------------------------------------------------------
// Directive_DB
//-----------------------------------------------------------------------------
Directive *Directive_DB::Factory::Create() const
{
	return new Directive_DB();
}

bool Directive_DB::OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const
{
	UInt32 bytes = 0;
	if (!DoDirective(context, pExpr, nullptr, &bytes)) return false;
	context.ForwardAddress(bytes);
	return true;
}

bool Directive_DB::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	if (!context.CheckSegmentRegionReady()) return false;
	if (pBuffDst == nullptr) pBuffDst = &context.GetSegmentBuffer();
	UInt32 bytes = 0;
	if (!DoDirective(context, pExpr, pBuffDst, &bytes)) return false;
	context.ForwardAddress(bytes);
	return true;
}

bool Directive_DB::DoDirective(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst, UInt32 *pBytes)
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
Directive *Directive_DSEG::Factory::Create() const
{
	return new Directive_DSEG();
}

bool Directive_DSEG::OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const
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
	context.SelectDataSegment();
	return true;
}

//-----------------------------------------------------------------------------
// Directive_DW
//-----------------------------------------------------------------------------
Directive *Directive_DW::Factory::Create() const
{
	return new Directive_DW();
}

bool Directive_DW::OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const
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
// Directive_END
//-----------------------------------------------------------------------------
Directive *Directive_END::Factory::Create() const
{
	return new Directive_END();
}

bool Directive_END::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	if (!exprStack.back()->IsTypeDirective(Directive::MACRO) &&
		!exprStack.back()->IsTypeDirective(Directive::SCOPE) &&
		!exprStack.back()->IsTypeDirective(Directive::PCGDATA)) {
		pParser->AddError("no matching directive");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetExprChildren().push_back(pExpr->Reference());
	Expr::Delete(exprStack.back());
	exprStack.pop_back();	// remove the Expr_Directive instance from the stack
	exprStack.push_back(pExpr.release());
	return true;
}

bool Directive_END::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .END needs no operands");
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
Directive *Directive_EQU::Factory::Create() const
{
	return new Directive_EQU();
}

bool Directive_EQU::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	Expr_Label *pExprLabel = exprStack.back()->GetExprChildren().SeekLabelToAssoc();
	if (pExprLabel == nullptr) {
		pParser->AddError("directive .EQU must be preceded by a symbol");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
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
Directive *Directive_FILENAME_JR::Factory::Create() const
{
	return new Directive_FILENAME_JR();
}

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
Directive *Directive_INCLUDE::Factory::Create() const
{
	return new Directive_INCLUDE();
}

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

bool Directive_INCLUDE::OnPhaseDeclareMacro(Context &context, Expr_Directive *pExpr) const
{
	return pExpr->GetExprIncluded()->OnPhaseDeclareMacro(context);
}

bool Directive_INCLUDE::OnPhaseExpandMacro(Context &context, Expr_Directive *pExpr) const
{
	return pExpr->GetExprIncluded()->OnPhaseExpandMacro(context);
}

bool Directive_INCLUDE::OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const
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
Directive *Directive_ISEG::Factory::Create() const
{
	return new Directive_ISEG();
}

bool Directive_ISEG::OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const
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
	context.SelectInternalSegment();
	return true;
}

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
Directive *Directive_MACRO::Factory::Create() const
{
	return new Directive_MACRO();
}

bool Directive_MACRO::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	Expr_Label *pExprLabel = exprStack.back()->GetExprChildren().SeekLabelToAssoc();
	if (pExprLabel == nullptr) {
		pParser->AddError("directive .MACRO must be preceded by a label");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	pExpr->SetSymbol(pExprLabel->GetSymbol(), pExprLabel->GetForceGlobalFlag());
	pExprLabel->SetAssigned(pExpr->Reference());	// associate it to the preceding symbol
	exprStack.push_back(pExpr->Reference());		// for children
	exprStack.push_back(pExpr.release());			// for operands
	return true;
}

bool Directive_MACRO::OnPhaseDeclareMacro(Context &context, Expr_Directive *pExpr) const
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
Directive *Directive_MML::Factory::Create() const
{
	return new Directive_MML();
}

bool Directive_MML::OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const
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
Directive *Directive_ORG::Factory::Create() const
{
	return new Directive_ORG();
}

bool Directive_ORG::OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const
{
	return DoDirective(context, pExpr);
}

bool Directive_ORG::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return DoDirective(context, pExpr);
}

bool Directive_ORG::DoDirective(Context &context, const Expr_Directive *pExpr)
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

//-----------------------------------------------------------------------------
// Directive_PCGDATA
//-----------------------------------------------------------------------------
Directive *Directive_PCGDATA::Factory::Create() const
{
	return new Directive_PCGDATA();
}

bool Directive_PCGDATA::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetExprChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr->Reference());		// for children
	exprStack.push_back(pExpr.release());			// for operands
	return true;
}

bool Directive_PCGDATA::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	const ExprOwner &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() < 3) {
		ErrorLog::AddError(pExpr, "too few operands for directive .PCGDATA");
		return false;
	}
	String symbol;
	size_t wdChar = 0, htChar = 0;
	const char *errMsg = "invalid operand for directive .PCGDATA";
	do {
		Expr *pExprOperand = exprOperands[0];
		if (!pExprOperand->IsTypeSymbol()) {
			ErrorLog::AddError(pExpr, errMsg);
			return false;
		}
		symbol = dynamic_cast<Expr_Symbol *>(pExprOperand)->GetSymbol();
	} while (0);
	do {
		// segmentation error when resolve is called.

		context.StartToResolve();
		AutoPtr<Expr> pExprOperand(exprOperands[1]->Resolve(context));
		if (pExprOperand.IsNull()) return false;
		if (!pExprOperand->IsTypeNumber()) {
			ErrorLog::AddError(pExpr, errMsg);
			return false;
		}
		wdChar = dynamic_cast<Expr_Number *>(pExprOperand.get())->GetNumber();
	} while (0);
	do {
		context.StartToResolve();
		AutoPtr<Expr> pExprOperand(exprOperands[2]->Resolve(context));
		if (pExprOperand.IsNull()) return false;
		if (!pExprOperand->IsTypeNumber()) {
			ErrorLog::AddError(pExpr, errMsg);
			return false;
		}
		htChar = dynamic_cast<Expr_Number *>(pExprOperand.get())->GetNumber();
	} while (0);
	Binary buffOrg;
	UInt32 bytes = 0;
	for (auto pExprChild : pExpr->GetExprChildren()) {
		if (!pExprChild->IsTypeDirective(Directive::DB) && !pExprChild->IsTypeDirective(Directive::END)) {
			ErrorLog::AddError(pExprChild, "only .DB directive can be stored in .PCGDATA");
			return false;
		}
		if (!Directive_DB::DoDirective(
				context, dynamic_cast<Expr_Directive *>(pExprChild), &buffOrg, &bytes)) return false;
	}
	size_t bytesExpected = wdChar * htChar * 8;
	if (bytesExpected != buffOrg.size()) {
		ErrorLog::AddError(pExpr, "stored data is %zu bytes, different from the expected %zu bytes",
						   buffOrg.size(), bytesExpected);
		return false;
	}
	for (size_t yChar = 0; yChar < htChar; yChar++) {
		Binary::iterator pDataColOrg = buffOrg.begin() + yChar * wdChar * 8;
		for (size_t xChar = 0; xChar < wdChar; xChar++, pDataColOrg++) {
			Binary buffDst;
			Binary::iterator pDataOrg = pDataColOrg;
			for (size_t i = 0; i < 8; i++, pDataOrg += wdChar) {
				buffDst += *pDataOrg;
			}
                
		}
	}
	return true;
}

bool Directive_PCGDATA::OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
								   DisasmDumper &disasmDumper, int indentLevelCode) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_PCGPAGE
//-----------------------------------------------------------------------------
Directive *Directive_PCGPAGE::Factory::Create() const
{
	return new Directive_PCGPAGE();
}

bool Directive_PCGPAGE::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() != 2) {
		ErrorLog::AddError(pExpr, "directive .PCGPAGE takes two operands");
		return false;
	}
	UInt32 num = 0;
	do {
		Expr *pExprOperand = exprOperands[0];
		if (!pExprOperand->IsTypeSymbol()) {
			ErrorLog::AddError(pExpr, "directive .PCGPAGE takes a symbol value as its first operand");
			return false;
		}
		const char *symbol = dynamic_cast<const Expr_Symbol *>(pExprOperand)->GetSymbol();
		if (::strcasecmp(symbol, "cram") == 0) {
			
		} else if (::strcasecmp(symbol, "user") == 0) {

		} else {
			ErrorLog::AddError(pExpr, "the first operand takes cram or user");
			return false;
		}
	} while (0);
	do {
		context.StartToResolve();
		AutoPtr<Expr> pExprOperand(exprOperands[1]->Resolve(context));
		if (pExprOperand.IsNull()) return false;
		if (!pExprOperand->IsTypeNumber()) {
			ErrorLog::AddError(pExpr, "directive .PCGPAGE takes a number value as its second operand");
			return false;
		}
		num = dynamic_cast<const Expr_Number *>(pExprOperand.get())->GetNumber();
	} while (0);
	if (num > 0xff) {
		ErrorLog::AddError(pExpr, "address value exceeds 8-bit range");
		return false;
	}
	//context.StartRegion(static_cast<UInt16>(num));
	return true;
}

//-----------------------------------------------------------------------------
// Directive_SCOPE
//-----------------------------------------------------------------------------
Directive *Directive_SCOPE::Factory::Create() const
{
	return new Directive_SCOPE();
}

bool Directive_SCOPE::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken) const
{
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetExprChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr->Reference());		// for children
	exprStack.push_back(pExpr.release());			// for operands
	return true;
}

bool Directive_SCOPE::OnPhaseAssignSymbol(Context &context, Expr_Directive *pExpr) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .SCOPE needs no operands");
		return false;
	}
	context.BeginScope();
	bool rtn = pExpr->GetExprChildren().OnPhaseAssignSymbol(context);
	context.EndScope();
	return rtn;
}

bool Directive_SCOPE::OnPhaseGenerate(Context &context, const Expr_Directive *pExpr, Binary *pBuffDst) const
{
	return pExpr->GetExprChildren().OnPhaseGenerate(context, pBuffDst);
}

bool Directive_SCOPE::OnPhaseDisasm(Context &context, const Expr_Directive *pExpr,
								   DisasmDumper &disasmDumper, int indentLevelCode) const
{
	return pExpr->GetExprChildren().OnPhaseDisasm(context, disasmDumper, indentLevelCode);
}
