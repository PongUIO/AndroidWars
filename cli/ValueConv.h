#ifndef VALUECONV_H
#define VALUECONV_H

#include <string>
#include <boost/lexical_cast.hpp>

template <typename T>
T convValue(const std::string &str, T def = T())
{
	try 
	{
		return boost::lexical_cast<T>(str);
	}
	
	catch (boost::bad_lexical_cast &)
	{
		return def;
	}
	
}

#endif
