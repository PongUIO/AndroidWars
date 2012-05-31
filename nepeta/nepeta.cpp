/**
 * This file is part of the Nepeta project.
 * 
 * Licensed under GNU LGPL (see license.txt and README)
 */
#include "nepeta.h"
#include "nepparser.h"

/**
 * @brief Compiles a nepeta formatted string using default parsing.
 * 
 * For most uses, this is the only function used to parse the nepeta format.
 */
void Nepeta::compile(const std::string &str)
{
	NepParser parser = NepParser(*this);
	
	parser.parse(str);
	parser.resolveReferences();
}
