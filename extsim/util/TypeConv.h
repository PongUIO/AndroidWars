#ifndef EXTSIM_TYPECONV_H
#define EXTSIM_TYPECONV_H

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>

#include "../../simulation/Vector.h"

namespace exts {
	template<class T>
	T badCastStrategy(const std::string &str, T def) { return def; }
	
	/**
	 * @brief Converts a string into a value of a specific type
	 */
	template<class T>
	T convValue(const std::string &str, T def=T())
	{
		T val = def;
		if(str.empty())
			return val;
		
		try { val = boost::lexical_cast<T>(str); }
		catch(boost::bad_lexical_cast &)
		{ val=badCastStrategy<T>(str,def); }
		
		return val;
	}
	
	/**
	 * @brief Converts a string of a binary number into a bitfield value.
	 * 
	 * The input string is parsed and stored based on the characters.
	 * Any "1" is a set bit, "0" is an unset bit, and any other character
	 * indicates the default value's bit should be used (most of the time
	 * this is an unset bit).
	 */
	template<class T>
	T convBitfield(const std::string &str, T def=T()) {
		T val = def;
		if(str.empty())
			return def;
		
		size_t bitIndex = str.size()-1;
		for(size_t i=0; i<str.size(); ++i) {
			char ch = str[i];
			if(ch == '1')
				val |= (1<<bitIndex);
			else if(ch == '0')
				val &= ~(1<<bitIndex);
			
			bitIndex--;
		}
		
		return val;
	}
	
#include <stdio.h>
	template<>
	inline Sim::Vector convValue<Sim::Vector>(const std::string &str,
	Sim::Vector def) {
		std::vector<std::string> sep;
		boost::split(sep, str, boost::is_any_of(" ,"),
			boost::algorithm::token_compress_on);
		
		Sim::Vector val = def;
		if(sep.size() == 2) {
			printf("convValue<Sim::Vector>('%s', '%s')\n", sep[0].c_str(), sep[1].c_str());
			val.x = convValue<double>(sep[0],def.x);
			val.y = convValue<double>(sep[1],def.y);
			printf("convValue<Sim::Vector>('%g', '%g')\n", val.x, val.y);
		}
		
		return val;
	}
	
	template<>
	inline bool badCastStrategy<bool>(const std::string &str, bool def)
	{
		if(str=="false")
			return false;
		else if(str=="true")
			return true;
		else return def;
	}
	
	template<>
	inline double badCastStrategy<double>(const std::string &str, double def)
	{
		if(str=="inf")
			return std::numeric_limits<double>::infinity();
		else if(str=="-inf")
			return -std::numeric_limits<double>::infinity();
		else return def;
	}
}

#endif
