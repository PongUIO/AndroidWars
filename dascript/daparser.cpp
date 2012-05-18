#include <stdio.h>
#include "daparser.h"
#include "dacodec.h"

// Default codec objects
static DaText daTextCodec;
static DaBase64 daBase64Codec;

// Character types
//
//
/// Any unicode character or english alphabet letters (a-zA-Z)
bool DaParser::isChar(char ch)
{	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')  || ch == '_'
	|| (ch&(1<<7)); }

/// A base-10 numerical value
bool DaParser::isNum(char ch)
{	return (ch >= '0' && ch <= '9'); }

/// Other special characters
bool DaParser::isSpecial(char ch)
{	return //_SpecialClass.check(ch);
		((ch >= '%' && ch <= '.') &&
		(ch != '\"' && ch != '#' && ch != '&')) ||
		(ch >= ':' && ch <= '@' && ch != ';') ||
		(ch >= '[' && ch <= '`' && ch != '\\') ||
		ch == '|' || ch == '~'; 
}

bool DaParser::isSpace(char ch)
{	return (ch == '\t' || ch == ' '); }

bool DaParser::isNewline(char ch)
{	return (ch == '\n' || ch == '\0'); }

bool DaParser::isWhite(char ch)
{	return isSpace(ch) || isNewline(ch); }

/// Checks if a character is a hash marker
bool DaParser::checkHash(char ch)
{	return (ch == '#'); }

/// Checks for a comment marker
bool DaParser::checkComment(char ch)
{	return (ch == '/'); }

/// Checks if a character is a legal identifier
bool DaParser::checkIdentifier(char ch)
{	return (checkWord(ch)); }

/// Check if a character is a legal character in a word argument
bool DaParser::checkWord(char ch)
{	return isChar(ch) || isNum(ch) || isSpecial(ch); }

/// Check if a character is a legal character in a reference string
bool DaParser::checkReference(char ch)
{	return checkIdentifier(ch) || ch == '/' || ch == '.' || ch == '[' || ch == ']'; }

/// Checks if a character is an opening string marker
bool DaParser::checkString(char ch)
{	return ch == '\"'; }

/// Checks if a character is a block opening marker
bool DaParser::checkBlock(char ch)
{	return ch == '{'; }



// Helper functions
//
//
size_t DaParser::helpFindIndent()
{
	size_t ind = 0;
	while(notEof()) {
		if( isSpace(getCurRaw()) ) ind++;
		else return ind;
		
		iterCur();
	}
	
	return ind;
}

bool DaParser::helpSeekNewline(bool checkWhite)
{
	bool ret = false;
	
	while( notEof() ) {
		char ch = getCurRaw();
		
		if( isNewline(ch) ) {
			iterCur();
			break;
		} else if( checkWhite && !isSpace(ch) ) {
			mScript.addError(DaScript::WarnReqNewline, "", mLine, mCol);
			checkWhite=false;
			ret = true;
		}
		
		iterCur();
	}
	
	return ret;
}

void DaParser::helpGenIdent(size_t& idStart, size_t& idEnd)
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

/**
 * Generates a string buffer from the current cursor position, storing
 * the result in \p arg. A string buffer is opened with a '{', and closed with
 * a '}' on an empty line with a lower indentation than the data itself.
 */
bool DaParser::helpGenBuffer(std::string& arg, size_t baseInd)
{
	size_t stLine = mLine, stCol = mCol;
	
	// Calculate the tabulator size for a data argument
	int tabsize = baseInd;
	
	int curTab = tabsize;
	int lineStart = mCur;
	while( notEof() ) {
		char ch = getCurRaw();
		
		if(curTab<=0) {
			if(isNewline(ch)) {
				arg.append(mStr->begin()+lineStart,mStr->begin()+mCur+1);
				curTab = tabsize;
				lineStart=mCur+1;
			}
			
			iterCur();
		}
		else if( isSpace(ch) ) {
			curTab--;
			if(curTab<=0)
				lineStart=mCur+1;
			iterCur();
		}
		else if( ch == '}' ) {
			// Move past the end marker
			iterCur();
			if(arg.size()>0)
				arg.erase(arg.size()-1);
			return false;
		}
		else {
			curTab=0;
			lineStart=mCur;
		}
	}
	
	mScript.addError(DaScript::ErrBlockNoClosing, "", stLine,stCol, mLine, mCol);
	return true;
}

/**
 * Skips a comment block.
 * 
 * Will, depending on if a single or multi-line comment was found, iterate
 * through the respective comment type. If this is not a real comment,
 * notify the error and return.
 */
void DaParser::skipComment()
{
	iterCur();
	
	// Get identifier
	char ch = getCur();
	if(!(ch=='*' || ch=='/')) {
		mScript.addError( DaScript::ErrIllegalCharacter, std::string(1,ch),
			mLine,mCol);
		helpSeekNewline(false);
		return;
	}
	
	bool isMultiline = (ch == '*');
	iterCur();
	
	while(notEof()) {
		iterCur();
		ch = getCur();
		
		// Multiline:
		// If closing marker detected, check the next character and iterate
		// to it if closing.
		if(isMultiline) {
			if(ch == '*' && getNext() == '/') {
				iterCur();
				break;
			}
		}
		// Singleline:
		// Break on end-of-line
		else if(isNewline(ch))
			break;
	}
	
	iterCur();
}


// Token generators
//
//
void DaParser::genArgString(DaScript::Node &data)
{
	iterCur();
	size_t start = mCur;
	size_t stLine=mLine, stCol=mCol;
	
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
		} else if( isNewline(ch) )
			break;
		
		iterCur();
	}
	
	mScript.addError(DaScript::ErrNoCloseString, "", stLine, stCol, mLine, mCol);
	return;
}

void DaParser::genArgWord(DaScript::Node &data)
{
	size_t start = mCur;
	
	while( true ) {
		char ch = getCurRaw();
		
		if( checkWord(ch) )
			iterCur();
		else {
			data.createArg( mStr->substr(start, mCur-start));
			return;
		}
	}
}

/**
 * Generates a reference-type argument. A reference argument is identified by
 * a '&', and contains a \c Node::lookupNode() compatible string.
 */
void DaParser::genArgReference(DaScript::Node& data)
{
	iterCur();
	size_t start = mCur;
	size_t stLine=mLine, stCol=mCol;
	
	while( true ) {
		char ch = getCurRaw();
		
		if( checkReference(ch) )
			iterCur();
		else {
			mRef.push_back( Reference(&data, mStr->substr(start,mCur-start),
				stLine, stCol) );
			return;
		}
	}
}

void DaParser::genArgBlock(DaScript::Node &data)
{
	// Move off the opening brace
	iterCur();
	
	// Build identifier
	while( isSpace(getCur()) )
		iterCur();
	size_t start, end;
	helpGenIdent(start,end);
	
	// Seek to the next line
	helpSeekNewline(true);
	
	// Generate the block
	std::string &arg = data.createArg();
	helpGenBuffer(arg,data.getCol());
	
	// Decode the block
	DaCodec *codec = &daTextCodec;
	if( (end-start) > 0) {
		DaCodec::CodecPack::iterator i=mCodec.find(getSubString(start,end));
		if(i != mCodec.end())
			codec = i->second;
	}
	codec->decode(arg);
}


// Contexts
//
//
void DaParser::genCtxArg(DaScript::Node &data)
{
	while( notEof() ) {
		char ch = getCurRaw();
		
		if( isSpace(ch) ) {
			iterCur();
		} else if( checkWord(ch) ) {
			genArgWord(data);
		} else if( checkString(ch) ) {
			genArgString(data);
		} else if( checkBlock(ch) ) {
			genArgBlock(data);
		} else if( isNewline(ch) || ch == ';' ) {
			iterCur();
			return;
		} else if( ch == '\\' ) {
			iterCur();
			helpSeekNewline(true);
		} else if( ch == '&' ) {
			genArgReference(data);
		} else {
			mScript.addError(DaScript::ErrIllegalCharacter, "", mLine, mCol);
			helpSeekNewline(false);
		}
	}
}

/**
 * Data generation context. Parses the arguments for a node, then if this
 * had a hash-marker nested nodes are parsed.
 */
bool DaParser::genCtxData(DaScript::Node &curBlock)
{
	size_t stL=mLine, stC=mCol;
	
	// Analyse the hash marker to determine type
	bool isHash = false;
	if(getCurRaw() == '#') {
		iterCur();
		
		char ch = getCur();
		if( checkIdentifier(ch) )	// This is a normal opening block
			isHash = true;
		else if( isWhite(ch) )		// This is a closing block
			return false;
		else {						// This is not valid data
			mScript.addError(DaScript::ErrIllegalCharacter, 
				std::string(1,ch),mLine,mCol
			);
			helpSeekNewline(false);
			return true;
		}
	}
	
	// Build the identifier and create the data node
	size_t start, end;
	helpGenIdent(start,end);
	DaScript::Node &data = curBlock.createNode(
		mStr->substr(start,mCur-start), DaScript::Node::NoPos, stL, stC );
	
	// Process the data context
	genCtxArg(data);
	if(isHash)
		genCtxBlock(data);
	
	return true;
}

void DaParser::genCtxBlock(DaScript::Node &curBlock)
{
	size_t stLine = mLine, stCol = mCol;
	
	while( notEof() ) {
		char ch = getCurRaw();
		
		// Skip whitespace
		if( isWhite(ch) ) {
			iterCur();
		// Check for data block marker
		} else if( checkIdentifier(ch) || checkHash(ch) ) {
			// Parse the data block.
			// If an end marker is found, this returns false
			if(!genCtxData(curBlock))
				return;
		// Check for comment marker
		} else if( checkComment(ch) ) {
			skipComment();
		// Any other character is invalid, skip to the next line
		} else {
			mScript.addError( DaScript::ErrIllegalCharacter, std::string(1,ch),
				mLine, mCol
			);
			helpSeekNewline(false);
		}
	}
	
	if(curBlock.getParent()) {
		mScript.addError( DaScript::ErrPrematureEnd, "",
		stLine, stCol, mLine, mCol);
	}
}

void DaParser::iterCur()
{
	if(getCurRaw() == '\n') {
		mCol=0;
		mLine++;
	}
	
	mCol++;
	mCur++;
}

void DaParser::setupGenerator(const std::string& str, size_t stLine, size_t stCol)
{
	mCur = 0;
	mLine = stLine;
	mCol = stCol;
	mStr = &str;
}

DaParser::DaParser(DaScript& scr) : 
	mCur(0), mLine(0), mCol(0), mStr(0), mCodec(),
	mScript(scr), mRef()
{
	mCodec["text"] = &daTextCodec;
	mCodec["base64"] = &daBase64Codec;
}


void DaParser::parse(const std::string &str)
{
	// Prepare generation
	setupGenerator(str);
	
	genCtxBlock(mScript.mRoot);
}

/**
 * Resolves all node references. If any invalid references was detected,
 * ignore them and notify the error.
 */
void DaParser::resolveReferences()
{
	// Resolve all references
	for(size_t i=0; i<mRef.size(); ++i) {
		Reference &ref = mRef[i];
		
		// Find the actual node from the reference string
		DaScript::Node &lookup = ref.mSrc->lookupNode(ref.mRef);
		
		if(lookup.isEmpty()) {
			mScript.addError(DaScript::ErrInvalidRef, ref.mRef,
				ref.mLine, ref.mCol);
		} else {
			ref.mSrc->createReference(lookup);
			
			// Test for circular references
			size_t circRef =0;
			while( (circRef=ref.mSrc->getCircularReference()) != DaScript::Node::NoPos ) {
				mScript.addError(DaScript::ErrCircularRef, ref.mRef,
					ref.mLine, ref.mCol);
				ref.mSrc->removeReference(circRef);
			}
		}
	}
	mRef.clear();
}
