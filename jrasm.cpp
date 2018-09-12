//=============================================================================
// jrasm.cpp
//=============================================================================
#include "jrasm.h"

class ListenerEx : public Tokenizer::Listener {
public:
	virtual bool FeedToken(const Token &token);
};

bool ListenerEx::FeedToken(const Token &token)
{
	::printf("%s\n", token.ToString().c_str());
	return true;
}

bool Parse(const char *fileName)
{
	FILE *fp;
	fp = ::fopen(fileName, "rt");
	ListenerEx listener;
	Tokenizer tokenizer(&listener);
	for (;;) {
		int chRaw = ::fgetc(fp);
		char ch = (chRaw < 0)? '\0' : static_cast<unsigned char>(chRaw);
		if (!tokenizer.FeedChar(ch)) {
			::fprintf(stderr, "%s\n", tokenizer.GetErrMsg());
		}
		if (ch == '\0') break;
	}
	::fclose(fp);
	return true;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		::fprintf(stderr, "usage: jasm file");
		::exit(1);
	}
	::Parse(argv[1]);
	return 0;
}
