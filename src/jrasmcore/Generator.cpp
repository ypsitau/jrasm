//=============================================================================
// Generator.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Generator
//-----------------------------------------------------------------------------
std::unique_ptr<Generator> Generator::_pGenerator;

Generator::~Generator()
{
}

void Generator::Initialize(Generator *pGenerator)
{
	_pGenerator.reset(pGenerator);
}

bool Generator::ForwardAddress(Context &context, const Expr_Instruction *pExpr) const
{
	return DoForwardAddress(context, pExpr);
}

bool Generator::Generate(Context &context, const Expr_Instruction *pExpr, Binary *pBuffDst) const
{
	if (pBuffDst == nullptr) pBuffDst = &context.GetSegmentBuffer();
	return DoGenerate(context, pExpr, *pBuffDst);
}

Expr *Generator::ComposeExpr_Save(
	Context &context, Expr *pExpr, Directive::SaveInfo &saveInfo) const
{
	return DoComposeExpr_Save(context, pExpr, saveInfo);
}

Expr *Generator::ComposeExpr_Restore(
	Context &context, Expr *pExpr, Directive::SaveInfo &saveInfo, const StringList &regNames) const
{
	return DoComposeExpr_Restore(context, pExpr, saveInfo, regNames);
}

void Generator::DumpDisasmHelper(
	Integer addr, const Binary &buff, const char *strCode,
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
			addr += static_cast<Integer>(iCol);
			iCol = 0;
			iLine++;
		}
	}
	if (iCol > 0) {
		::fprintf(fp, formatHead, addr, JustifyLeft(str.c_str(), 3 * nColsPerLineMax).c_str(),
				  (iLine == 0)? strCode : "");
	}
}
