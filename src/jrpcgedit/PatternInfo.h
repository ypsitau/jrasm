//=============================================================================
// PatternInfo.h
//=============================================================================
#ifndef __JRPCGEDIT_PATTERNINFO_H__
#define __JRPCGEDIT_PATTERNINFO_H__

//-----------------------------------------------------------------------------
// PatternInfo
//-----------------------------------------------------------------------------
class PatternInfo {
private:
	int _cntRef;
	int _nDotsX;
	int _nDotsY;
	int _sizeDot;
	std::unique_ptr<bool[]> _dotTbl;
	std::unique_ptr<wxBitmap> _pBitmap;
public:
	DeclareReferenceAccessor(PatternInfo);
public:
	PatternInfo(int nDotsX, int nDotsY);
protected:
	inline ~PatternInfo() {};
public:
	inline int GetNDotsX() const { return _nDotsX; }
	inline int GetNDotsY() const { return _nDotsY; }
	inline void ClearAll() { ::memset(_dotTbl.get(), 0x00, _nDotsX * _nDotsY); }
	inline void PutDot(int iDotX, int iDotY, bool data) {
		_dotTbl[iDotX + iDotY * _nDotsX] = data;
	}
	inline bool GetDot(int iDotX, int iDotY) const {
		return _dotTbl[iDotX + iDotY * _nDotsX];
	}
	wxBitmap &MakeBitmap(int sizeDot);
};

//-----------------------------------------------------------------------------
// PatternInfoList
//-----------------------------------------------------------------------------
class PatternInfoList : public std::vector<PatternInfo *> {
};

//-----------------------------------------------------------------------------
// PatternInfoOwner
//-----------------------------------------------------------------------------
class PatternInfoOwner : public PatternInfoList {
public:
	~PatternInfoOwner();
	void Clear();
};

#endif

