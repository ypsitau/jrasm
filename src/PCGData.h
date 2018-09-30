//=============================================================================
// PCGData.h
//=============================================================================
#ifndef __PCGDATA_H__
#define __PCGDATA_H__

#include "Common.h"

//-----------------------------------------------------------------------------
// PCGType
//-----------------------------------------------------------------------------
enum PCGType {
	PCGTYPE_User,
	PCGTYPE_CRAM,
};

//-----------------------------------------------------------------------------
// PCGData
//-----------------------------------------------------------------------------
class PCGData {
private:
	int _cntRef;
	PCGType _pcgType;
	int _charCode;
	Binary _data;
public:
	DeclareReferenceAccessor(PCGData);
public:
	inline PCGData(PCGType pcgType, int charCode, const Binary &data) :
		_cntRef(1), _pcgType(pcgType), _charCode(charCode), _data(data) {}
private:
	inline ~PCGData() {}
public:
	inline PCGType GetPCGType() const { return _pcgType; }
	inline int GetCharCode() const { return _charCode; }
	inline bool HasSameData(const Binary &data) { return _data == data; }
	inline const Binary &GetData() const { return _data; }
};

//-----------------------------------------------------------------------------
// PCGDataList
//-----------------------------------------------------------------------------
class PCGDataList : public std::vector<PCGData *> {
public:
	const PCGData *FindSameData(const Binary &data) const;
};

//-----------------------------------------------------------------------------
// PCGDataOwner
//-----------------------------------------------------------------------------
class PCGDataOwner : public PCGDataList {
public:
	~PCGDataOwner();
	void Clear();
};

#endif
