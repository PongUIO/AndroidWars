#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <string>
#include <stack>

#include "dascript.h"

class DaParser {
	public:
		/// @name Character classes
		//@{
			static bool isChar(char ch);
			static bool isNum(char ch);
			static bool isSpecial(char ch);
			static bool isSpace(char ch);
			static bool isNewline(char ch);
			static bool isWhite(char ch);
		//@}
		
		/// @name Token classes
		//@{
			static bool checkWord(char ch);
			static bool checkHash(char ch);
			static bool checkComment(char ch);
			static bool checkIdentifier(char ch);
			static bool checkReference(char ch);
			static bool checkString(char ch);
			static bool checkBlock(char ch);
		//@}
		
		/// @name Helper functions
		//@{
			size_t helpFindIndent();
			void helpGenIdent(size_t &idStart, size_t &idEnd);
			bool helpSeekNewline(bool checkWhite);
			bool helpGenBuffer(std::string &arg, size_t baseInd);
			
			void skipComment();
			
			bool notEof()
			{ return mCur<mStr->size(); }
			
			char getCur()		{ return notEof() ?  getCurRaw() : '\0' ; }
			char getCurRaw()	{ return (*mStr)[mCur]; }
			char getNext()
			{ return mCur+1<(mStr->size()) ? (*mStr)[mCur+1] : '\0'; }
			
			std::string getSubString(size_t st, size_t en)
			{ return mStr->substr(st,en-st); }
		
			void iterCur();
		//@}
		
		/// @name Argument parsing contexts
		//@{
			void genArgWord(DaScript::Node &data);
			void genArgString(DaScript::Node &data);
			void genArgBlock(DaScript::Node &data);
			void genArgReference(DaScript::Node &data);
		//@}
		
		/// @name Top-level contexts
		//@{
			void genCtxArg(DaScript::Node &data);
			void genCtxBlock(DaScript::Node &curBlock);
			bool genCtxData(DaScript::Node &curBlock);
		//@}
		
		/// @name Parsing interface
		//@{
			DaParser(DaScript &scr);
			~DaParser() {}
			
			void parse(const std::string &str);
			void resolveReferences();
			DaCodec::CodecPack &getCodec() { return mCodec; }
		//@}
		
	private:
		void setupGenerator(const std::string &str,
			size_t stLine=1, size_t stCol=1);
		
		size_t mCur;
		size_t mLine, mCol;
		const std::string *mStr;
		DaCodec::CodecPack mCodec;
		
		DaScript &mScript;
		
		struct Reference {
			Reference(DaScript::Node *src=0, const std::string &ref="",
				size_t line=0, size_t col=0) :
				mSrc(src), mRef(ref), mLine(line), mCol(col) {}
			
			DaScript::Node *mSrc;
			std::string mRef;
			size_t mLine, mCol;
		};
		typedef std::vector<Reference> RefVec;
		RefVec mRef;
};

#endif
