//=============================================================================
// PageInfo.h
//=============================================================================
#ifndef __JRPCGEDIT_PAGEINFO_H__
#define __JRPCGEDIT_PAGEINFO_H__

#include "PCGInfo.h"

//-----------------------------------------------------------------------------
// PageInfo
//-----------------------------------------------------------------------------
class PageInfo {
private:
	int _cntRef;
	String _symbol;
	PCGType _pcgType;
	int _charCodeStart;
	PCGInfoOwner _pcgInfoOwner;
public:
	DeclareReferenceAccessor(PageInfo);
public:
	PageInfo(const String &symbol, PCGType pcgType, int charCodeStart);
protected:
	inline ~PageInfo() {};
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline PCGType GetPCGType() const { return _pcgType; }
	inline int GetCharCodeStart() const { return _charCodeStart; }
	void NewPCGInfo();
	PCGInfoOwner &GetPCGInfoOwner() { return _pcgInfoOwner; }
	const PCGInfoOwner &GetPCGInfoOwner() const { return _pcgInfoOwner; }
};

//-----------------------------------------------------------------------------
// PageInfoList
//-----------------------------------------------------------------------------
class PageInfoList : public std::vector<PageInfo *> {
};

//-----------------------------------------------------------------------------
// PageInfoOwner
//-----------------------------------------------------------------------------
class PageInfoOwner : public PageInfoList {
public:
	~PageInfoOwner();
	void Clear();
};

#endif

