#ifndef DATOKEN_H
#define DATOKEN_H

#include <vector>
#include <string>
#include <stack>

#include "script.h"

class TokenData {
	public:
		enum HashType {
			HtHash = 0,
			HtDoubleHash,
			HtEnd
		};
		
		enum TokenType {
			TkHash=0,
			TkDoubleHash,
			TkHashEnd,
			TkData,
			TkArg,
			TkBuffer,
		};
		
		enum ContextType {
			CtxBlock = 0,
			CtxData,
			CtxDataBlock,
			
			CtxMax
		};
		
		enum TokenStatus {
			TsOk = 0,
			TsNoToken,
			
			// Token error types
			TsErrNoCloseString,
			TsErrBlockReqNewline,
			TsErrBlockNoClosing,
			TsErrIllegalCharacter,
			
			TsErrorStart = TsErrNoCloseString
		};
		
		enum SeekStatus {
			SeekHash,
			SeekData,
			SeekArgWord,
			SeekArgString,
			SeekArgBlock,
			SeekNewline,
			SeekEnd,
			SeekError,
			
			SeekUser
		};
		
		enum ContextSwitch {
			CsNone = 0,
			CsPop,
			CsPush,
		};
		
		static bool isChar(char ch);
		static bool isNum(char ch);
		static bool isSpecial(char ch);
		static bool isWord(char ch);
		static bool isSpace(char ch);
		static bool isNewline(char ch);
		static bool isWhite(char ch);
		static bool checkHash(char ch);
		static bool checkIdentifier(char ch);
		
		bool notEof()
		{ return mCur<mStr->size(); }
		
		char getCur()		{ return notEof() ?  getCurRaw() : '\0' ; }
		char getCurRaw()	{ return (*mStr)[mCur]; }
		
		size_t helpFindIndent();
		void helpGenIdent(size_t &idStart, size_t &idEnd);
		bool helpSeekNewline(bool checkWhite);
		bool helpGenHashIdent(size_t &idStart, size_t &idEnd, HashType &type);
		bool helpGenBuffer(bool removeTab, char endMarker, std::string &arg);
		
		HashType genHash(Script::Block *curBlock);
		void genBuffer(Script::Data  &data);
		
		void genArgWord(Script::Data &data);
		void genArgString(Script::Data &data);
		void genArgBlock(Script::Data &data);
		
		void genCtxArg(Script::Data &data);
		void genCtxBlock(Script::Block *curBlock);
		void genCtxData(Script::Block *curBlock);
		
		std::string getSubString(size_t st, size_t en)
			{ return mStr->substr(st,en-st); }
		
		SeekStatus seekBlock();
		SeekStatus seekArg();
		
		size_t curLine() { return mLine; }
		size_t curCol() { return mCol; }
		size_t curPos() { return mCur; }
		void iterCur();
		
		TokenData(Script &scr) : mScript(scr)
			{}
		
		void setupGenerator(const std::string &str,
			size_t stLine=1, size_t stCol=1);
		void generate(const std::string &str);
	private:
		size_t mCur;
		size_t mLine, mCol;
		const std::string *mStr;
		
		Script &mScript;
};

#endif
