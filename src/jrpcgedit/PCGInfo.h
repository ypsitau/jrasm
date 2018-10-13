//=============================================================================
// PCGInfo.h
//=============================================================================
#ifndef __JRPCGEDIT_PCGINFO_H__
#define __JRPCGEDIT_PCGINFO_H__

//-----------------------------------------------------------------------------
// PCGInfo
//-----------------------------------------------------------------------------
class PCGInfo {
private:
	int _cntRef;
	String _symbol;
	int _nDotsX;
	int _nDotsY;
	int _sizeDot;
	std::unique_ptr<bool[]> _dotTbl;
	std::unique_ptr<wxBitmap> _pBitmap;
public:
	DeclareReferenceAccessor(PCGInfo);
public:
	PCGInfo(const String &symbol, int nDotsX, int nDotsY);
protected:
	inline ~PCGInfo() {};
public:
	inline int GetNDotsX() const { return _nDotsX; }
	inline int GetNDotsY() const { return _nDotsY; }
	inline int GetDotPosXMax() const { return _nDotsX - 1; }
	inline int GetDotPosYMax() const { return _nDotsY - 1; }
	inline void ClearAll() { ::memset(_dotTbl.get(), 0x00, _nDotsX * _nDotsY); }
	inline bool IsWithin(int dotPosX, int dotPosY) const {
		return 0 <= dotPosX && dotPosX < _nDotsX && 0 <= dotPosY && dotPosY < _nDotsY;
	}
	inline void PutDot(int dotPosX, int dotPosY, bool data) {
		if (IsWithin(dotPosX, dotPosY)) _dotTbl[dotPosX + dotPosY * _nDotsX] = data;
	}
	inline bool GetDot(int dotPosX, int dotPosY) const {
		return IsWithin(dotPosX, dotPosY)? _dotTbl[dotPosX + dotPosY * _nDotsX] : false;
	}
	wxBitmap &MakeBitmap(int sizeDot);
};

//-----------------------------------------------------------------------------
// PCGInfoList
//-----------------------------------------------------------------------------
class PCGInfoList : public std::vector<PCGInfo *> {
};

//-----------------------------------------------------------------------------
// PCGInfoOwner
//-----------------------------------------------------------------------------
class PCGInfoOwner : public PCGInfoList {
public:
	~PCGInfoOwner();
	void Clear();
};

#endif

