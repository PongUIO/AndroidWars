#ifndef EXTSIM_PARAMLIST_H
#define EXTSIM_PARAMLIST_H

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

#include "nepeta.h"
#include "../../simulation/Save.h"
#include "../../simulation/Common.h"

namespace ExtS {
	class ExtSim;
	
	template<class Type>
	struct Listener {
		virtual void process(Type *src)=0;
	};
	
	/**
	 * @brief Manages the global listener for a parameter and constraint.
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
	 * @brief Interface for a generic constrained parameter of any data type.
	 * 
	 * Contains parameter information such as the default value of a parameter
	 * and its constraints.
	 * 
	 * Any implementation of this class describe the details of the
	 * implementation.
	 */
	class RuleParameter : public Sim::Save::OperatorImpl<RuleParameter> {
		public:
			RuleParameter(const std::string &dataName) :
				mDataName(dataName), mIsConstraintDefined(false) {}
			virtual ~RuleParameter() {}
			
			virtual RuleParameter *clone()=0;
			virtual void callback()=0;
			
			virtual void readNode(Nepeta::Node &node)=0;
			virtual void postProcess(ExtSim &) {}
			
			virtual void save(Sim::Save::BasePtr &fp) const=0;
			virtual void load(Sim::Save::BasePtr &fp)=0;
			
			virtual bool isConstrained(const RuleParameter *, ExtSim&) const=0;
			
			const std::string &getDataName() const { return mDataName; }
			bool isConstraintDefined() const { return mIsConstraintDefined; }
			
		protected:
			Nepeta::Node &getNodeData(Nepeta::Node &node) const
			{ return node.getNodeSimple(getDataName()); }
			
			void setConstraintDefined() { mIsConstraintDefined=true; }
			
		private:
			std::string mDataName;
			bool mIsConstraintDefined;
	};
	
	/**
	 * @brief Contains a list of \c RuleParameter objects.
	 */
	class ParamList : public Sim::Save::OperatorImpl<ParamList> {
		public:
			typedef std::vector<RuleParameter*> RuleParamVec;
			
			ParamList() {}
			~ParamList() { clearRuleParam(); }
			
			ParamList &operator=(const ParamList &src);
			
			template<class T> T *getParamT(size_t i)
			{ return static_cast<T*>(mRuleParam[i]); }
			template<class T> const T *getParamT(size_t i) const
			{ return static_cast<T*>(mRuleParam[i]); }
			
			RuleParameter *getParam(size_t i) { return mRuleParam[i]; }
			const RuleParameter *getParam(size_t i) const
			{ return mRuleParam[i]; }
			
			size_t size() const { return mRuleParam.size(); }
			
			void save(Sim::Save::BasePtr &fp) const;
			void load(Sim::Save::BasePtr &fp);
			
		protected:
			void clearRuleParam();
			
			RuleParamVec mRuleParam;
	};
}

#endif
