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
DirectiveFactoryDict Directive::_directiveFactoryDict;

const DirectiveFactory *Directive::CSEG			= nullptr;
const DirectiveFactory *Directive::DB			= nullptr;
const DirectiveFactory *Directive::DSEG			= nullptr;
const DirectiveFactory *Directive::DS			= nullptr;
const DirectiveFactory *Directive::DW			= nullptr;
const DirectiveFactory *Directive::END			= nullptr;
const DirectiveFactory *Directive::EQU			= nullptr;
const DirectiveFactory *Directive::FILENAME_JR	= nullptr;
const DirectiveFactory *Directive::INCLUDE		= nullptr;
const DirectiveFactory *Directive::MACRO		= nullptr;
const DirectiveFactory *Directive::MML			= nullptr;
const DirectiveFactory *Directive::ORG			= nullptr;
const DirectiveFactory *Directive::PCG			= nullptr;
const DirectiveFactory *Directive::PCGPAGE		= nullptr;
const DirectiveFactory *Directive::SCOPE		= nullptr;
const DirectiveFactory *Directive::STRUCT		= nullptr;

Directive::~Directive()
{
}

void Directive::Initialize()
{
	_directiveFactoryDict.Assign(CSEG			= new Directive_CSEG::Factory());
	_directiveFactoryDict.Assign(DB				= new Directive_DB::Factory());
	_directiveFactoryDict.Assign(DSEG			= new Directive_DSEG::Factory());
	_directiveFactoryDict.Assign(DS				= new Directive_DS::Factory());
	_directiveFactoryDict.Assign(DW				= new Directive_DW::Factory());
	_directiveFactoryDict.Assign(END			= new Directive_END::Factory());
	_directiveFactoryDict.Assign(EQU			= new Directive_EQU::Factory());
	_directiveFactoryDict.Assign(FILENAME_JR	= new Directive_FILENAME_JR::Factory());
	_directiveFactoryDict.Assign(INCLUDE		= new Directive_INCLUDE::Factory());
	_directiveFactoryDict.Assign(MACRO			= new Directive_MACRO::Factory());
	_directiveFactoryDict.Assign(MML			= new Directive_MML::Factory());
	_directiveFactoryDict.Assign(ORG			= new Directive_ORG::Factory());
	_directiveFactoryDict.Assign(PCG			= new Directive_PCG::Factory());
	_directiveFactoryDict.Assign(PCGPAGE		= new Directive_PCGPAGE::Factory());
	_directiveFactoryDict.Assign(SCOPE			= new Directive_SCOPE::Factory());
	_directiveFactoryDict.Assign(STRUCT			= new Directive_STRUCT::Factory());
}

bool Directive::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken)
{
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetExprChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr.release());
	return true;
}

bool Directive::OnPhasePreprocess(Context &context, Expr *pExpr)
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseAssignMacro(Context &context, Expr *pExpr)
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseExpandMacro(Context &context, Expr *pExpr)
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	// nothing to do
	return true;
}

bool Directive::OnPhaseDisasm(Context &context, const Expr *pExpr,
							  DisasmDumper &disasmDumper, int indentLevelCode) const
{
	// nothing to do
	return true;
}

Expr *Directive::Resolve(Context &context, const Expr *pExpr) const
{
	return pExpr->Reference();
}

//-----------------------------------------------------------------------------
// Directive_CSEG
//-----------------------------------------------------------------------------
Directive *Directive_CSEG::Factory::Create() const
{
	return new Directive_CSEG();
}

bool Directive_CSEG::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .CSEG needs no operands");
		return false;
	}
	context.SelectCodeSegment();
	return true;
}

bool Directive_CSEG::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	context.SelectCodeSegment();
	return true;
}

bool Directive_CSEG::OnPhaseDisasm(Context &context, const Expr *pExpr,
								   DisasmDumper &disasmDumper, int indentLevelCode) const
{
	disasmDumper.DumpCode(pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
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

bool Directive_DB::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	Integer bytes = 0;
	if (!DoDirective(context, pExpr, nullptr, &bytes)) return false;
	context.ForwardAddrOffset(bytes);
	return true;
}

bool Directive_DB::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	if (pBuffDst == nullptr) pBuffDst = &context.GetSegmentBuffer();
	Integer bytes = 0;
	if (!DoDirective(context, pExpr, pBuffDst, &bytes)) return false;
	context.ForwardAddrOffset(bytes);
	return true;
}

bool Directive_DB::OnPhaseDisasm(Context &context, const Expr *pExpr,
								 DisasmDumper &disasmDumper, int indentLevelCode) const
{
	Binary buffDst;
	Integer addr = context.GetAddress();
	if (!OnPhaseGenerate(context, pExpr, &buffDst)) return false;
	disasmDumper.DumpDataAndCode(
		addr, buffDst,
		pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	return true;
}

bool Directive_DB::DoDirective(Context &context, const Expr *pExpr, Binary *pBuffDst, Integer *pBytes)
{
	Integer bytes = 0;
	for (auto pExprOperand : pExpr->GetExprOperands()) {
		context.StartToResolve();
		AutoPtr<Expr> pExprResolved(pExprOperand->Resolve(context));
		if (pExprResolved.IsNull()) return false;
		if (pExprResolved->IsTypeInteger()) {
			Integer num = dynamic_cast<Expr_Integer *>(pExprResolved.get())->GetInteger();
			if (num < -0x80 || num > 0xff) {
				ErrorLog::AddError(pExpr, "an element value of directive .DB exceeds 8bit range");
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
			bytes += static_cast<Integer>(buff.size());
		} else {
			ErrorLog::AddError(pExpr, "elements of directive .DB must be integer or string value");
			return false;
		}
	}
	*pBytes = bytes;
	return true;
}

//-----------------------------------------------------------------------------
// Directive_DS
//-----------------------------------------------------------------------------
Directive *Directive_DS::Factory::Create() const
{
	return new Directive_DS();
}

bool Directive_DS::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	Integer bytes = 0;
	if (!DoDirective(context, pExpr, nullptr, &bytes)) return false;
	context.ForwardAddrOffset(bytes);
	return true;
}

bool Directive_DS::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	if (pBuffDst == nullptr) pBuffDst = &context.GetSegmentBuffer();
	Integer bytes = 0;
	if (!DoDirective(context, pExpr, pBuffDst, &bytes)) return false;
	context.ForwardAddrOffset(bytes);
	return true;
}

bool Directive_DS::OnPhaseDisasm(Context &context, const Expr *pExpr,
								 DisasmDumper &disasmDumper, int indentLevelCode) const
{
	Integer bytes = 0;
	Integer addr = context.GetAddress();
	if (!DoDirective(context, pExpr, nullptr, &bytes)) return false;
	context.ForwardAddrOffset(bytes);
	disasmDumper.DumpAddrAndCode(
		addr, addr + bytes - 1, pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	return true;
}

bool Directive_DS::DoDirective(Context &context, const Expr *pExpr, Binary *pBuffDst, Integer *pBytes) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .DS expects one operand");
		return false;
	}
	context.StartToResolve();
	AutoPtr<Expr> pExprResolved(exprOperands.front()->Resolve(context));
	if (pExprResolved.IsNull()) return false;
	if (!pExprResolved->IsTypeInteger()) {
		ErrorLog::AddError(pExpr, "directive .DS expects integer value");
		return false;
	}
	Integer bytes = dynamic_cast<Expr_Integer *>(pExprResolved.get())->GetInteger();
	if (bytes <= 0) {
		ErrorLog::AddError(pExpr, "directive .DS expects positive integer value");
		return false;
	}
	if (pBuffDst != nullptr) {
		for (Integer i = 0; i < bytes; i++) *pBuffDst += '\0';
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

bool Directive_DSEG::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .DSEG needs no operands");
		return false;
	}
	context.SelectDataSegment();
	return true;
}

bool Directive_DSEG::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	context.SelectDataSegment();
	return true;
}

bool Directive_DSEG::OnPhaseDisasm(Context &context, const Expr *pExpr,
								   DisasmDumper &disasmDumper, int indentLevelCode) const
{
	disasmDumper.DumpCode(pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
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

bool Directive_DW::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	context.ForwardAddrOffset(static_cast<Integer>(pExpr->GetExprOperands().size() * sizeof(UInt16)));
	return true;
}

bool Directive_DW::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	if (pBuffDst == nullptr) pBuffDst = &context.GetSegmentBuffer();
	for (auto pExprOperand : pExpr->GetExprOperands()) {
		context.StartToResolve();
		AutoPtr<Expr> pExprResolved(pExprOperand->Resolve(context));
		if (pExprResolved.IsNull()) return false;
		if (!pExprResolved->IsTypeInteger()) {
			ErrorLog::AddError(pExpr, "elements of directive .DW must be integer value");
			return false;
		}
		Integer num = dynamic_cast<Expr_Integer *>(pExprResolved.get())->GetInteger();
		if (num < -0x8000 || num > 0xffff) {
			ErrorLog::AddError(pExpr, "an element value of directive .DW exceeds 16bit range");
			return false;
		}
		*pBuffDst += static_cast<UInt8>(num >> 8);
		*pBuffDst += static_cast<UInt8>(num);
	}
	size_t bytes = pExpr->GetExprOperands().size() * sizeof(UInt16);
	context.ForwardAddrOffset(static_cast<Integer>(bytes));
	return true;
}

bool Directive_DW::OnPhaseDisasm(Context &context, const Expr *pExpr,
								 DisasmDumper &disasmDumper, int indentLevelCode) const
{
	Binary buffDst;
	Integer addr = context.GetAddress();
	if (!OnPhaseGenerate(context, pExpr, &buffDst)) return false;
	disasmDumper.DumpDataAndCode(
		addr, buffDst,
		pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	return true;
}

//-----------------------------------------------------------------------------
// Directive_END
//-----------------------------------------------------------------------------
Directive *Directive_END::Factory::Create() const
{
	return new Directive_END();
}

bool Directive_END::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken)
{
	if (!exprStack.back()->IsGroupingDirective()) {
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

bool Directive_END::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (!exprOperands.empty()) {
		ErrorLog::AddError(pExpr, "directive .END needs no operands");
		return false;
	}
	return true;
}

bool Directive_END::OnPhaseDisasm(Context &context, const Expr *pExpr,
								  DisasmDumper &disasmDumper, int indentLevelCode) const
{
	disasmDumper.DumpCode(pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	return true;
}

//-----------------------------------------------------------------------------
// Directive_EQU
//-----------------------------------------------------------------------------
Directive *Directive_EQU::Factory::Create() const
{
	return new Directive_EQU();
}

bool Directive_EQU::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken)
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

Expr *Directive_EQU::Resolve(Context &context, const Expr *pExpr) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .EQU epects one operand");
		return nullptr;
	}
	return exprOperands.back()->Resolve(context);
}

bool Directive_EQU::OnPhaseDisasm(Context &context, const Expr *pExpr,
								  DisasmDumper &disasmDumper, int indentLevelCode) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_FILENAME_JR
//-----------------------------------------------------------------------------
Directive *Directive_FILENAME_JR::Factory::Create() const
{
	return new Directive_FILENAME_JR();
}

bool Directive_FILENAME_JR::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .FILENAME.JR expects one operand");
		return false;
	}
	context.StartToResolve();
	AutoPtr<Expr> pExprLast(exprOperands.back()->Resolve(context));
	if (pExprLast.IsNull()) return false;
	if (!pExprLast->IsTypeString()) {
		ErrorLog::AddError(pExpr, "directive .FIENAME.JR expects a string value as its operand");
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

bool Directive_FILENAME_JR::OnPhaseDisasm(Context &context, const Expr *pExpr,
										  DisasmDumper &disasmDumper, int indentLevelCode) const
{
	return true;
}

//-----------------------------------------------------------------------------
// Directive_INCLUDE
//-----------------------------------------------------------------------------
Directive *Directive_INCLUDE::Factory::Create() const
{
	return new Directive_INCLUDE();
}

bool Directive_INCLUDE::OnPhasePreprocess(Context &context, Expr *pExpr)
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .INCLUDE expects one operand");
		return false;
	}
	const Expr *pExprLast = exprOperands.back();
	if (!pExprLast->IsTypeString()) {
		ErrorLog::AddError(pExpr, "directive .INCLUDE expects a string value as its operand");
		return false;
	}
	String fileNameIncluded = CorrectFileSeparator(dynamic_cast<const Expr_String *>(pExprLast)->GetString());
	String dirName;
	::SplitFileName(pExpr->GetPathNameSrc(), &dirName, nullptr);
	String pathNameIncluded = JoinPathName(dirName.c_str(), fileNameIncluded.c_str());
	Parser parser(pathNameIncluded);
	if (!parser.ParseFile()) return false;
	AutoPtr<Expr> pExprRoot(parser.GetExprRoot()->Reference());
	if (!pExprRoot->OnPhasePreprocess(context)) return false;
	_pExprIncluded.reset(pExprRoot.release());
	return true;
}

bool Directive_INCLUDE::OnPhaseAssignMacro(Context &context, Expr *pExpr)
{
	return _pExprIncluded->OnPhaseAssignMacro(context);
}

bool Directive_INCLUDE::OnPhaseExpandMacro(Context &context, Expr *pExpr)
{
	return _pExprIncluded->OnPhaseExpandMacro(context);
}

bool Directive_INCLUDE::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	return _pExprIncluded->OnPhaseAssignSymbol(context);
}

bool Directive_INCLUDE::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	return _pExprIncluded->OnPhaseGenerate(context, pBuffDst);
}

bool Directive_INCLUDE::OnPhaseDisasm(Context &context, const Expr *pExpr,
									  DisasmDumper &disasmDumper, int indentLevelCode) const
{
	disasmDumper.DumpCode(pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	return _pExprIncluded->OnPhaseDisasm(context, disasmDumper, indentLevelCode + 1);
}

//-----------------------------------------------------------------------------
// Directive_MACRO
//-----------------------------------------------------------------------------
Directive *Directive_MACRO::Factory::Create() const
{
	return new Directive_MACRO();
}

bool Directive_MACRO::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken)
{
	Expr_Label *pExprLabel = exprStack.back()->GetExprChildren().SeekLabelToAssoc();
	if (pExprLabel == nullptr) {
		pParser->AddError("directive .MACRO must be preceded by a label");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	_symbol = pExprLabel->GetSymbol();
	_forceGlobalFlag = pExprLabel->GetForceGlobalFlag();
	pExprLabel->SetAssigned(pExpr->Reference());	// associate it to the preceding symbol
	exprStack.push_back(pExpr->Reference());		// for children
	exprStack.push_back(pExpr.release());			// for operands
	return true;
}

bool Directive_MACRO::OnPhaseAssignMacro(Context &context, Expr *pExpr)
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	AutoPtr<Macro> pMacro(new Macro(_symbol, pExpr->GetExprChildren().Reference()));
	Macro::ParamOwner &paramOwner = pMacro->GetParamOwner();
	paramOwner.reserve(exprOperands.size());
	for (auto pExpr : exprOperands) {
		if (pExpr->IsTypeSymbol()) {
			paramOwner.push_back(new Macro::Param(dynamic_cast<const Expr_Symbol *>(pExpr)->GetSymbol()));
		} else if (pExpr->IsTypeAssign()) {
			Expr_Assign *pExprEx = dynamic_cast<Expr_Assign *>(pExpr);
			if (!pExprEx->GetLeft()->IsTypeSymbol()) {
				ErrorLog::AddError(pExpr, "left value of the assignment must be a symbol");
				return false;
			}
			paramOwner.push_back(new Macro::Param(
									 dynamic_cast<const Expr_Symbol *>(pExprEx->GetLeft())->GetSymbol(),
									 pExprEx->GetRight()->Reference()));
		} else {
			ErrorLog::AddError(pExpr, "directive .MACRO expects a list of parameter names");
			return false;
		}
	}
	context.GetMacroDict().Assign(pMacro.release());
	return true;
}

bool Directive_MACRO::OnPhaseDisasm(Context &context, const Expr *pExpr,
									DisasmDumper &disasmDumper, int indentLevelCode) const
{
	// nothing to do
	return true;
}

//-----------------------------------------------------------------------------
// Directive_MML
//-----------------------------------------------------------------------------
Directive *Directive_MML::Factory::Create() const
{
	return new Directive_MML();
}

bool Directive_MML::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	//Handler handler(nullptr);
	return true;
}

bool Directive_MML::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
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
	context.ForwardAddrOffset(handler.GetBytesSum());
	return true;
}

bool Directive_MML::OnPhaseDisasm(Context &context, const Expr *pExpr,
								  DisasmDumper &disasmDumper, int indentLevelCode) const
{
	Binary buffDst;
	Integer addr = context.GetAddress();
	if (!OnPhaseGenerate(context, pExpr, &buffDst)) return false;
	disasmDumper.DumpDataAndCode(
		addr, buffDst,
		pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
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

bool Directive_ORG::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	return DoDirective(context, pExpr);
}

bool Directive_ORG::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	return DoDirective(context, pExpr);
}

bool Directive_ORG::OnPhaseDisasm(Context &context, const Expr *pExpr,
								  DisasmDumper &disasmDumper, int indentLevelCode) const
{
	disasmDumper.DumpCode(pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	return DoDirective(context, pExpr);
}

bool Directive_ORG::DoDirective(Context &context, const Expr *pExpr)
{
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() != 1) {
		ErrorLog::AddError(pExpr, "directive .ORG expects one operand");
		return false;
	}
	context.StartToResolve();
	AutoPtr<Expr> pExprLast(exprOperands.back()->Resolve(context));
	if (pExprLast.IsNull()) return false;
	if (!pExprLast->IsTypeInteger()) {
		ErrorLog::AddError(pExpr, "directive .ORG expects a integer value as its operand");
		return false;
	}
	Integer num = dynamic_cast<const Expr_Integer *>(pExprLast.get())->GetInteger();
	if (num < -0x8000 || num > 0xffff) {
		ErrorLog::AddError(pExpr, "address value exceeds 16bit range");
		return false;
	}
	return context.AddRegion(num);
}

//-----------------------------------------------------------------------------
// Directive_PCG
//-----------------------------------------------------------------------------
Directive *Directive_PCG::Factory::Create() const
{
	return new Directive_PCG();
}

bool Directive_PCG::ExtractParams(Context &context, const Expr *pExpr, String *pSymbol,
								  int *pWdChar, int *pHtChar, int *pStepX, int *pStepY, Binary &buff)
{
	const ExprOwner &exprOperands = pExpr->GetExprOperands();
	const char *errMsg = "directive syntax: .PCG symbol,width,height,[stepx,[stepy]]";
	if (exprOperands.size() < 3 || exprOperands.size() > 6) {
		ErrorLog::AddError(pExpr, errMsg);
		return false;
	}
	String symbol;
	int wdChar = 0, htChar = 0;
	int stepX = 1, stepY = 32;
	do {
		Expr *pExprOperand = exprOperands[0];
		if (!pExprOperand->IsTypeSymbol()) {
			ErrorLog::AddError(pExpr, "parameter symbol expects a symbol value");
			return false;
		}
		symbol = dynamic_cast<Expr_Symbol *>(pExprOperand)->GetSymbol();
	} while (0);
	do {
		Expr *pExprOperand = exprOperands[1];
		if (!pExprOperand->IsTypeInteger()) {
			ErrorLog::AddError(pExpr, "parameter width expects an integer value");
			return false;
		}
		wdChar = dynamic_cast<Expr_Integer *>(pExprOperand)->GetInteger();
	} while (0);
	do {
		Expr *pExprOperand = exprOperands[2];
		if (!pExprOperand->IsTypeInteger()) {
			ErrorLog::AddError(pExpr, "parameter height expects an integer value");
			return false;
		}
		htChar = dynamic_cast<Expr_Integer *>(pExprOperand)->GetInteger();
	} while (0);
	if (exprOperands.size() >= 4) {
		Expr *pExprOperand = exprOperands[3];
		if (!pExprOperand->IsTypeInteger()) {
			ErrorLog::AddError(pExpr, "parameter stepx expects an integer value");
			return false;
		}
		stepX = dynamic_cast<Expr_Integer *>(pExprOperand)->GetInteger();
	} while (0);
	if (exprOperands.size() >= 5) {
		Expr *pExprOperand = exprOperands[4];
		if (!pExprOperand->IsTypeInteger()) {
			ErrorLog::AddError(pExpr, "parameter stepy expects an integer value");
			return false;
		}
		stepY = dynamic_cast<Expr_Integer *>(pExprOperand)->GetInteger();
	} while (0);
	for (auto pExprChild : pExpr->GetExprChildren()) {
		if (!pExprChild->IsTypeDirective(Directive::DB) && !pExprChild->IsTypeDirective(Directive::END)) {
			ErrorLog::AddError(pExprChild, "only .DB directive can be stored in .PCG");
			return false;
		}
		Integer bytes = 0;
		if (!Directive_DB::DoDirective(
				context, dynamic_cast<Expr_Directive *>(pExprChild), &buff, &bytes)) return false;
	}
	size_t bytesExpected = wdChar * htChar * 8;
	if (bytesExpected != buff.size()) {
		ErrorLog::AddError(pExpr, "stored data is %zu bytes, different from the expected %zu bytes",
						   buff.size(), bytesExpected);
		return false;
	}
	*pSymbol = symbol;
	*pWdChar = wdChar;
	*pHtChar = htChar;
	*pStepX = stepX;
	*pStepY = stepY;
	return true;
}

bool Directive_PCG::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken)
{
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetExprChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr->Reference());		// for children
	exprStack.push_back(pExpr.release());			// for operands
	return true;
}

bool Directive_PCG::OnPhasePreprocess(Context &context, Expr *pExpr)
{
	String symbol;
	int wdChar, htChar;
	int stepX, stepY;
	Binary buffOrg;
	if (!ExtractParams(context, pExpr, &symbol, &wdChar, &htChar, &stepX, &stepY, buffOrg)) return false;
	PCGType pcgType = context.GetPCGPageCur()->GetPCGType();
	_pPCGData.reset(new PCGData(symbol, pcgType, wdChar, htChar, stepX, stepY));
	if (context.GetPCGPageCur() == nullptr) {
		ErrorLog::AddError(pExpr, ".PCGPAGE is not declared");
		return false;
	}
	int charCodeCur = context.GetPCGPageCur()->GetCharCodeCur();
	for (size_t yChar = 0; yChar < htChar; yChar++) {
		Binary::iterator pDataColOrg = buffOrg.begin() + yChar * wdChar * 8;
		for (size_t xChar = 0; xChar < wdChar; xChar++, pDataColOrg++) {
			Binary buffDst;
			Binary::iterator pDataOrg = pDataColOrg;
			for (size_t i = 0; i < 8; i++, pDataOrg += wdChar) {
				buffDst += *pDataOrg;
			}
			const PCGChar *pPCGChar = context.GetPCGCharsBuiltIn().FindSamePattern(buffDst);
			if (pPCGChar == nullptr) {
				if (pcgType == PCGTYPE_CRAM) {
					if (charCodeCur < 0 || charCodeCur > 255) {
						ErrorLog::AddError(pExpr, "CRAM character code must be between 0 and 255");
						return false;
					}
				} else { // pcgType == PCGTYPE_User
					if (charCodeCur < 0 || charCodeCur > 63) {
						ErrorLog::AddError(pExpr, "user-defined character code must be between 32 and 95");
						return false;
					}
				}
				_pPCGData->AddPCGChar(context.GetPCGPageCur()->CreatePCGChar(buffDst));
			} else {
				_pPCGData->AddPCGChar(pPCGChar->Reference());
			}
		}
	}
	return true;
}

bool Directive_PCG::OnPhaseAssignMacro(Context &context, Expr *pExpr)
{
	_pExprGenerated.reset(_pPCGData->ComposeExpr());
	if (_pExprGenerated.IsNull()) return false;
	return _pExprGenerated->OnPhaseAssignMacro(context);
}

bool Directive_PCG::OnPhaseExpandMacro(Context &context, Expr *pExpr)
{
	return true;
}

bool Directive_PCG::OnPhaseDisasm(Context &context, const Expr *pExpr,
								   DisasmDumper &disasmDumper, int indentLevelCode) const
{
	// nothing to do
	return true;
}

//-----------------------------------------------------------------------------
// Directive_PCGPAGE
//-----------------------------------------------------------------------------
Directive *Directive_PCGPAGE::Factory::Create() const
{
	return new Directive_PCGPAGE();
}

bool Directive_PCGPAGE::ExtractParams(const Expr *pExpr, String *pSymbol, PCGType *pPCGType, int *pCharCodeStart)
{
	for (auto pExprChild : pExpr->GetExprChildren()) {
		if (!pExprChild->IsTypeDirective(Directive::PCG) && !pExprChild->IsTypeDirective(Directive::END)) {
			ErrorLog::AddError(pExprChild, "only .PCG directive can be stored in .PCGPAGE");
			return false;
		}
	}
	const char *errMsg = "directive syntax: .PCGPAGE symbol,range,...";
	const ExprList &exprOperands = pExpr->GetExprOperands();
	if (exprOperands.size() != 2) {
		ErrorLog::AddError(pExpr, errMsg);
		return false;
	}
	String symbol;
	PCGType pcgType = PCGTYPE_None;
	int charCodeStart = 0;
	do {
		Expr *pExprOperand = exprOperands[0];
		if (!pExprOperand->IsTypeSymbol()) {
			ErrorLog::AddError(pExpr, errMsg);
			return false;
		}
		symbol = dynamic_cast<const Expr_Symbol *>(pExprOperand)->GetSymbol();
	} while (0);
	do {
		Expr *pExprOperand = exprOperands[1];
		if (!pExprOperand->IsTypeBinOp(Operator::Pair)) {
			ErrorLog::AddError(pExpr, errMsg);
			return false;
		}
		ExprList exprFields;
		Expr_BinOp *pExprOperandEx = dynamic_cast<Expr_BinOp *>(pExprOperand);
		if (pExprOperandEx->GetLeft()->IsTypeBinOp(Operator::Pair)) {
			Expr_BinOp *pExprLeftEx = dynamic_cast<Expr_BinOp *>(pExprOperandEx->GetLeft());
			exprFields.push_back(pExprLeftEx->GetLeft());
			exprFields.push_back(pExprLeftEx->GetRight());
			exprFields.push_back(pExprOperandEx->GetRight());
		} else {
			exprFields.push_back(pExprOperandEx->GetLeft());
			exprFields.push_back(pExprOperandEx->GetRight());
		}
		do {
			Expr *pExprField = exprFields[0];
			if (!pExprField->IsTypeSymbol()) {
				ErrorLog::AddError(pExpr, errMsg);
				return false;
			}
			const char *symbolType = dynamic_cast<const Expr_Symbol *>(pExprField)->GetSymbol();
			if (::strcasecmp(symbolType, "cram") == 0) {
				pcgType = PCGTYPE_CRAM;
			} else if (::strcasecmp(symbolType, "user") == 0) {
				pcgType = PCGTYPE_User;
			} else {
				ErrorLog::AddError(pExpr, errMsg);
				return false;
			}
		} while (0);
		do {
			Expr *pExprField = exprFields[1];
			if (!pExprField->IsTypeInteger()) {
				ErrorLog::AddError(pExpr, errMsg);
				return false;
			}
			charCodeStart = dynamic_cast<const Expr_Integer *>(pExprField)->GetInteger();
			if (pcgType == PCGTYPE_CRAM) {
				if (charCodeStart < 0 || charCodeStart > 255) {
					ErrorLog::AddError(pExpr, "CRAM character code must be between 0 and 255");
					return false;
				}
			} else { // pcgType == PCGTYPE_User
				charCodeStart -= 32;
				if (charCodeStart < 0 || charCodeStart > 63) {
					ErrorLog::AddError(pExpr, "user-defined character code must be between 32 and 95");
					return false;
				}
			}
		} while (0);
	} while (0);
	*pSymbol = symbol;
	*pPCGType = pcgType;
	*pCharCodeStart = charCodeStart;
	return true;
}

bool Directive_PCGPAGE::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken)
{
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetExprChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr->Reference());		// for children
	exprStack.push_back(pExpr.release());			// for operands
	return true;
}

bool Directive_PCGPAGE::OnPhasePreprocess(Context &context, Expr *pExpr)
{
	String symbol;
	PCGType pcgType;
	int charCodeStart;
	if (!ExtractParams(pExpr, &symbol, &pcgType, &charCodeStart)) return false;
	_pPCGPage.reset(new PCGPage(symbol, pcgType, charCodeStart));
	context.AddPCGPage(_pPCGPage->Reference());
	return true;
}

bool Directive_PCGPAGE::OnPhaseAssignMacro(Context &context, Expr *pExpr)
{
	_pExprGenerated.reset(_pPCGPage->ComposeExpr());
	if (_pExprGenerated.IsNull()) return false;
	return _pExprGenerated->OnPhaseAssignMacro(context);
}

bool Directive_PCGPAGE::OnPhaseExpandMacro(Context &context, Expr *pExpr)
{
	return _pExprGenerated->OnPhaseExpandMacro(context);
}

bool Directive_PCGPAGE::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	return _pExprGenerated->OnPhaseAssignSymbol(context);
}

bool Directive_PCGPAGE::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	if (pBuffDst == nullptr) pBuffDst = &context.GetSegmentBuffer();
	for (auto pPCGChar : _pPCGPage->GetPCGCharOwner()) {
		const Binary &buff = pPCGChar->GetBuffer();
		*pBuffDst += buff;
		context.ForwardAddrOffset(static_cast<Integer>(buff.size()));
	}
	return true;
}

bool Directive_PCGPAGE::OnPhaseDisasm(Context &context, const Expr *pExpr,
									  DisasmDumper &disasmDumper, int indentLevelCode) const
{
	bool upperCaseFlag = disasmDumper.GetUpperCaseFlag();
	const char *symbol = upperCaseFlag? ".DB" : ".db";
	const char *formatData = upperCaseFlag? "0x%02X" : "0x%02x";
	String strHead = JustifyLeft(symbol, Generator::GetInstance().GetInstNameLenMax()) + " ";
	disasmDumper.DumpCode(pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	for (auto pPCGChar : _pPCGPage->GetPCGCharOwner()) {
		const Binary &buff = pPCGChar->GetBuffer();
		String strData;
		for (auto data : buff) {
			char str[64];
			if (!strData.empty()) strData += ",";
			::sprintf_s(str, formatData, static_cast<UInt8>(data));
			strData += str;
		}
		disasmDumper.DumpDataAndCode(context.GetAddress(), buff,
									 (strHead + strData).c_str(), indentLevelCode + 1);
		context.ForwardAddrOffset(static_cast<Integer>(buff.size()));
	}
	return true;
}

//-----------------------------------------------------------------------------
// Directive_SCOPE
//-----------------------------------------------------------------------------
Directive *Directive_SCOPE::Factory::Create() const
{
	return new Directive_SCOPE();
}

bool Directive_SCOPE::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken)
{
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	exprStack.back()->GetExprChildren().push_back(pExpr->Reference());
	exprStack.push_back(pExpr->Reference());		// for children
	exprStack.push_back(pExpr.release());			// for operands
	return true;
}

bool Directive_SCOPE::OnPhasePreprocess(Context &context, Expr *pExpr)
{
	bool rtn = true;
	const ExprList &exprOperands = pExpr->GetExprOperands();
	ExprOwner &exprChildren = pExpr->GetExprChildren();
	StringList regNames;
	for (auto pExprOperand : exprOperands) {
		if (!pExprOperand->IsTypeSymbol()) {
			ErrorLog::AddError("only symbols are acceptable");
			return false;
		}
		const char *regName = dynamic_cast<Expr_Symbol *>(pExprOperand)->GetSymbol();
		if (std::find(regNames.begin(), regNames.end(), regName) != regNames.end()) {
			ErrorLog::AddError("duplicated register name");
			return false;
		}
		regNames.push_back(regName);
	}
	AutoPtr<Expr> pExpr_end(exprChildren.back());
	exprChildren.pop_back();						// remove .end directive
	rtn = Generator::GetInstance().GenCodeScope(context, pExpr, regNames);
	exprChildren.push_back(pExpr_end.release());	// restore .end directive
	return rtn;
}

bool Directive_SCOPE::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	context.BeginScope();
	bool rtn = pExpr->GetExprChildren().OnPhaseAssignSymbol(context);
	context.EndScope();
	return rtn;
}


bool Directive_SCOPE::OnPhaseGenerate(Context &context, const Expr *pExpr, Binary *pBuffDst) const
{
	return pExpr->GetExprChildren().OnPhaseGenerate(context, pBuffDst);
}

bool Directive_SCOPE::OnPhaseDisasm(Context &context, const Expr *pExpr,
									DisasmDumper &disasmDumper, int indentLevelCode) const
{
	disasmDumper.DumpCode(pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	return pExpr->GetExprChildren().OnPhaseDisasm(context, disasmDumper, indentLevelCode + 1);
}

//-----------------------------------------------------------------------------
// Directive_STRUCT
//-----------------------------------------------------------------------------
Directive *Directive_STRUCT::Factory::Create() const
{
	return new Directive_STRUCT();
}

bool Directive_STRUCT::OnPhaseParse(const Parser *pParser, ExprStack &exprStack, const Token *pToken)
{
	Expr_Label *pExprLabel = exprStack.back()->GetExprChildren().SeekLabelToAssoc();
	if (pExprLabel == nullptr) {
		pParser->AddError("directive .STRUCT must be preceded by a label");
		return false;
	}
	AutoPtr<Expr_Directive> pExpr(new Expr_Directive(Reference()));
	pParser->SetExprSourceInfo(pExpr.get(), pToken);
	_symbol = pExprLabel->GetSymbol();
	_forceGlobalFlag = pExprLabel->GetForceGlobalFlag();
	pExprLabel->SetAssigned(pExpr->Reference());	// associate it to the preceding symbol
	exprStack.push_back(pExpr->Reference());		// for children
	exprStack.push_back(pExpr.release());			// for operands
	return true;
}

bool Directive_STRUCT::OnPhaseAssignSymbol(Context &context, Expr *pExpr)
{
	pExpr->AssignExprDict(context, true);
	Integer offset = 0;
	ExprDict &exprDict = context.GetExprDictCurrent();
	for (auto pExprChild : pExpr->GetExprChildren()) {
		if (pExprChild->IsTypeLabel()) {
			Expr_Label *pExprEx = dynamic_cast<Expr_Label *>(pExprChild);
			String symbol = _symbol;
			symbol += ".";
			symbol += pExprEx->GetSymbol();
			AutoPtr<Expr> pExprAssigned(new Expr_Integer(offset));
			pExprAssigned->DeriveSourceInfo(pExprEx);
			exprDict.Assign(symbol.c_str(), pExprAssigned.release(), _forceGlobalFlag);
		} else if (pExprChild->IsTypeDirective(Directive::DS)) {
			Integer bytes = 0;
			Directive_DS *pDirective = dynamic_cast<Directive_DS *>(
				dynamic_cast<Expr_Directive *>(pExprChild)->GetDirective());
			if (!pDirective->DoDirective(context, pExprChild, nullptr, &bytes)) return false;
			offset += bytes;
		} else if (pExprChild->IsTypeDirective(Directive::END)) {
			// nothing to do
		} else {
			ErrorLog::AddError(pExprChild, "only label and .DS directive can be stored in .STRUCT");
			return false;
		}
	}
	do {
		String symbol = "@";
		symbol += _symbol;
		AutoPtr<Expr> pExprAssigned(new Expr_Integer(offset));
		pExprAssigned->DeriveSourceInfo(pExpr);
		exprDict.Assign(symbol.c_str(), pExprAssigned.release(), _forceGlobalFlag);
	} while (0);
	return true;
}

bool Directive_STRUCT::OnPhaseDisasm(Context &context, const Expr *pExpr,
									DisasmDumper &disasmDumper, int indentLevelCode) const
{
	disasmDumper.DumpCode(pExpr->ComposeSource(disasmDumper.GetUpperCaseFlag()).c_str(), indentLevelCode);
	return pExpr->GetExprChildren().OnPhaseDisasm(context, disasmDumper, indentLevelCode + 1);
}
