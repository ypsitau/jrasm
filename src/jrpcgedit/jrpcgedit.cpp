//=============================================================================
// jrpcgedit.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// AppMain
//-----------------------------------------------------------------------------
class AppMain: public wxApp {
public:
	virtual bool OnInit();
};

//-----------------------------------------------------------------------------
// AppMain
//-----------------------------------------------------------------------------
wxIMPLEMENT_APP(AppMain);

bool AppMain::OnInit()
{
	Context::Initialize(new Generator_M6800());
	Config &cfg = Config::GetInst();
	cfg.Restore();
	FrameMain *pFrame = new FrameMain(nullptr, wxPoint(cfg.FrameMain.x, cfg.FrameMain.y),
									  wxSize(cfg.FrameMain.width, cfg.FrameMain.height));
	pFrame->Show(true);
	return true;
}
