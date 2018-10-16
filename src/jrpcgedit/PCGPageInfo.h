//=============================================================================
// PCGPageInfo.h
//=============================================================================
#ifndef __JRPCGEDIT_PCGPAGEINFO_H__
#define __JRPCGEDIT_PCGPAGEINFO_H__

#include "PCGInfo.h"

//-----------------------------------------------------------------------------
// PCGPageInfo
//-----------------------------------------------------------------------------
class PCGPageInfo {
private:
	int _cntRef;
	String _symbol;
	bool _upperCaseFlag;
	std::unique_ptr<PCGRangeOwner> _pPCGRangeOwner;
	PCGInfoOwner _pcgInfoOwner;
public:
	DeclareReferenceAccessor(PCGPageInfo);
public:
	PCGPageInfo(const String &symbol, PCGRangeOwner *pPCGRangeOwner, bool upperCaseFlag);
protected:
	inline ~PCGPageInfo() {};
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline PCGType GetPCGType() const { return _pPCGRangeOwner->front()->GetPCGType(); }
	inline const char *GetPCGTypeName() const { return (GetPCGType() == PCGTYPE_CRAM)? "CRAM" : "USER"; }
	inline int GetCharCodeStart() const { return _pPCGRangeOwner->front()->GetCharCodeStart(); }
	inline PCGInfoOwner &GetPCGInfoOwner() { return _pcgInfoOwner; }
	inline const PCGInfoOwner &GetPCGInfoOwner() const { return _pcgInfoOwner; }
	inline bool IsEmptyPCGInfo() const { return _pcgInfoOwner.empty(); }
	inline void NewPCGInfo(bool selectedFlag) { _pcgInfoOwner.NewPCGInfo(selectedFlag); }
	void AddPCGInfo(PCGInfo *pPCGInfo);
	bool WriteFile(FILE *fp);
	static PCGPageInfo *CreateFromExpr(Context &context, const Expr *pExpr);
};

//-----------------------------------------------------------------------------
// PCGPageInfoList
//-----------------------------------------------------------------------------
class PCGPageInfoList : public std::vector<PCGPageInfo *> {
};

//-----------------------------------------------------------------------------
// PCGPageInfoOwner
//-----------------------------------------------------------------------------
class PCGPageInfoOwner : public PCGPageInfoList {
public:
	~PCGPageInfoOwner();
	void Clear();
	void NewPCGPageInfo();
};

#endif

