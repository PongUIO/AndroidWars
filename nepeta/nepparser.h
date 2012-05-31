/**
 * This file is part of the Nepeta project.
 * 
 * Licensed under GNU LGPL (see license.txt and README)
 */
#ifndef _NEPPARSER_H
#define _NEPPARSER_H

#include <vector>
#include <string>

#include "nepeta.h"
#include "nepcodec.h"

class NepParser {
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
			static bool checkHash(char ch);
			static bool checkComment(char ch);
			static bool checkIdentifier(char ch);
			static bool checkReference(char ch);
			static bool checkString(char ch);
			static bool checkBlock(char ch);
		//@}
		
		/// @name Helper functions
		//@{
			void helpGenIdent(size_t &idStart, size_t &idEnd);
			bool helpSeekNewline(bool checkWhite);
			bool helpGenBuffer(std::string &arg, size_t baseInd,
				char endMarker='}');
			bool helpGenString(std::string &arg, char endMarker='\"');
			void helpSkipComment();
			
			bool notEof()
			{ return mCur<mStr->size(); }
			
			char getCur()		{ return notEof() ?  getCurRaw() : '\0' ; }
			char getCurRaw()	{ return (*mStr)[mCur]; }
			char getNext()
			{ return mCur+1<(mStr->size()) ? (*mStr)[mCur+1] : '\0'; }
			
			std::string getSubString(size_t st, size_t en)
			{ return mStr->substr(st,en-st); }
		
			void iterCur();
			
			size_t getCurLine() const { return mLine; }
			size_t getCurCol() const { return mCol; }
			size_t getCurPos() const { return mCur; }
		//@}
		
		/// @name Argument parsing contexts
		//@{
			void genArgWord(Nepeta::Node &data);
			void genArgString(Nepeta::Node &data);
			void genArgBlock(Nepeta::Node &data);
			void genArgReference(Nepeta::Node &data);
		//@}
		
		/// @name Top-level contexts
		//@{
			void genCtxArg(Nepeta::Node &data);
			void genCtxBlock(Nepeta::Node &node);
			bool genCtxData(Nepeta::Node &node);
		//@}
		
		/// @name Parsing interface
		//@{
			NepParser(Nepeta &scr);
			~NepParser() {}
			
			void parse(const std::string &str);
			void resolveReferences();
			NepCodec::CodecPack &getCodec() { return mCodec; }
			
			void setupGenerator(const std::string &str,
			size_t stLine=1, size_t stCol=1);
		//@}
		
	private:
		size_t mCur;
		size_t mLine, mCol;
		const std::string *mStr;
		NepCodec::CodecPack mCodec;
		
		Nepeta &mScript;
		
		struct Reference {
			Reference(Nepeta::Node *src=0, const std::string &ref="",
				size_t line=0, size_t col=0) :
				mSrc(src), mRef(ref), mLine(line), mCol(col) {}
			
			Nepeta::Node *mSrc;
			std::string mRef;
			size_t mLine, mCol;
		};
		typedef std::vector<Reference> RefVec;
		RefVec mRef;
};

#endif
