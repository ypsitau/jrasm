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
};

//-----------------------------------------------------------------------------
// InlineDataList
//-----------------------------------------------------------------------------
class InlineDataList : public std::vector<InlineData *> {
public:
	InlineData *FindTheSame(const Binary &buff);
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
