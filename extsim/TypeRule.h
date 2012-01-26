#ifndef EXTSIM_TYPERULE_H
#define EXTSIM_TYPERULE_H

#include <boost/unordered_map.hpp>

#include "../dascript/script.h"
#include "../simulation/Save.h"

namespace ExtS {
	/**
	 * Contains a list of parameters
	 */
	class TypeParam {
		public:
			TypeParam();
			~TypeParam();
			
			/**
			 * A single parameter object.
			 */
			struct Param {
				Param() {}
				virtual ~Param() {}
				
				virtual void save(Sim::Save::BasePtr& fp)=0;
			};
			
			template<class T>
			void insertParam(const typename T::Config &cfg=T::Config()) {
				mParam.push_back(new T(cfg));
			}
			
			void save(Sim::Save::BasePtr &fp);
			
		private:
			typedef std::vector<Param*> ParamVec;
			ParamVec mParam;
	};
	
	/**
	 * @brief Manages the behaviour of a base type
	 * 
	 * Script-defined objects that specify an inheritance through the
	 * "Base" parameter in reality select a \c TypeRule to use.
	 * 
	 * This links an extended implementation to the internal simulation-type,
	 * providing extended information to simplify the creation of input and
	 * standard abilitiy types.
	 * 
	 * \c TypeRule converts parameters into simulation input.
	 * 
	 * Specialization of generic base types are done through the use of a
	 * "#PARAM" block, and the arguments to this block are predefined and
	 * limited by type and range (for example, one may have a type for armor
	 * types, or another for integers [which may further be limited in a numeric
	 * range]). If a parameter is not specified in #PARAM, it is assumed to
	 * be customizeable, as such can be altered externally (for example through
	 * a GUI).
	 * 
	 * An example of such a rule is the MoveTowards program. The \c TypeRule
	 * would define a parameter "Position", which takes a value of type
	 * WorldPosition. The GUI could interpret this as having to right click
	 * on a position in the world.
	 * 
	 * Other parameter types could be, for example, TargetBot, or TargetObject,
	 * SelectWeapon, TargetProgram, TargetProgramClass.
	 * 
	 * Conditional parameters should also be possible. For
	 * example allowing the selection of either a WorldPosition or TargetBot.
	 */
	class TypeRule {
		public:
			TypeRule();
			virtual ~TypeRule();
			
			virtual void readParam();
			
		private:
	};
	
	class TypeRuleMgr {
		public:
			TypeRuleMgr();
			~TypeRuleMgr();
			
			struct RuleData {
				TypeRule *mRule;
				TypeParam mParam;
			};
			
			RuleData loadBlock(Script::Block &block);
			
		private:
			typedef boost::unordered_map<std::string,TypeRule*> RuleMap;
			RuleMap mRuleMap;
	};
}

#endif
