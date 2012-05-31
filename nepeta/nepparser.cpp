/**
 * This file is part of the Nepeta project.
 * 
 * Licensed under GNU LGPL (see license.txt and README)
 */
#include "nepparser.h"

// Default codec objects
static NepText nepTextCodec;
static NepBase64 nepBase64Codec;

// Character types
//
//
/// Any unicode character or english alphabet letters (a-zA-Z)
bool NepParser::isChar(char ch)
{	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
	|| (ch&(1<<7)); }

/// A base-10 numerical value
bool NepParser::isNum(char ch)
{	return (ch >= '0' && ch <= '9'); }

/// Other special characters
bool NepParser::isSpecial(char ch)
{	return //_SpecialClass.check(ch);
		((ch >= '%' && ch <= '.') &&
		(ch != '\"' && ch != '#' && ch != '&')) ||
		(ch >= ':' && ch <= '@' && ch != ';') ||
		(ch >= '[' && ch <= '`' && ch != '\\') ||
		ch == '|' || ch == '~'; 
}

/// Space characters excluding newlines
bool NepParser::isSpace(char ch)
{	return (ch == '\t' || ch == ' '); }

/// Newline characters including null
bool NepParser::isNewline(char ch)
{	return (ch == '\n' || ch == '\0'); }

/// Any whitespace (space or newline)
bool NepParser::isWhite(char ch)
{	return isSpace(ch) || isNewline(ch); }

/// Checks if a character is a hash marker
bool NepParser::checkHash(char ch)
{	return (ch == '#'); }

/// Checks for a opening comment marker
bool NepParser::checkComment(char ch)
{	return (ch == '/'); }

/// Checks if a character is a legal identifier
bool NepParser::checkIdentifier(char ch)
{	return isChar(ch) || isNum(ch) || isSpecial(ch); }

/// Check if a character is a legal character in a reference string
bool NepParser::checkReference(char ch)
{	return checkIdentifier(ch) || ch == '/' || ch == '.' || ch == '[' || ch == ']'; }

/// Checks if a character is an opening string marker
bool NepParser::checkString(char ch)
{	return ch == '\"'; }

/// Checks if a character is a block opening marker
bool NepParser::checkBlock(char ch)
{	return ch == '{'; }



// Helper functions
//
//
/**
 * @brief Makes the parser skip until the next newline.
 * 
 * After a call to this, the parser cursor is at the beginning
 * of the next line.
 * 
 * @param checkWhite If true, an error message is raised if any
 * non-whitespace character occurs.
 * 
 * @return true if an error occurred, else false.
 */
bool NepParser::helpSeekNewline(bool checkWhite)
{
	bool ret = false;
	
	while( notEof() ) {
		char ch = getCurRaw();
		
		if( isNewline(ch) ) {
			break;
		} else if( checkWhite && !isSpace(ch) ) {
			mScript.addError(Nepeta::WarnReqNewline, "",
				getCurLine(), getCurCol());
			checkWhite=false;
			ret = true;
		}
		
		iterCur();
	}
	
	// Skips the newline character to the next line
	if(notEof())
		iterCur();
	
	return ret;
}

/**
 * @brief Generates an identifier.
 * 
 * At the end of a a call to this, the parser cursor is
 * just after the parsed identifier.
 * 
 * @param idStart Desitnation for the beginning of the identifier
 * @param idEnd Destination for the end of the identifier
 */
void NepParser::helpGenIdent(size_t& idStart, size_t& idEnd)
{
	idStart = getCurPos();
	while( true ) {
		char ch = getCur();
		
		if( !checkIdentifier(ch) ) {
			idEnd = getCurPos();
			break;
		}
		
		iterCur();
	}
}

/**
 * @brief Generates a string buffer from the current cursor position.
 *
 * A string buffer is closed when a @p endMarker is detected on an
 * empty line at the indentation of @p baseInd. When the function is
 * over, the parser cursor is at the character just after the closing marker.
 * 
 * @param baseInd The indentation at which end marker is checked and the
 * number of whitespace to skip for each line of data.
 * @param endMarker Character that marks the end of a buffer.
 * 
 * @return true if an error occurred, false otherwise.
 */
bool NepParser::helpGenBuffer(std::string& arg, size_t baseInd, char endMarker)
{
	size_t stLine = getCurLine(), stCol = getCurCol();
	
	int curTab = baseInd;
	int lineStart = getCurPos();
	while( notEof() ) {
		char ch = getCurRaw();
		
		if(curTab<=0) {
			if(isNewline(ch)) {
				arg.append( getSubString(lineStart, getCurPos()+1) );
				curTab = baseInd;
				lineStart=getCurPos()+1;
			}
			
			iterCur();
		}
		else if( isSpace(ch) ) {
			curTab--;
			if(curTab<=0)
				lineStart=getCurPos()+1;
			iterCur();
		}
		else if( ch == endMarker ) {
			// Move past the end marker
			iterCur();
			if(arg.size()>0)
				arg.erase(arg.size()-1);
			return false;
		}
		else {
			curTab=0;
			lineStart=getCurPos();
		}
	}
	
	mScript.addError(Nepeta::ErrBlockNoClosing, "", stLine,stCol,
		getCurLine(), getCurCol());
	return true;
}

/**
 * @brief Generates a string.
 * 
 * Will seek until the @p endMarker is found, any '\' escapes
 * the marker. The resulting string is stored in @p arg.
 * 
 * At the end of this function, the parser cursor is just after the closing
 * marker.
 * 
 * @return true if an error occurred, false otherwise.
 */
bool NepParser::helpGenString(std::string& arg, char endMarker)
{
	size_t start = getCurPos();
	size_t stLine=getCurLine(), stCol=getCurCol();
	
	while( notEof() ) {
		char ch = getCurRaw();
		
		if( ch == endMarker ) {
			arg += getSubString(start, getCurPos());
			iterCur();
			return false;
		} else if( ch == '\\') {
			arg += getSubString(start, getCurPos());
			iterCur();
			start = getCurPos();
		} else if( isNewline(ch) )
			break;
		
		iterCur();
	}
	
	mScript.addError(Nepeta::ErrNoCloseString, "", stLine, stCol,
		getCurLine(), getCurCol());
	return true;
}


/**
 * @brief Skips a comment block.
 * 
 * Will, depending on if a single or multi-line comment was found, iterate
 * through the respective comment type. If this is not a real comment,
 * notify the error and return.
 * 
 * The function assumes the parser is at a comment start marker '/'. It
 * ends where the parser cursor is after the closing comment (for
 * single-line comments this is at the newline, and for multi-line this is
 * at the character just after the closing '/').
 */
void NepParser::helpSkipComment()
{
	size_t stLine = getCurLine(), stCol = getCurCol();
	iterCur();
	
	// Get identifier
	char ch = getCur();
	if(!(ch=='*' || ch=='/')) {
		mScript.addError( Nepeta::ErrIllegalCharacter, std::string(1,ch),
			getCurLine(),getCurCol());
		helpSeekNewline(false);
		return;
	}
	
	bool isMultiline = (ch == '*');
	iterCur();
	
	bool didFinish = false;
	while(notEof()) {
		iterCur();
		ch = getCur();
		
		// Multiline:
		// If the closing marker detected, check the next character and iterate
		// to it if closing.
		if(isMultiline) {
			if(ch == '*' && getNext() == '/') {
				iterCur();
				didFinish = true;
				break;
			}
		}
		// Singleline:
		// Break on end-of-line
		else if(isNewline(ch))
			break;
	}
	
	if(isMultiline && !didFinish) {
		mScript.addError( Nepeta::ErrCommentNotClosed, "",
		stLine,stCol);
	}
	
	iterCur();
}


// Token generators
//
//
/**
 * @brief Generates a string type argument and inserts it into @p data.
 */
void NepParser::genArgString(Nepeta::Node &data)
{
	iterCur();
	helpGenString(data.createArg());
}

/**
 * @brief Generates a word type argument and inserts it into @p data.
 */
void NepParser::genArgWord(Nepeta::Node &data)
{
	size_t idStart, idEnd;
	helpGenIdent(idStart,idEnd);
	data.createArg( getSubString(idStart,idEnd) );
}

/**
 * @brief Generates a reference-type argument.
 * 
 * A reference argument is identified by a '&', and contains a
 * \c Node::lookupNode() compatible string.
 * 
 * Parsing starts on the '&' marker, and ends just after the end of the
 * reference string.
 */
void NepParser::genArgReference(Nepeta::Node& data)
{
	iterCur();
	size_t start = getCurPos();
	size_t stLine=getCurLine(), stCol=getCurCol();
	
	while( true ) {
		char ch = getCur();
		
		if( checkReference(ch) )
			iterCur();
		else {
			mRef.push_back( Reference(&data, getSubString(start,getCurPos()),
				stLine, stCol) );
			return;
		}
	}
}

/**
 * @brief Generates a block type argument and stores it in data.
 * 
 * Parsing starts on the opening '{', and ends just after the closing '}'.
 * Will decode the string if a valid decoding identifier is detected after the
 * '{' on the same line.
 * 
 * @see helpGenBuffer
 */
void NepParser::genArgBlock(Nepeta::Node &data)
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
	NepCodec *codec = &nepTextCodec;
	if( (end-start) > 0) {
		NepCodec::CodecPack::iterator i=mCodec.find(getSubString(start,end));
		if(i != mCodec.end())
			codec = i->second;
	}
	codec->decode(data,data.getArgCount(),arg);
}


// Contexts
//
//
/**
 * @brief Parses in the argument context for the given node.
 */
void NepParser::genCtxArg(Nepeta::Node &data)
{
	while( notEof() ) {
		char ch = getCurRaw();
		
		if( isSpace(ch) ) {
			iterCur();
		} else if( checkIdentifier(ch) ) {
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
			mScript.addError(Nepeta::ErrIllegalCharacter, std::string(1,ch),
				getCurLine(), getCurCol());
			helpSeekNewline(false);
		}
	}
}

/**
 * @brief Parses in the data context for the given node.
 * 
 * The data context first detects the node identifier and whether
 * the node has any nested nodes, then starts parsing the new node's
 * arguments.
 * 
 * If this node has nested nodes, then the parser enters the
 * block context for the node right after the argument context.
 * 
 * @return false if this node is a closing block marker.
 */
bool NepParser::genCtxData(Nepeta::Node &node)
{
	size_t stL=getCurLine(), stC=getCurCol();
	
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
			mScript.addError(Nepeta::ErrIllegalCharacter, 
				std::string(1,ch),getCurLine(),getCurCol()
			);
			helpSeekNewline(false);
			return true;
		}
	}
	
	// Build the identifier and create the data node
	size_t start, end;
	helpGenIdent(start,end);
	Nepeta::Node &data = node.createNode(
		getSubString(start,getCurPos()), Nepeta::NoPos, stL, stC );
	
	// Process the data context
	genCtxArg(data);
	if(isHash)
		genCtxBlock(data);
	
	return true;
}

/**
 * @brief Parses in the block context for the given node
 * 
 * The block context parses comments and data identifiers, any other
 * data is invalid in this context.
 * 
 * The block context ends when a closing hash marker is found.
 */
void NepParser::genCtxBlock(Nepeta::Node &node)
{
	size_t stLine = getCurLine(), stCol = getCurCol();
	
	while( notEof() ) {
		char ch = getCurRaw();
		
		// Skip whitespace
		if( isWhite(ch) ) {
			iterCur();
		// Check for data block marker
		} else if( checkIdentifier(ch) || checkHash(ch) ) {
			// Parse the data block.
			// If an end marker is found, this returns false
			if(!genCtxData(node))
				return;
		// Check for comment marker
		} else if( checkComment(ch) ) {
			helpSkipComment();
		// Any other character is invalid, skip to the next line
		} else {
			mScript.addError( Nepeta::ErrIllegalCharacter, std::string(1,ch),
				getCurLine(), getCurCol()
			);
			helpSeekNewline(false);
		}
	}
	
	if(node.getParent()) {
		mScript.addError( Nepeta::ErrPrematureEnd, "",
		stLine, stCol, getCurLine(), getCurCol());
	}
}

/**
 * @brief Iterates the parser position one forward.
 * 
 * The line and column counter is updated as well.
 */
void NepParser::iterCur()
{
	if(getCurRaw() == '\n') {
		mCol=0;
		mLine++;
	}
	
	mCol++;
	mCur++;
}

/**
 * @brief Initializes the parser with the given data.
 */
void NepParser::setupGenerator(const std::string& str, size_t stLine, size_t stCol)
{
	mCur = 0;
	mLine = stLine;
	mCol = stCol;
	mStr = &str;
}

NepParser::NepParser(Nepeta& scr) : 
	mCur(0), mLine(0), mCol(0), mStr(0), mCodec(),
	mScript(scr), mRef()
{
	mCodec["text"] = &nepTextCodec;
	mCodec["base64"] = &nepBase64Codec;
}

/**
 * @brief Runs the parser from the root node on the given string.
 */
void NepParser::parse(const std::string &str)
{
	// Prepare generation
	setupGenerator(str);
	
	genCtxBlock(mScript.getRoot());
}

/**
 * @brief Resolves all node references constructed by the parser.
 * 
 * If any invalid references was detected, ignore them and notify the error.
 */
void NepParser::resolveReferences()
{
	// Resolve all references
	for(size_t i=0; i<mRef.size(); ++i) {
		Reference &ref = mRef[i];
		
		// Find the actual node from the reference string
		Nepeta::Node &lookup = ref.mSrc->lookupNode(ref.mRef);
		
		if(lookup.isEmpty()) {
			mScript.addError(Nepeta::ErrInvalidRef, ref.mRef,
				ref.mLine, ref.mCol);
		} else {
			ref.mSrc->createReference(lookup);
			
			// Test for circular references
			size_t circRef =0;
			while( (circRef=ref.mSrc->getCircularReference()) != Nepeta::NoPos ) {
				mScript.addError(Nepeta::ErrCircularRef, ref.mRef,
					ref.mLine, ref.mCol);
				ref.mSrc->removeReference(circRef);
			}
		}
	}
	mRef.clear();
}
