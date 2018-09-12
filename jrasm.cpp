//=============================================================================
// jrasm.cpp
//=============================================================================
#include "jrasm.h"

class Parser : public Tokenizer::Listener {
private:
	Tokenizer _tokenizer;
public:
	Parser();
	inline bool FeedChar(char ch) { return _tokenizer.FeedChar(ch); }
	inline const char *GetErrMsg() const { return _tokenizer.GetErrMsg(); }
	virtual bool FeedToken(const Token &token);
};

Parser::Parser() : _tokenizer(this)
{
}

bool Parser::FeedToken(const Token &token)
{
	::printf("%s\n", token.ToString().c_str());
	return true;
}

bool Parse(const char *fileName)
{
	FILE *fp;
	fp = ::fopen(fileName, "rt");
	Parser parser;
	for (;;) {
		int chRaw = ::fgetc(fp);
		char ch = (chRaw < 0)? '\0' : static_cast<unsigned char>(chRaw);
		if (!parser.FeedChar(ch)) {
			::fprintf(stderr, "%s\n", parser.GetErrMsg());
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
