#ifndef EXTSIM_TYPERULE_H
#define EXTSIM_TYPERULE_H

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

#include "../dascript/script.h"
#include "../simulation/Save.h"

namespace ExtS {
	class MetaParam;
	class ExtSim;
	
	template<class Type>
	struct Listener {
		virtual void process(Type *src)=0;
	};
	
	/**
	 * Manages the global listener for a parameter or constraint.
	 * 
	 * Used to implement callbacks for \c Param or \c Constraint
	 * specializations.
	 */
	template<class Type>
	class ListenerSlot {
		private:
			typedef boost::shared_ptr<Listener<Type> > SharedPtr;
		
		public:
			template<class LT>
			static void setListener(const LT &L) {
				sListener = SharedPtr(new LT(L));
			}
			
			static void clearListener() {
				sListener.reset();
			}
			
			static void raiseListener(Type *src) {
				if(sListener.get())
					sListener->process(src);
			}
		
		private:
			static SharedPtr sListener;
	};
	
	template<class Type>
	typename ListenerSlot<Type>::SharedPtr ListenerSlot<Type>::sListener =
		typename ListenerSlot<Type>::SharedPtr();
	
	/**
	 * Contains parameter information such as the default value of a parameter
	 * and its constraints.
	 * 
	 * Any implementation of this class describe the details of the
	 * implementation.
	 */
	class RuleParameter {
		public:
			RuleParameter(const std::string &dataName) : mDataName(dataName),
				mIsConstraintDefined(false) {}
			virtual ~RuleParameter() {}
			
			virtual RuleParameter *clone()=0;
			
			virtual void readBlock(Script::Block *block)=0;
			virtual void postProcess(ExtSim &extsim) {}
			
			virtual void callback()=0;
			
			virtual bool isValid(RuleParameter *param, ExtSim &extsim) const=0;
			
			const std::string &getDataName() const { return mDataName; }
			bool isConstraintUndefined() const
			{ return !mIsConstraintDefined; }
			
		protected:
			Script::Data &getBlockData(Script::Block *block) const
			{ return block ?
				block->getDataSimple(getDataName()) :
				Script::Data::emptyData(); }
			
			void setDefinedConstraint() { mIsConstraintDefined=true; }
			
		private:
			std::string mDataName;
			
			bool mIsConstraintDefined;
	};
	
	/**
	 * Contains a list of parameter objects.
	 */
	class ParamList {
		public:
			typedef std::vector<RuleParameter*> RuleParamVec;
			
			ParamList() {}
			~ParamList() { clearRuleParam(); }
			
			ParamList(const ParamList &src);
			ParamList &operator=(const ParamList &src);
			
			void registerRuleParam(RuleParameter *mgr)
			{ mRuleParam.push_back(mgr); }
			void clearRuleParam();
			
			size_t size() const { return mRuleParam.size(); }
			
		protected:
			RuleParamVec mRuleParam;
	};
	
	/**
	 * @brief Manages the behaviour of a base type
	 * 
	 * A TypeRule contain a vector of \c MetaParam objects. Each of these
	 * objects contain information about the default paramter and parameter
	 * constraints.
	 * 
	 * \c TypeRule::makeParam is used to create a \c ParamList for parameters
	 * that the caller can manipulate. When the parameter package is ready
	 * it is fed back in order to create the object that depend on the
	 * parameters.
	 */
	class TypeRule : protected ParamList {
		public:
			typedef ParamList::RuleParamVec RuleParamVec;
			
			TypeRule();
			virtual ~TypeRule();
			
			virtual TypeRule *clone()=0;
			
			void readBlock(Script::Block *block);
			void postProcess(ExtSim &extsim);
			
			ParamList *makeParam() const;
			
			const RuleParamVec &getRuleParamVec() const { return mRuleParam; }
			
		private:
			
	};
	
	/**
	 * @brief Manages a mapping of \c TypeRule objects by a string identifier.
	 * 
	 * This mapping is used by \c loadRuleBlock() to find any matching
	 * "Base" type for inheritance.
	 */
	class TypeRuleMgr {
		public:
			TypeRuleMgr();
			~TypeRuleMgr();
			
			TypeRule *loadRuleBlock(Script::Block &block);
			
			void registerTypeRule(const std::string &name, TypeRule *rule)
			{ mRuleMap[name] = rule; }
			
		private:
			typedef boost::unordered_map<std::string,TypeRule*> RuleMap;
			RuleMap mRuleMap;
	};
}

#endif
