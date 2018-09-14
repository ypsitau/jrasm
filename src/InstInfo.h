//=============================================================================
// InstInfo.h
//=============================================================================
#ifndef __INSTINFO_H__
#define __INSTINFO_H__

#include "Common.h"

class ExprList;
class ExprOwner;

//-----------------------------------------------------------------------------
// InstInfo
//-----------------------------------------------------------------------------
class InstInfo {
public:
	class Rule {
	protected:
		UInt8 _code;
	public:
		inline Rule(UInt8 code) : _code(code) {}
		virtual ~Rule();
		virtual size_t Apply(Context &context, const ExprList &exprList) = 0;
	};
	class Rule_ACC : public Rule {
	private:
		String _accName;
	public:
		enum { bytes = 1 };
	public:
		inline Rule_ACC(UInt8 code, const char *accName = "") : Rule(code), _accName(accName) {}
		virtual size_t Apply(Context &context, const ExprList &exprList);
	};
	class Rule_REL : public Rule {
	public:
		enum { bytes = 2 };
	public:
		inline Rule_REL(UInt8 code) : Rule(code) {}
		virtual size_t Apply(Context &context, const ExprList &exprList);
	};
	class Rule_INH : public Rule {
	public:
		enum { bytes = 1 };
	public:
		inline Rule_INH(UInt8 code) : Rule(code) {}
		virtual size_t Apply(Context &context, const ExprList &exprList);
	};
	class Rule_IMM8 : public Rule {
	private:
		String _accName;
	public:
		enum { bytes = 2 };
	public:
		inline Rule_IMM8(UInt8 code, const char *accName = "") : Rule(code), _accName(accName) {}
		virtual size_t Apply(Context &context, const ExprList &exprList);
	};
	class Rule_IMM16 : public Rule {
	public:
		enum { bytes = 3 };
	public:
		inline Rule_IMM16(UInt8 code) : Rule(code) {}
		virtual size_t Apply(Context &context, const ExprList &exprList);
	};
	class Rule_DIR : public Rule {
	private:
		String _accName;
	public:
		enum { bytes = 2 };
	public:
		inline Rule_DIR(UInt8 code, const char *accName = "") : Rule(code), _accName(accName) {}
		virtual size_t Apply(Context &context, const ExprList &exprList);
	};
	class Rule_IDX : public Rule {
	private:
		String _accName;
	public:
		enum { bytes = 2 };
	public:
		inline Rule_IDX(UInt8 code, const char *accName = "") : Rule(code), _accName(accName) {}
		virtual size_t Apply(Context &context, const ExprList &exprList);
	};
	class Rule_EXT : public Rule {
	private:
		String _accName;
	public:
		enum { bytes = 3 };
	public:
		inline Rule_EXT(UInt8 code, const char *accName = "") : Rule(code), _accName(accName) {}
		virtual size_t Apply(Context &context, const ExprList &exprList);
	};
	typedef std::vector<Rule *> RuleList;
	class RuleOwner : public RuleList {
	public:
		~RuleOwner();
		void Clear();
	};
	class Entry {
	private:
		String _symbol;
		String _syntaxDesc;
		RuleOwner _ruleOwner;
	public:
		Entry(const String &symbol, const String &syntaxDesc);
		bool ApplyRule(Context &context, const ExprList &exprList) const;
		inline const char *GetSymbol() const { return _symbol.c_str(); }
		inline const char *GetSyntaxDesc() const { return _syntaxDesc.c_str(); }
		inline void AddRule(Rule *pRule) { _ruleOwner.push_back(pRule); }
	};
	class EntryMap : public std::map<String, Entry *> {
	public:
		~EntryMap();
		void Initialize();
		inline void Add(Entry *pEntry) {
			insert(std::make_pair(ToLower(pEntry->GetSymbol()), pEntry));
		}
		const Entry *Lookup(const char *symbol) const;
	};
private:
	EntryMap _entryMap;
public:
	InstInfo();
	void InitializeEntryMap();
	bool EvalExpr(Context &context, const Expr *pExpr);
private:
	static Entry *Entry_ACC(const String &symbol, UInt8 codeACC);
	static Entry *Entry_REL(const String &symbol, UInt8 codeREL);
	static Entry *Entry_INH(const String &symbol, UInt8 codeINH);
	static Entry *Entry_ACC_ACC(const String &symbol, UInt8 codeACC_A, UInt8 codeACC_B);
	static Entry *Entry_IDX_EXT(const String &symbol, UInt8 codeIDX, UInt8 codeEXT);
	static Entry *Entry_DIR_IDX_EXT(
		const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeEXT);
	static Entry *Entry_DIR_IDX_IMM8_EXT(
		const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeIMM8, UInt8 codeEXT);
	static Entry *Entry_DIR_IDX_IMM16_EXT(
		const String &symbol, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeIMM16, UInt8 codeEXT);
	static Entry *Entry_ACC_ACC_IDX_EXT(
		const String &symbol, UInt8 codeACC_A, UInt8 codeACC_B, UInt8 codeIDX, UInt8 codeEXT);
	static Entry *Entry_IMM8_DIR_IDX_EXT(
		const String &symbol, UInt8 codeIMM8, UInt8 codeDIR, UInt8 codeIDX, UInt8 codeEXT);
	static Entry *Entry_AxB_DIR_IDX_EXT	(
		const String &symbol,
		UInt8 codeDIR_A, UInt8 codeIDX_A, UInt8 codeEXT_A,
		UInt8 codeDIR_B, UInt8 codeIDX_B, UInt8 codeEXT_B);
	static Entry *Entry_AxB_IMM8_DIR_IDX_EXT(
		const String &symbol,
		UInt8 codeIMM8_A, UInt8 codeDIR_A, UInt8 codeIDX_A, UInt8 codeEXT_A,
		UInt8 codeIMM8_B, UInt8 codeDIR_B, UInt8 codeIDX_B, UInt8 codeEXT_B);
};

#endif
