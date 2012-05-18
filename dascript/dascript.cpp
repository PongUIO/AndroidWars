#include "dascript.h"
#include "daparser.h"

void DaScript::compile(const std::string &str)
{
	DaParser parser = DaParser(*this);
	
	parser.parse(str);
	parser.resolveReferences();
}
