#ifndef EXTSIM_TYPERULE_H
#define EXTSIM_TYPERULE_H

#include <boost/unordered_map.hpp>

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
		public:
			template<class LT>
			static void setListener(const LT &L) {
				clearListener();
				sListener = new LT(L);
			}
			
			static void clearListener() {
				if(sListener)
					delete sListener;
				sListener=0;
			}
			
			static void raiseListener(Type *src) {
				if(sListener)
					sListener->process(src);
			}
		
		private:
			static Listener<Type> *sListener;
	};
	
	template<class Type>
	Listener<Type> *ListenerSlot<Type>::sListener = 0;
	
	/**
	 * A single parameter object.
	 * 
	 * Each parameter type must also have an associated \c MetaParam
	 * implementation.
	 */
	class Param {
		public:
			Param(MetaParam *parent) : mParent(parent) {}
			virtual ~Param() {}
			
			virtual void readParam(Script::Data &data)=0;
			
			virtual void callback()=0;
			
		protected:
			MetaParam *mParent;
	};
	
	/**
	 * Implements parameter constraints
	 */
	class Constraint {
		public:
			Constraint(MetaParam *parent) :
				mParent(parent), mIsUndefined(false) {}
			
			virtual void readConstraint(Script::Data &data)=0;
			virtual bool isValid(Param *param, ExtSim &extsim) const=0;
			
			virtual void callback()=0;
			
			bool isUndefined() const { return mIsUndefined; }
			
		protected:
			MetaParam *mParent;
			bool mIsUndefined;
			
			friend class MetaParam;
	};
	
	/**
	 * Contains static information about a parameter type, including
	 * parameter constraints.
	 */
	class MetaParam {
		public:
			MetaParam(const std::string &dataName) : mDataName(dataName) {}
			virtual ~MetaParam() {}
			
			/**
			 * Constructs a default parameter object of the type this
			 * \c MetaParam object implements.
			 */
			virtual Param *constructParam()=0;
			
			virtual MetaParam *clone()=0;
			
			virtual void readParam(Script::Data &data)=0;
			virtual void readConstraint(Script::Data &data)=0;
			
			virtual void postProcess(ExtSim &extsim) {}
			
			virtual Param *getDefaultParam()=0;
			virtual Constraint *getDefaultConstraint()=0;
			
			const std::string &getDataName() const { return mDataName; }
			
		private:
			std::string mDataName;
	};
	
	/**
	 * Contains a list of parameter objects.
	 */
	class ParamList {
		public:
			typedef std::vector<Param*> ParamVec;
			
			ParamList() {}
			~ParamList() {
				for(ParamVec::iterator i=mParam.begin(); i!=mParam.end();
					++i)
					delete *i;
			}
			
			void insertParam(Param *p) { mParam.push_back(p); }
			
			size_t size() const { return mParam.size(); }
			
		private:
			ParamVec mParam;
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
	class TypeRule {
		public:
			typedef std::vector<MetaParam*> MetaParamVec;
			
			TypeRule();
			virtual ~TypeRule();
			
			TypeRule *clone();
			
			ParamList *makeParam() const;
			
			void readParam(Script::Block &paramBlock);
			void readConstraint(Script::Block &constraintBlock);
			
			void postProcess(ExtSim &extsim);
			
			const MetaParamVec &getMetaParamVec() const { return mMetaParam; }
			
			/**
			 * Must be implemented in order to convert a parameter list into
			 * a
			 */
			virtual void saveInput(ParamList *, Sim::Save::BasePtr &,
				ExtSim &esim) const=0;
			
		protected:
			void registerMetaParam(MetaParam *mgr)
			{ mMetaParam.push_back(mgr); }
			
			
		private:
			MetaParamVec mMetaParam;
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
