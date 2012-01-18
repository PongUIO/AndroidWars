#include <stdio.h>
#include <sstream>
#include <iostream>

#include "script.h"
#include "token.h"

std::string Script::Data::sEmpty = "";

void Script::compile(const std::string &str)
{
	TokenData tokenizer = TokenData(*this);
	
	tokenizer.generate(str);
}

Script::Block::~Block()
{
	for(BlockMap::iterator i=mBlock.begin(); i!=mBlock.end(); i++) {
		delete i->second;
	}
}

std::string Script::errorStr(const Script::Error& err)
{
	std::ostringstream res;
	
	switch(err.type)
	{
		case ErrNoCloseString:
			res << "(error) String not closed, starting at L:" <<
				err.stLine << ", C:" << err.stCol;
			break;
			
		case ErrBlockNoClosing:
			res << "(error) Block not closed, starting at L:" <<
				err.stLine << ", C:" << err.stCol;
			break;
			
		case ErrIllegalCharacter:
			res << "(error) Illegal character occured at L:" <<
				err.stLine << ", C:" << err.stCol;
			break;
			
		case ErrPrematureEnd:
			res << "(error) Script came to an abrupt end starting L:"<<
				err.stLine << ", C:" << err.stCol;
			break;
			
		case WarnReqNewline:
			res << "(warning) New line expected at L:"<<err.stLine <<
				", C:" << err.stCol << ". Extraneous data ignored";
			break;
	}
	
	return res.str();
}
