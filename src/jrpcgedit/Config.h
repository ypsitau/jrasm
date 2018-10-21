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
public:
	struct {
		int x, y;
		int width, height;
	} FrameMain;
public:
	static Config &GetInst() { return _cfg; }
	bool Save();
	bool Restore();
};

#endif
