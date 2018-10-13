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
	String _symbol;
	int _nDotsX;
	int _nDotsY;
	int _sizeDot;
	std::unique_ptr<bool[]> _dotTbl;
	std::unique_ptr<wxBitmap> _pBitmap;
public:
	DeclareReferenceAccessor(PatternInfo);
public:
	PatternInfo(const String &symbol, int nDotsX, int nDotsY);
protected:
	inline ~PatternInfo() {};
public:
	inline int GetNDotsX() const { return _nDotsX; }
	inline int GetNDotsY() const { return _nDotsY; }
	inline int GetDotXMax() const { return _nDotsX - 1; }
	inline int GetDotYMax() const { return _nDotsY - 1; }
	inline void ClearAll() { ::memset(_dotTbl.get(), 0x00, _nDotsX * _nDotsY); }
	inline bool IsWithin(int iDotX, int iDotY) const {
		return 0 <= iDotX && iDotX < _nDotsX && 0 <= iDotY && iDotY < _nDotsY;
	}
	inline void PutDot(int iDotX, int iDotY, bool data) {
		if (IsWithin(iDotX, iDotY)) _dotTbl[iDotX + iDotY * _nDotsX] = data;
	}
	inline bool GetDot(int iDotX, int iDotY) const {
		return IsWithin(iDotX, iDotY)? _dotTbl[iDotX + iDotY * _nDotsX] : false;
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

