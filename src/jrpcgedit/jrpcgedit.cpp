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
	virtual int OnExit();
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

int AppMain::OnExit()
{
	Config &cfg = Config::GetInst();
	cfg.Save();
	return 0;
}

