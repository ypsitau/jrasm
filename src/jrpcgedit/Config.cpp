//=============================================================================
// Config.cpp
//=============================================================================
#include "stdafx.h"

//-----------------------------------------------------------------------------
// Config
//-----------------------------------------------------------------------------
Config Config::_cfg;
const wxChar *Config::_appName = wxT("jrpcgedit");
const wxChar *Config::_vendorName = wxT("ypsitau");

Config::Config()
{
	FrameMain.x = -1;
	FrameMain.y = -1;
	FrameMain.width = 800;
	FrameMain.height = 600;
}

bool Config::Save()
{
	std::unique_ptr<wxConfig> pCfg(new wxConfig(_appName, _vendorName));
	//::printf("Config::Save()\n");
	pCfg->Write(wxT("/FrameMain/x"), FrameMain.x);
	pCfg->Write(wxT("/FrameMain/y"), FrameMain.y);
	pCfg->Write(wxT("/FrameMain/width"), FrameMain.width);
	pCfg->Write(wxT("/FrameMain/height"), FrameMain.height);
	return true;
}

bool Config::Restore()
{
	std::unique_ptr<wxConfig> pCfg(new wxConfig(_appName, _vendorName));
	::printf("%s\n", (const char *)wxFileConfig::GetLocalFile(wxT("jrpcgedit")).GetFullName());
	//::printf("Config::Restore()\n");
	pCfg->Read(wxT("/FrameMain/x"), &FrameMain.x);
	pCfg->Read(wxT("/FrameMain/y"), &FrameMain.y);
	pCfg->Read(wxT("/FrameMain/width"), &FrameMain.width);
	pCfg->Read(wxT("/FrameMain/height"), &FrameMain.height);
	return true;
}
