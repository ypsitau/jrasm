//=============================================================================
// InlineData.h
//=============================================================================
#ifndef __JRASMCORE_INLINEDATA_H__
#define __JRASMCORE_INLINEDATA_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// InlineData
//-----------------------------------------------------------------------------
class InlineData {
private:
	int _cntRef;
	Binary _buff;
	Integer _addrOffset;
	AutoPtr<Region> _pRegion;
public:
	DeclareReferenceAccessor(InlineData);
public:
	inline InlineData(const Binary &buff) : _cntRef(1), _buff(buff), _addrOffset(0) {}
private:
	inline ~InlineData() {}
public:
	inline const Binary &GetBinary() const { return _buff; }
	inline Integer GetInteger() const {
		return _pRegion.IsNull()? _addrOffset : _addrOffset + _pRegion->GetAddrTop();
	}
	bool OnPhaseAssignSymbol(Context &context);
	bool OnPhaseGenerate(Context &context, Binary *pBuffDst) const;
	bool OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// InlineDataList
//-----------------------------------------------------------------------------
class InlineDataList : public std::vector<InlineData *> {
public:
	InlineData *Lookup(const Binary &buff);
	bool OnPhaseAssignSymbol(Context &context);
	bool OnPhaseGenerate(Context &context, Binary *pBuffDst) const;
	bool OnPhaseDisasm(Context &context, DisasmDumper &disasmDumper, int indentLevelCode) const;
};

//-----------------------------------------------------------------------------
// InlineDataOwner
//-----------------------------------------------------------------------------
class InlineDataOwner : public InlineDataList {
public:
	~InlineDataOwner();
	void Clear();
};

#endif
