#ifndef EXTSIM_TYPECONV_H
#define EXTSIM_TYPECONV_H

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <stdio.h>

#include "../simulation/Vector.h"

namespace exts {
	namespace BadCastType {
		struct BctInteger {};
		struct BctFloat {};
		struct BctOther {};
	}
	
	template<class T, class V>
	struct BadCast {
		static T strategy(const std::string &str, T def) { return def; }
	};
	
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
		{
			if(boost::is_integral<T>::value == boost::true_type::value &&
			boost::is_same<T,bool>::value == boost::false_type::value)
				val=BadCast<T,BadCastType::BctInteger>::strategy(str,def);
			else if(boost::is_float<T>::value == boost::true_type::value)
				val=BadCast<T,BadCastType::BctFloat>::strategy(str,def);
			else
				val=BadCast<T,BadCastType::BctOther>::strategy(str,def);
		}
		
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
	
	template<>
	inline Sim::Vector convValue<Sim::Vector>(const std::string &str,
	Sim::Vector def) {
		std::vector<std::string> sep;
		boost::split(sep, str, boost::is_any_of(" ,"),
			boost::algorithm::token_compress_on);
		
		Sim::Vector val = def;
		if(sep.size() == 2) {
			val.x = convValue<double>(sep[0],def.x);
			val.y = convValue<double>(sep[1],def.y);
		}
		
		return val;
	}
	
	template<>
	struct BadCast<bool,BadCastType::BctOther> {
		static bool strategy(const std::string &str, bool def)
		{
			if(str=="false")
				return false;
			else if(str=="true")
				return true;
			else return def;
		}
	};
	
	template<class T>
	struct BadCast<T,BadCastType::BctFloat> {
		static T strategy(const std::string &str, T def)
		{
			if(str=="inf")
				return std::numeric_limits<T>::infinity();
			else if(str=="-inf")
				return -std::numeric_limits<T>::infinity();
			else return def;
		}
	};
	
	template<class T>
	struct BadCast<T,BadCastType::BctInteger> {
		static T strategy(const std::string &str, T def)
		{
			if(str=="max")
				return std::numeric_limits<T>::max();
			else if(str=="min")
				return std::numeric_limits<T>::min();
			else return def;
		}
	};
}

#endif
