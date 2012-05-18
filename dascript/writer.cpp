#include <sstream>
#include "dascript.h"
#include "daparser.h"
#include "dacodec.h"

std::string DaScript::errorStr(const DaScript::Error& err)
{
	std::ostringstream res;
	
	res << ( (err.type < WarningStart) ? "(error) " : "(warning) ");
	switch(err.type)
	{
		case ErrNoCloseString:
			res << "String not closed, starting at L:" <<
				err.stLine << ", C:" << err.stCol;
			break;
			
		case ErrBlockNoClosing:
			res << "Block not closed, starting at L:" <<
				err.stLine << ", C:" << err.stCol;
			break;
			
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
			
		case ErrPrematureEnd:
			res << "Script came to an abrupt end starting L:"<<
				err.stLine << ", C:" << err.stCol;
			break;
		
		case ErrCircularRef:
			res << "Circular reference detected in reference '" << err.msg <<
				"' at L:"<<err.stLine <<", C:" << err.stCol <<
				". Silently corrected.";
			break;
		
		case ErrInvalidRef:
			res << "Invalid reference '" << err.msg << "' at L:"<<err.stLine <<
				", C:" << err.stCol;
			break;
		
		case WarnReqNewline:
			res << "New line expected at L:"<<err.stLine <<
				", C:" << err.stCol << ". Extraneous data ignored.";
			break;
	}
	
	return res.str();
}

/**
 * Creates a complete string of all error messages that occured during
 * compilation.
 */
std::string DaScript::fullErrorStr()
{
	std::stringstream res;
	
	for(ErrorVec::iterator i=mErrors.begin(); i!=mErrors.end(); ++i) {
		res << errorStr(*i) << std::endl;
	}
	
	return res.str();
}

/**
 * Creates a string from the contained data. The resulting string may
 * be parsed by the \c DaParser and replicate the data.
 */
std::string DaScript::makeString(bool useRelRef)
{
	std::stringstream res;
	makeNodeString(res,getRoot(),useRelRef,0);
	
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
		if( DaParser::isNewline(ch) )
			type = AtText;
		else if( type==AtWord && !DaParser::checkWord(ch) )
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
 * Writes an argument based on the format of the string
 */
void DaScript::makeArg(std::stringstream& str, const std::string& arg, int ind)
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
			DaBase64 base64;
			std::string out = arg;
			base64.encode(out);
			
			str << " { base64";
			for(size_t i=0; i<out.size(); ++i) {
				if( (i % (70-ind*6)) == 0 )
					str << "\n" << IND;
				str << out[i];
			}
			str << "\n" << std::string(ind-1,'\t') << "}";
			break;
		}
		
	}
}

void DaScript::makeNodeString(std::stringstream& str, Node &node,
	bool useRelRef, int ind)
{
	if(node.getId().size()>0) {
		str << IND << (node.getNodeCount()>0?"#":"") << node.getId();
		ind++;
	}
	
	// Write references
	for(size_t i=0; i<node.getReferenceCount(); i++) {
		str << " &" << node.getReference(i).buildLookupString(
			useRelRef ? &node : 0);
	}
	
	// Write arguments
	if(node.getId().size() > 0) {
		for(size_t i=0; i<node.getArgCount(); ++i) {
			const std::string &ref = node.getArg(i);
			makeArg(str, ref, ind);
		}
		str << std::endl;
	}
	
	// Write sub-nodes
	size_t nodeCount = node.getNodeCount(false);
	for(size_t i=0; i<nodeCount; i++) {
		makeNodeString(str, node.getNode(i,false), useRelRef, ind);
	}
	
	if(node.getId().size()>0 && node.getNodeCount()>0) {
		ind--;
		str << IND << "#" << std::endl;
	}
}
