#include <stdio.h>
#include "token.h"
#include "script.h"

// Character types
//
//
bool TokenData::isChar(char ch)
{	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')  || ch == '_'; }

bool TokenData::isNum(char ch)
{	return (ch >= '0' && ch <= '9'); }

bool TokenData::isSpecial(char ch)
{	return (ch == '-' || ch == '+' || ch == '.'); }

bool TokenData::isWord(char ch)
{	return isChar(ch) || isNum(ch) || isSpecial(ch); }

bool TokenData::isSpace(char ch)
{	return (ch == '\t' || ch == ' '); }

bool TokenData::isNewline(char ch)
{	return (ch == '\n' || ch == '\0'); }

bool TokenData::isWhite(char ch)
{	return isSpace(ch) || isNewline(ch); }

/// Checks if a character is a hash marker
bool TokenData::checkHash(char ch)
{	return (ch == '#'); }

/// Checks if a character is a legal identifier
bool TokenData::checkIdentifier(char ch)
{	return (isChar(ch) || isNum(ch) || ch == '_'); }

// Helper functions
//
//
size_t TokenData::helpFindIndent()
{
	size_t ind = 0;
	while(notEof()) {
		if(getCurRaw() == '\t') ind++;
		else return ind;
		
		iterCur();
	}
	
	return ind;
}

bool TokenData::helpSeekNewline(bool checkWhite)
{
	bool ret = false;
	
	while( notEof() ) {
		char ch = getCurRaw();
		
		if( isNewline(ch) ) {
			iterCur();
			break;
		} else if( checkWhite && !isSpace(ch) ) {
			mScript.addError(Script::WarnReqNewline, mLine, mCol);
			checkWhite=false;
			ret = true;
		}
		
		iterCur();
	}
	
	return ret;
}

void TokenData::helpGenIdent(size_t& idStart, size_t& idEnd)
{
	idStart = mCur;
	while( true ) {
		char ch = getCur();
		
		if( !checkIdentifier(ch) ) {
			idEnd = mCur;
			break;
		}
		
		iterCur();
	}
}

bool TokenData::helpGenHashIdent(size_t &idStart, size_t &idEnd, HashType& type)
{
	iterCur();
	
	char ch = getCur();
	if( ch == '#' ) {
		type = HtDoubleHash;
		iterCur();
	} else if( !checkIdentifier(ch) ) {
		type = HtEnd;
		return false;
	} else
		type = HtHash;
	
	helpGenIdent(idStart,idEnd);
	
	return false;
}

bool TokenData::helpGenBuffer(bool removeTab, char endMarker, std::string& arg)
{
	size_t stLine = mLine, stCol = mCol;
	
	// Calculate the tabulator size if a data argument
	int tabsize = 0;
	if(removeTab)
		tabsize = helpFindIndent();
	
	int curTab = 0;
	int lineStart = mCur;
	bool checkClosing = true;
	while( notEof() ) {
		char ch = getCurRaw();
		
		if(curTab<=0) {
			if(isNewline(ch)) {
				arg += mStr->substr(lineStart, mCur-lineStart+1);
				curTab = tabsize;
				lineStart=mCur+1;
				checkClosing=true;
			}
			else if( ch == '\\' ) {
				arg += mStr->substr(lineStart, mCur-lineStart);
				lineStart = mCur+1;
			}
			else if( checkClosing ) {
				if( ch == endMarker ) {
					// Move past the end marker
					iterCur();
					if(arg.size()>0)
						arg.erase(arg.size()-1);
					
					return false;
				}
				else if( !isSpace(ch) )
					checkClosing=false;
			}
			
			iterCur();
		}
		else if(ch == '\t') {
			curTab--;
			if(curTab<=0)
				lineStart=mCur+1;
			iterCur();
		}
		else {
			curTab=0;
			lineStart=mCur;
		}
	}
	
	mScript.addError(Script::ErrBlockNoClosing, stLine,stCol, mLine, mCol);
	return true;
}



// Token generators
//
//
/**
 * Generates a hash token.
 * 
 * Resulting token types:
 * - TkHash
 * - TkDoubleHash
 * - TkHashEnd
 */
TokenData::HashType TokenData::genHash(Script::Block *curBlock)
{
	HashType hashType;
	
	size_t stL = mLine, stC = mCol;
	
	// Parse the identifier
	size_t start,end;
	helpGenHashIdent(start,end,hashType);
	
	// Get to a new line
	helpSeekNewline(true);
	
	if(hashType != HtEnd) {
		std::string id = mStr->substr(start, end-start);
		
		if(hashType == HtHash)
			genCtxBlock( curBlock->createBlock(id, stL, stC) );
		else
			genBuffer(curBlock->createData(id, stL, stC));
	}
	
	return hashType;
}

/**
 * Generates a hashed buffer
 */
void TokenData::genBuffer(Script::Data &data)
{
	helpGenBuffer(false, '#', data.createArg());
	helpSeekNewline(true);
}

void TokenData::genArgString(Script::Data &data)
{
	iterCur();
	size_t start = mCur;
	
	std::string &str = data.createArg();
	while( notEof() ) {
		char ch = getCurRaw();
		
		if( ch == '\"' ) {
			str += mStr->substr(start, mCur-start);
			iterCur();
			return;
		} else if(ch == '\\') {
			str += mStr->substr(start, mCur-start);
			iterCur();
			start = mCur;
		}
		
		iterCur();
	}
	
	mScript.mErrors.push_back( Script::Error(Script::ErrNoCloseString) );
	return;
}

void TokenData::genArgWord(Script::Data &data)
{
	size_t start = mCur;
	
	while( true ) {
		char ch = getCurRaw();
		
		if( isWord(ch) )
			iterCur();
		else {
			data.addArg( mStr->substr(start, mCur-start));
			return;
		}
	}
}

void TokenData::genArgBlock(Script::Data &data)
{
	// Move off the opening brace
	iterCur();
	
	// Seek to the next line
	helpSeekNewline(true);
	
	// Generate the block
	helpGenBuffer(true, '}', data.createArg());
}


// Token seekers
//
//
/**
 * Searches for a block token marker
 */
TokenData::SeekStatus TokenData::seekBlock()
{
	while( notEof() ) {
		char ch = getCurRaw();
		
		if( isWhite(ch) )
			iterCur();
		else if( checkHash(ch) )
			return SeekHash;
		else if( checkIdentifier(ch) )
			return SeekData;
		else
			return SeekError;
	}
	
	return SeekEnd;
}

/**
 * Searches for any argument marker
 */
TokenData::SeekStatus TokenData::seekArg()
{
	while( notEof() ) {
		char ch = getCurRaw();
		
		if( isSpace(ch) ) {
			iterCur();
		} else if( isWord(ch) ) {
			return SeekArgWord;
		} else if( ch == '\"' ) {
			return SeekArgString;
		} else if( ch == '{' ) {
			return SeekArgBlock;
		} else if( isNewline(ch) ) {
			return SeekNewline;
		} else
			return SeekError;
	}
	
	return SeekNewline;
}

// Contexts
//
//
void TokenData::genCtxBlock(Script::Block *curBlock)
{
	size_t stLine = mLine, stCol = mCol;
	
	while( true ) {
		SeekStatus seek = seekBlock();
		switch( seek )
		{
			case SeekHash:
			{
				HashType type = genHash(curBlock);
				if(type == HtEnd)
					goto EndWhile;
			}
				break;
			
			case SeekData:
				genCtxData(curBlock);
				break;
			
			case SeekEnd:
				if(curBlock->getParent())
					mScript.addError( Script::ErrPrematureEnd, stLine, stCol, mLine, mCol);
				goto EndWhile;
				break;
			
			default:
				mScript.addError( Script::ErrIllegalCharacter, mLine, mCol);
				helpSeekNewline(false);
				goto EndWhile;
		}
	}
	
	EndWhile:
	;
		
}

void TokenData::genCtxArg(Script::Data& data)
{
	while( true ) {
		SeekStatus seek = seekArg();
		
		switch(seek)
		{
			case SeekArgString:
				genArgString(data);
				break;
			
			case SeekArgWord:
				genArgWord(data);
				break;
			
			case SeekArgBlock:
				genArgBlock(data);
				break;
			
			case SeekNewline:
			case SeekEnd:
				goto EndWhile;
				break;
			
			default:
				mScript.addError(Script::ErrIllegalCharacter, mLine, mCol);
				helpSeekNewline(false);
				goto EndWhile;
		}
	}
	
	EndWhile: ;
}


void TokenData::genCtxData(Script::Block *curBlock)
{
	size_t stL=mLine, stC=mCol;
	
	size_t start, end;
	helpGenIdent(start,end);
	Script::Data &data = curBlock->createData( mStr->substr(start,mCur-start), stL, stC );
	
	genCtxArg(data);
}

void TokenData::iterCur()
{
	if(getCurRaw() == '\n') {
		mCol=0;
		mLine++;
	}
	
	mCol++;
	mCur++;
}

void TokenData::setupGenerator(const std::string& str, size_t stLine, size_t stCol)
{
	mCur = 0;
	mLine = stLine;
	mCol = stCol;
	mStr = &str;
}

void TokenData::generate(const std::string &str)
{
	// Prepare generation
	setupGenerator(str);
	
	genCtxBlock(&mScript.mRoot);
}
