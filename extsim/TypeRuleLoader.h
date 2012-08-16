#ifndef EXTSIM_TYPERULELOADER_H
#define EXTSIM_TYPERULELOADER_H

#include <string>
#include <boost/unordered_map.hpp>

#include "nepeta.h"

namespace exts {
	class TypeRule;
	class ExtSim;
	
	/**
	 * @brief Loads \c TypeRule objects from nepeta nodes based on an internal
	 * reference table.
	 * 
	 * This class stores a name-associated set of reference \c TypeRule
	 * objects. When a \c Nepeta::Node is loaded, this reference set is
	 * checked for any suitable match and the \c TypeRule is cloned.
	 * 
	 * This class take ownership of any \c TypeRule added to the reference set.
	 */
	class TypeRuleLoader {
		public:
			TypeRuleLoader(ExtSim &esim);
			~TypeRuleLoader();
			
			TypeRule *loadRuleNode(const Nepeta::Node &node);
			void registerRule(const std::string &name, TypeRule *rule);
			
			void clear();
			
		private:
			ExtSim &mExtSim;
			
			typedef boost::unordered_map<std::string,TypeRule*> ReferenceMap;
			ReferenceMap mReferences;
			
	};
}

#endif
