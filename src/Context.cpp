//=============================================================================
// Context.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Context
//-----------------------------------------------------------------------------
Context::Context() : _preparationFlag(false)
{
	_fileNameJR = "JRASM_PRODUCT";
	_lookupTableStack.push_back(new LookupTable());
	_segmentOwner.push_back(new Segment("code"));		// code segment
	_segmentOwner.push_back(new Segment("data"));		// data segment
	_segmentOwner.push_back(new Segment("internal"));	// internal segment
	SelectCodeSegment();
}

void Context::StartRegion(UInt32 addr)
{
	_pSegmentCur->AddRegion(new Region(addr));
	_pSegmentCur->SetAddress(addr);
}

Context::LookupTable *Context::AddLookupTable()
{
	LookupTable *pLookupTable = new LookupTable(_lookupTableStack.back()->Reference());
	_lookupTableStack.push_back(pLookupTable);
	return pLookupTable;
}

void Context::RemoveLookupTable()
{
	LookupTable *pLookupTable = _lookupTableStack.back();
	_lookupTableStack.pop_back();
	LookupTable::Delete(pLookupTable);
}

Context::LabelInfoOwner *Context::MakeLabelInfoOwner() const
{
	std::unique_ptr<LabelInfoOwner> pLabelInfoOwner(new LabelInfoOwner());
	for (auto iter : *GetLookupTableRoot()) {
		//pLabelInfoOwner->push_back(new LabelInfo(iter.second, iter.first.c_str()));
	}
	pLabelInfoOwner->SortByAddr();
	return pLabelInfoOwner.release();
}

//-----------------------------------------------------------------------------
// Context::LookupTable
//-----------------------------------------------------------------------------
Context::LookupTable::~LookupTable()
{
	for (auto iter : *this) {
		Expr::Delete(iter.second);
	}
}

void Context::LookupTable::Set(const String &label, Expr *pExpr)
{
	LookupTable *pLookupTable = IsGlobalLabel(label.c_str())? GetRoot() : this;
	pLookupTable->insert(std::make_pair(label, pExpr));
}

bool Context::LookupTable::IsDefined(const char *label) const
{
	const LookupTable *pLookupTable = IsGlobalLabel(label)? GetRoot() : this;
	return pLookupTable->find(label) != end();
}

const Expr *Context::LookupTable::Lookup(const char *label) const
{
	const_iterator iter = find(label);
	if (iter != end())	return iter->second;
	return _pLookupTableParent.IsNull()? nullptr : _pLookupTableParent->Lookup(label);
}

Context::LookupTable *Context::LookupTable::GetRoot()
{
	LookupTable *pLookupTable = this;
	for ( ; pLookupTable->GetParent() != nullptr; pLookupTable = pLookupTable->GetParent()) ;
	return pLookupTable;
}

//-----------------------------------------------------------------------------
// Context::LookupTableOwner
//-----------------------------------------------------------------------------
Context::LookupTableOwner::~LookupTableOwner()
{
	Clear();
}

void Context::LookupTableOwner::Clear()
{
	for (auto pLookupTable : *this) {
		LookupTable::Delete(pLookupTable);
	}
	clear();
}

//-----------------------------------------------------------------------------
// Context::LabelInfoList
//-----------------------------------------------------------------------------
size_t Context::LabelInfoList::GetLabelLenMax(const_iterator ppLabelInfo, const_iterator ppLabelInfoEnd)
{
	size_t labelLenMax = 0;
	for ( ; ppLabelInfo != ppLabelInfoEnd; ppLabelInfo++) {
		const LabelInfo *pLabelInfo = *ppLabelInfo;
		size_t labelLen = ::strlen(pLabelInfo->GetLabel());
		if (labelLenMax < labelLen) labelLenMax = labelLen;
	}
	return labelLenMax;
}

//-----------------------------------------------------------------------------
// Context::LabelInfoOwner
//-----------------------------------------------------------------------------
Context::LabelInfoOwner::~LabelInfoOwner()
{
	Clear();
}

void Context::LabelInfoOwner::Clear()
{
	for (auto pLabelInfo : *this) {
		delete pLabelInfo;
	}
	clear();
}
