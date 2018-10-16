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
	PCGType _pcgType;
	int _charCodeStart;
	bool _upperCaseFlag;
	PCGInfoOwner _pcgInfoOwner;
public:
	DeclareReferenceAccessor(PCGPageInfo);
public:
	PCGPageInfo(const String &symbol, PCGType pcgType, int charCodeStart, bool upperCaseFlag);
protected:
	inline ~PCGPageInfo() {};
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline PCGType GetPCGType() const { return _pcgType; }
	inline const char *GetPCGTypeName() const { return (_pcgType == PCGTYPE_User)? "USER" : "CRAM"; }
	inline int GetCharCodeStart() const { return _charCodeStart; }
	inline PCGInfoOwner &GetPCGInfoOwner() { return _pcgInfoOwner; }
	inline const PCGInfoOwner &GetPCGInfoOwner() const { return _pcgInfoOwner; }
	inline bool IsEmptyPCGInfo() const { return _pcgInfoOwner.empty(); }
	inline void NewPCGInfo() { _pcgInfoOwner.NewPCGInfo(); }
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

