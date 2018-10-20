//=============================================================================
// Document.h
//=============================================================================
#ifndef __JRPCGEDIT_DOCUMENT_H__
#define __JRPCGEDIT_DOCUMENT_H__

#include "PCGPageInfo.h"

//-----------------------------------------------------------------------------
// Document
//-----------------------------------------------------------------------------
class Document {
private:
	int _cntRef;
	String _pathName;
	StringList _strSrcSegs;
	std::unique_ptr<PCGPageInfoOwner> _pPCGPageInfoOwner;
public:
	DeclareReferenceAccessor(Document);
public:
	Document();
protected:
	inline ~Document() {};
public:
	inline bool IsNoName() const { return _pathName.empty(); }
	inline const char *GetPathName() const { return _pathName.c_str(); }
	inline PCGPageInfoOwner &GetPCGPageInfoOwner() { return *_pPCGPageInfoOwner; }
	inline const PCGPageInfoOwner &GetPCGPageInfoOwner() const { return *_pPCGPageInfoOwner; }
	inline void NewPCGPageInfo() { _pPCGPageInfoOwner->NewPCGPageInfo(); }
	void InitAsBlank();
	bool ReadFile(const char *pathName);
	bool WriteFile(const char *pathName);
public:
	static String ReadLines(FILE *fp, int nLines);
	static String ReadRest(FILE *fp);
	static void SkipLines(FILE *fp, int nLines);
};

#endif
