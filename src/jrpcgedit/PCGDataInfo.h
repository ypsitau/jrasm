//=============================================================================
// PCGDataInfo.h
//=============================================================================
#ifndef __JRPCGEDIT_PCGDATAINFO_H__
#define __JRPCGEDIT_PCGDATAINFO_H__

//-----------------------------------------------------------------------------
// PCGDataInfo
//-----------------------------------------------------------------------------
class PCGDataInfo {
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
		inline bool *GetDotTbl() { return _dotTbl.get(); }
		inline bool IsWithin(int dotPosX, int dotPosY) const {
			return 0 <= dotPosX && dotPosX < _dotNumX && 0 <= dotPosY && dotPosY < _dotNumY;
		}
		inline void PutDot(int dotPosX, int dotPosY, bool flag) {
			if (IsWithin(dotPosX, dotPosY)) _dotTbl[dotPosX + dotPosY * _dotNumX] = flag;
		}
		inline bool GetDot(int dotPosX, int dotPosY) const {
			return IsWithin(dotPosX, dotPosY)? _dotTbl[dotPosX + dotPosY * _dotNumX] : false;
		}
		static Pattern *CreateFromBuff(int wdChar, int htChar, Binary &buff);
		void Paste(int dotPosX, int dotPosY, const Pattern *pPatternSrc);
	};
private:
	int _cntRef;
	String _symbol;
	PCGStride _pcgStride;
	std::unique_ptr<PCGColorOwner> _pPCGColorOwner;
	bool _upperCaseFlag;
	int _dotSizeBrowser;
	int _dotSizeEditor;
	int _dotPosX;
	int _dotPosY;
	bool _selectedFlag;
	wxRect _rcItem;
	AutoPtr<Pattern> _pPattern;
	AutoPtr<Pattern> _pPatternOrg;
	std::unique_ptr<wxBitmap> _pBitmapForBrowser;
public:
	DeclareReferenceAccessor(PCGDataInfo);
public:
	PCGDataInfo(const String &symbol, Pattern *pPattern, const PCGStride &pcgStride,
				PCGColorOwner *pPCGColorOwner, bool upperCaseFlag);
	PCGDataInfo(const String &symbol, int dotNumX, int dotNumY, const PCGStride &pcgStride,
				PCGColorOwner *pPCGColorOwner, bool upperCaseFlag);
protected:
	inline ~PCGDataInfo() {};
public:
	inline const char *GetSymbol() const { return _symbol.c_str(); }
	inline int GetStrideX() const { return _pcgStride.strideX; }
	inline int GetStrideY() const { return _pcgStride.strideY; }
	inline void SetDotSizeEditor(int dotSizeEditor) { _dotSizeEditor = dotSizeEditor; }
	inline int GetDotSizeEditor() const { return _dotSizeEditor; }
	inline int GetDotNumX() const { return _pPattern->GetDotNumX(); }
	inline int GetDotNumY() const { return _pPattern->GetDotNumY(); }
	inline int GetDotPosXMax() const { return _pPattern->GetDotNumX() - 1; }
	inline int GetDotPosYMax() const { return _pPattern->GetDotNumY() - 1; }
	inline int GetDotPosX() const { return _dotPosX; }
	inline int GetDotPosY() const { return _dotPosY; }
	inline void SetDotPosX(int dotPosX) { _dotPosX = dotPosX; }
	inline void SetDotPosY(int dotPosY) { _dotPosY = dotPosY; }
	inline void SetDotPos(int dotPosX, int dotPosY) { _dotPosX = dotPosX, _dotPosY = dotPosY; }
	inline void InvalidatePatternOrg() { _pPatternOrg.reset(nullptr); }
	inline void ClearPattern() { _pPattern->Clear(); InvalidatePatternOrg(); }
	inline bool IsWithin(int dotPosX, int dotPosY) const { return _pPattern->IsWithin(dotPosX, dotPosY); }
	inline void PutDot(int dotPosX, int dotPosY, bool flag) {
		_pPattern->PutDot(dotPosX, dotPosY, flag); InvalidatePatternOrg();
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
	wxBitmap &MakeBitmapForBrowser(int dotSizeBrowser);
	bool WriteFile(FILE *fp);
};

//-----------------------------------------------------------------------------
// PCGDataInfoList
//-----------------------------------------------------------------------------
class PCGDataInfoList : public std::vector<PCGDataInfo *> {
public:
	iterator FindSelected();
	const_iterator FindSelected() const;
	inline bool IsFirst(const_iterator ppPCGDataInfo) const {
		return ppPCGDataInfo == begin();
	}
	inline bool IsLast(const_iterator ppPCGDataInfo) const {
		return ppPCGDataInfo != end() && ppPCGDataInfo + 1 == end();
	}
	bool MoveSelectionUp();
	bool MoveSelectionDown();
};

//-----------------------------------------------------------------------------
// PCGDataInfoOwner
//-----------------------------------------------------------------------------
class PCGDataInfoOwner : public PCGDataInfoList {
public:
	~PCGDataInfoOwner();
	void Clear();
	bool DeleteSelection();
	void NewPCGDataInfo(bool selectedFlag);
};

#endif

