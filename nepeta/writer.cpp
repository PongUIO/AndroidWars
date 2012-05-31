/**
 * This file is part of the Nepeta project.
 * 
 * Licensed under GNU LGPL (see license.txt and README)
 */
#include <sstream>
#include "nepeta.h"
#include "nepparser.h"
#include "nepcodec.h"

/**
 * @brief Generates an error string for the given error.
 */
std::string Nepeta::errorStr(const Nepeta::Error& err)
{
	std::ostringstream res;
	
	res << ( (err.type < WarningStart) ? "(error) " : "(warning) ");
	switch(err.type)
	{
		case ErrIllegalCharacter: {
			res << "Illegal character ";
			
			if(!err.msg.empty()) {
				char ch = err.msg[0];
				if(isprint(ch) || (ch & (1<<8)))
					res << "'" << ch << "'";
				else
					res << "(" << int(ch) << ")";
			}
			
			res << " occured at L:" << err.stLine << ", C:" << err.stCol;
			break;
		}
		
		case ErrPrematureEnd: {
			res << "Nested node came to an abrupt end starting L:"<<
				err.stLine << ", C:" << err.stCol;
			break;
		}
		
		case ErrCommentNotClosed: {
			res << "Comment was never closed, starting at L:"<<
				err.stLine << ", C:" << err.stCol;
			break;
		}
		
		case ErrNoCloseString: {
			res << "String not closed, starting at L:" <<
				err.stLine << ", C:" << err.stCol;
			break;
		}
		
		case ErrBlockNoClosing: {
			res << "Block not closed, starting at L:" <<
				err.stLine << ", C:" << err.stCol;
			break;
		}
		
		case ErrCircularRef: {
			res << "Circular reference detected in reference '" << err.msg <<
				"' at L:"<<err.stLine <<", C:" << err.stCol <<
				". Silently corrected.";
			break;
		}
		
		case ErrInvalidRef: {
			res << "Invalid reference '" << err.msg << "' at L:"<<err.stLine <<
				", C:" << err.stCol;
			break;
		}
		
		case WarnReqNewline: {
			res << "New line expected at L:"<<err.stLine <<
				", C:" << err.stCol << ". Extraneous data ignored.";
			break;
		}
	}
	
	return res.str();
}

/**
 * @brief Generates an error string for all error messages from compilation.
 */
std::string Nepeta::fullErrorStr()
{
	std::stringstream res;
	
	for(ErrorVec::iterator i=mErrors.begin(); i!=mErrors.end(); ++i) {
		res << errorStr(*i) << std::endl;
	}
	
	return res.str();
}

/**
 * @brief Creates a string from the contained node tree.
 * The resulting string may be parsed by the \c NepParser and
 * replicate the data.
 */
std::string Nepeta::makeString(bool useRelRef)
{
	std::ostringstream res;
	getRoot().makeString(res,useRelRef,0);
	
	return res.str();
}

enum ArgType {
	AtWord,
	AtString,
	AtText,
	AtBinary
};
/**
 * Analyzes the argument to determine its type according to the following
 * ruleset:
 * - \c AtWord : A word argument has no spaces and is not empty
 * - \c AtString : A string argument has no newlines.
 * - \c AtText : A text argument consists of printable characters with newlines
 * - \c AtBinary : If any non-printable characters exist, the text is binary.
 */
static ArgType checkArgType(const std::string &arg)
{
	if(arg.size() == 0)
		return AtString;
	
	// Assume it is a word
	ArgType type = AtWord;
	
	for(size_t i=0; i<arg.size() && i<1024; ++i) {
		char ch = arg[i];
		if( NepParser::isNewline(ch) )
			type = AtText;
		else if( type==AtWord && !NepParser::checkIdentifier(ch) )
			type = AtString;
		else if( (ch>=0 && ch<=8) || (ch>=11 && ch <=31 && ch!=13) ||
			ch==127) {
			return AtBinary;
		}
	}
	
	return type;
}

#define IND std::string(ind,'\t')

/**
 * @brief Writes an argument based on the format of the string.
 * 
 * This function tries to auto-detect the format of the string, and
 * writes it using a suitable argument format. Type analysis is done
 * only for a limited length of the argument.
 * 
 * @warning This function is not yet guaranteed to write all arguments
 * correctly. However it is only very rarely that an argument string should
 * fail.
 */
void Nepeta::Node::makeArgString(std::ostringstream& str,
	const std::string& arg, int ind)
{
	switch( checkArgType(arg) ) {
		case AtWord: {
			str << " " << arg;
			break;
		}
		
		case AtString: {
			str << " \"";
			for(size_t i=0; i<arg.size(); ++i) {
				char ch = arg[i];
				if(ch == '\"' || ch == '\\')
					str << '\\';
				str << ch;
			}
			str << "\"";
			break;
		}
		
		case AtText: {
			str << " {\n";
			bool atNewline = true;
			for(size_t i=0; i<arg.size(); ++i) {
				char ch = arg[i];
				
				if(atNewline)
					str << IND;
				str << ch;
				atNewline = (ch == '\n');
			}
			str << "\n" << std::string(ind-1,'\t') << "}";
			break;
		}
		
		case AtBinary: {
			NepBase64 base64;
			std::string out = arg;
			
			str << " { base64";
			base64.encode(str,ind,out);
			str << "\n" << std::string(ind-1,'\t') << "}";
			break;
		}
		
	}
}

/**
 * @brief Writes the node and all its nested data to an output stream.
 * 
 * @param useRelRef If true, references are written relative to the node.
 * @param ind The indentation level to write the node at.
 */
void Nepeta::Node::makeString(std::ostringstream& str, bool useRelRef, int ind)
{
	if(getId().size()>0) {
		str << IND << (getNodeCount()>0?"#":"") << getId();
		ind++;
	}
	
	// Write references
	for(size_t i=0; i<getReferenceCount(); i++) {
		str << " &" << getReference(i).buildLookupString(
			useRelRef ? this : 0);
	}
	
	// Write arguments
	if(getId().size() > 0) {
		for(size_t i=0; i<getArgCount(); ++i) {
			const std::string &ref = getArg(i);
			makeArgString(str, ref, ind);
		}
		str << std::endl;
	}
	
	// Write sub-nodes
	size_t nodeCount = getNodeCount(false);
	for(size_t i=0; i<nodeCount; i++) {
		getNode(i,false).makeString(str, useRelRef, ind);
	}
	
	if(getId().size()>0 && getNodeCount()>0) {
		ind--;
		str << IND << "#" << std::endl;
	}
}
