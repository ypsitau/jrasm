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
	ErrorLog::Clear();
	Directive::Initialize();
	Operator::Initialize();
	Generator::Initialize(new Generator_M6800());
	FrameMain *pFrame = new FrameMain(nullptr, wxT("JR-200 PCG Editor"), wxPoint(50, 50), wxSize(800, 600));
	pFrame->Show(true);
	return true;
}
