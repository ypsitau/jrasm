//=============================================================================
// Config.h
//=============================================================================
#ifndef __JRPCGEDIT_CONFIG_H__
#define __JRPCGEDIT_CONFIG_H__

//-----------------------------------------------------------------------------
// Config
//-----------------------------------------------------------------------------
class Config {
private:
	static Config _cfg;
	static const wxChar *_appName;
	static const wxChar *_vendorName;
public:
	struct {
		int x, y;
		int width, height;
	} FrameMain;
public:
	Config();
	static Config &GetInst() { return _cfg; }
	bool Save();
	bool Restore();
};

#endif
