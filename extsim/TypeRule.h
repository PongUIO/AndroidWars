#ifndef EXTSIM_TYPERULE_H
#define EXTSIM_TYPERULE_H

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

#include "../dascript/script.h"
#include "../simulation/Save.h"
#include "../simulation/Common.h"

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
	class RuleParameter : public Sim::Save::OperatorImpl<RuleParameter> {
		public:
			RuleParameter(const std::string &dataName) : mDataName(dataName),
				mIsConstraintDefined(false) {}
			virtual ~RuleParameter() {}
			
			virtual RuleParameter *clone()=0;
			
			virtual void readBlock(Script::Block *block)=0;
			virtual void postProcess(ExtSim &extsim) {}
			
			virtual void callback()=0;
			
			virtual void save(Sim::Save::BasePtr &fp) const=0;
			virtual void load(Sim::Save::BasePtr &fp)=0;
			
			/**
			 * By using this object as a reference, test if \c param uphelds
			 * our constraints.
			 */
			virtual bool isValid(const RuleParameter *param,
				ExtSim &extsim) const=0;
			
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
	class ParamList : public Sim::Save::OperatorImpl<ParamList> {
		public:
			typedef std::vector<RuleParameter*> RuleParamVec;
			
			ParamList() {}
			~ParamList() { clearRuleParam(); }
			
			ParamList(const ParamList &src);
			ParamList &operator=(const ParamList &src);
			
			void registerRuleParam(RuleParameter *mgr)
			{ mRuleParam.push_back(mgr); }
			void clearRuleParam();
			
			template<class T>
			const T *getParam(size_t i) const
			{ return static_cast<T*>(mRuleParam[i]); }
			
			size_t size() const { return mRuleParam.size(); }
			
			const RuleParamVec &getRuleParamVec() const { return mRuleParam; }
			
			/**
			 * Saves a parameter list.
			 * 
			 * @warning This saves each rule directly without caring about
			 * size or contents. This means that the loaded list must contain
			 * the same elements.
			 */
			void save(Sim::Save::BasePtr &fp) const
			{
				for(RuleParamVec::const_iterator i=mRuleParam.begin();
					i!=mRuleParam.end(); ++i)
					fp << *(*i);
			}
			
			/**
			 * Loads a parameter list.
			 * 
			 * @warning It is required that the list that destination list
			 * already contain the exact same number of parameters and type
			 * for the parameters.
			 */
			void load(Sim::Save::BasePtr &fp)
			{
				for(RuleParamVec::iterator i=mRuleParam.begin();
					i!=mRuleParam.end(); ++i)
					fp >> *(*i);
			}
			
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
			
			/**
			 * Creates an input object for this typerule from a \c ParamList.
			 */
			virtual void makeInput(
				ExtSim &extsim, const ParamList *param) const=0;
			bool checkConstrained(ParamList *srcList, ExtSim &extsim) const;
			
			void readBlock(Script::Block *block);
			void postProcess(ExtSim &extsim);
			
			ParamList *makeParam() const;
			
		protected:
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
