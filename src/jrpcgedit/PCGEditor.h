//=============================================================================
// PCGEditor.h
//=============================================================================
#ifndef __JRPCGEDIT_PCGEDITOR_H__
#define __JRPCGEDIT_PCGEDITOR_H__

#include "PCGDataInfo.h"

//-----------------------------------------------------------------------------
// PCGEditor
//-----------------------------------------------------------------------------
class PCGEditor : public wxPanel {
public:
	class Listener {
	public:
		virtual void NotifyPCGModified() = 0;
	};
	class ListenerList : public std::vector<Listener *> {
	public:
		void NotifyPCGModified();
	};
private:
	wxRect _rcMatrix;
	AutoPtr<PCGDataInfo> _pPCGDataInfo;
	std::unique_ptr<wxBitmap> _pBmpMatrix;
	wxPen _penBorder;
	wxPen _penGrid;
	wxPen _penGridHL;
	wxBrush _brushBg;
	wxBrush _brushMatrix;
	ListenerList _listenerList;
private:
	static int _mgnLeft;
	static int _mgnRight;
	static int _mgnTop;
	static int _mgnBottom;
public:
	PCGEditor(wxWindow *pParent, PCGDataInfo *pPCGDataInfo);
	inline void AddListener(Listener *pListener) { _listenerList.push_back(pListener); }
	inline int DotPosXToMatrixCoord(int dotPosX) const { return _mgnLeft + dotPosX * GetDotSize(); }
	inline int DotPosYToMatrixCoord(int dotPosY) const { return _mgnTop + dotPosY * GetDotSize(); }
	inline int GetDotSize() const { return _pPCGDataInfo->GetDotSizeEditor(); }
	void SetDotSize(int dotSize);
	void SetPCGDataInfo(PCGDataInfo *pPCGDataInfo);
	void PrepareMatrix(bool refreshFlag);
	void UpdateMatrix(bool refreshFlag);
	void PutDot(int dotPosX, int dotPosY, bool flag);
	wxRect DotPosToCursorRect(int dotPosX, int dotPosY);
	void PointToDotPos(const wxPoint &pt, int *pDotPosX, int *pDotPosY) const;
private:
    wxDECLARE_EVENT_TABLE();
	void OnEraseBackground(wxEraseEvent &event);
	void OnSize(wxSizeEvent &event);
	void OnPaint(wxPaintEvent &event);
	void OnSetFocus(wxFocusEvent &event);
	void OnKillFocus(wxFocusEvent &event);
	void OnMotion(wxMouseEvent &event);
	void OnLeftDown(wxMouseEvent &event);
	void OnLeftUp(wxMouseEvent &event);
	void OnLeftDClick(wxMouseEvent &event);
	void OnRightDown(wxMouseEvent &event);
	void OnRightUp(wxMouseEvent &event);
	void OnLeaveWindow(wxMouseEvent &event);
	void OnKeyDown(wxKeyEvent &event);
	void OnKeyUp(wxKeyEvent &event);
};

#endif
