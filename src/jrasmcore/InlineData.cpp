//=============================================================================
// InlineData.h
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// InlineData
//-----------------------------------------------------------------------------
bool InlineData::OnPhaseAssignSymbol(Context &context)
{
	Region *pRegion = context.GetInlineDataSegment()->GetRegionCur();
	_addrOffset = pRegion->GetAddrOffset();
	_pRegion.reset(pRegion->Reference());
	pRegion->ForwardAddrOffset(static_cast<Integer>(_buff.size()));
	return true;
}

bool InlineData::OnPhaseGenerate(Context &context, Binary *pBuffDst) const
{
	Region *pRegion = context.GetInlineDataSegment()->GetRegionCur();
	if (pBuffDst == nullptr) pBuffDst = &pRegion->GetBuffer();
	*pBuffDst += _buff;
	pRegion->ForwardAddrOffset(static_cast<Integer>(_buff.size()));
	return true;
}

bool InlineData::OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const
{
	Region *pRegion = context.GetInlineDataSegment()->GetRegionCur();
	disasmDumper.DumpDataAndCode(pRegion->GetAddress(), _buff, _strSrc.c_str(), indentLevelCode);
	pRegion->ForwardAddrOffset(static_cast<Integer>(_buff.size()));
	return true;
}

//-----------------------------------------------------------------------------
// InlineDataList
//-----------------------------------------------------------------------------
InlineData *InlineDataList::Lookup(InlineData::Type type, const Binary &buff)
{
	for (auto pInlineData : *this) {
		if (pInlineData->GetType() == type && pInlineData->GetBinary() == buff) return pInlineData;
	}
	return nullptr;
}

bool InlineDataList::OnPhaseAssignSymbol(Context &context)
{
	for (auto pInlineData : *this) {
		if (!pInlineData->OnPhaseAssignSymbol(context)) return false;
	}
	return true;
}

bool InlineDataList::OnPhaseGenerate(Context &context, Binary *pBuffDst) const
{
	for (auto pInlineData : *this) {
		if (!pInlineData->OnPhaseGenerate(context, pBuffDst)) return false;
	}
	return true;
}

bool InlineDataList::OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const
{
	if (empty()) return true;
	disasmDumper.DumpCode(";; Inline Data", indentLevelCode);
	for (auto pInlineData : *this) {
		if (!pInlineData->OnPhaseDisasm(context, disasmDumper, indentLevelCode)) return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// InlineDataOwner
//-----------------------------------------------------------------------------
InlineDataOwner::~InlineDataOwner()
{
	Clear();
}

void InlineDataOwner::Clear()
{
	for (auto pInlineData : *this) {
		InlineData::Delete(pInlineData);
	}
	clear();
}
