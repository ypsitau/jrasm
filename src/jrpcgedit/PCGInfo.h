//=============================================================================
// PCGInfo.h
//=============================================================================
#ifndef __JRPCGEDIT_PCGINFO_H__
#define __JRPCGEDIT_PCGINFO_H__

//-----------------------------------------------------------------------------
// PCGInfo
//-----------------------------------------------------------------------------
class PCGInfo {
public:
	class Pattern {
	private:
		int _cntRef;
		int _dotNumX;
		int _dotNumY;
		std::unique_ptr<bool[]> _dotTbl;
	public:
		DeclareReferenceAccessor(Pattern);
	public:
		Pattern(int dotNumX, int dotNumY);
	private:
		inline ~Pattern() {}
	public:
		inline int GetDotNumX() const { return _dotNumX; }
		inline int GetDotNumY() const { return _dotNumY; }
		inline void Clear() { ::memset(_dotTbl.get(), 0x00, _dotNumX * _dotNumY); }
		inline bool IsWithin(int dotPosX, int dotPosY) const {
			return 0 <= dotPosX && dotPosX < _dotNumX && 0 <= dotPosY && dotPosY < _dotNumY;
		}
		inline void PutDot(int dotPosX, int dotPosY, bool flag) {
			if (IsWithin(dotPosX, dotPosY)) _dotTbl[dotPosX + dotPosY * _dotNumX] = flag;
		}
		inline bool GetDot(int dotPosX, int dotPosY) const {
			return IsWithin(dotPosX, dotPosY)? _dotTbl[dotPosX + dotPosY * _dotNumX] : false;
		}
		void Paste(int dotPosX, int dotPosY, const Pattern *pPatternSrc);
	};
private:
	int _cntRef;
	String _symbol;
	int _sizeDot;
	int _dotPosX;
	int _dotPosY;
	bool _selectedFlag;
	wxRect _rcItem;
	AutoPtr<Pattern> _pPattern;
	AutoPtr<Pattern> _pPatternOrg;
	std::unique_ptr<wxBitmap> _pBitmap;
public:
	DeclareReferenceAccessor(PCGInfo);
public:
	PCGInfo(const String &symbol, int dotNumX, int dotNumY);
protected:
	inline ~PCGInfo() {};
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline int GetDotNumX() const { return _pPattern->GetDotNumX(); }
	inline int GetDotNumY() const { return _pPattern->GetDotNumY(); }
	inline int GetDotPosXMax() const { return _pPattern->GetDotNumX() - 1; }
	inline int GetDotPosYMax() const { return _pPattern->GetDotNumY() - 1; }
	inline int GetDotPosX() const { return _dotPosX; }
	inline int GetDotPosY() const { return _dotPosY; }
	inline void SetDotPosX(int dotPosX) { _dotPosX = dotPosX; }
	inline void SetDotPosY(int dotPosY) { _dotPosY = dotPosY; }
	inline void SetDotPos(int dotPosX, int dotPosY) { _dotPosX = dotPosX, _dotPosY = dotPosY; }
	inline void ClearPattern() { _pPattern->Clear(); }
	inline bool IsWithin(int dotPosX, int dotPosY) const { return _pPattern->IsWithin(dotPosX, dotPosY); }
	inline void PutDot(int dotPosX, int dotPosY, bool flag) {
		_pPattern->PutDot(dotPosX, dotPosY, flag);
	}
	inline bool GetDot(int dotPosX, int dotPosY) const {
		return _pPattern->GetDot(dotPosX, dotPosY);
	}
	inline void SetSelectedFlag(bool selectedFlag) { _selectedFlag = selectedFlag; }
	inline bool GetSelectedFlag() { return _selectedFlag; }
	inline const wxRect &GetRectItem() const { return _rcItem; }
	inline void SetRectItem(int x, int y, int width, int height) {
		_rcItem = wxRect(x, y, width, height);
	}
	void ChangeDotNum(int dotNumX, int dotNumY);
	wxBitmap &MakeBitmap(int sizeDot);
};

//-----------------------------------------------------------------------------
// PCGInfoList
//-----------------------------------------------------------------------------
class PCGInfoList : public std::vector<PCGInfo *> {
public:
	const PCGInfo *FindSelected() const;
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

