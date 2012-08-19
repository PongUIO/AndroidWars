#ifndef EXTSIM_PARAMLIST_H
#define EXTSIM_PARAMLIST_H

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

#include "nepeta.h"
#include "../../simulation/Save.h"
#include "../../simulation/Common.h"

namespace exts {
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
			virtual void clearListener()=0;
			
			virtual void readNode(
				const Nepeta::Node &param,
				const Nepeta::Node &constraint
			)=0;
			virtual void postProcess(ExtSim &) {}
			
			virtual void save(Sim::Save::BasePtr &fp) const=0;
			virtual void load(Sim::Save::BasePtr &fp)=0;
			
			virtual bool isConstrained(const RuleParameter *, ExtSim&) const=0;
			
			const std::string &getDataName() const { return mDataName; }
			bool isConstraintDefined() const { return mIsConstraintDefined; }
			
		protected:
			const Nepeta::Node &getNodeData(const Nepeta::Node &node) const
			{ return node.getNode(getDataName()); }
			
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
			
			ParamList(Sim::IdType refTypeRule=Sim::NoId)
				: mRefTypeRuleId(refTypeRule) {}
			~ParamList() { clearRuleParam(); }
			ParamList(const ParamList &other);
			
			ParamList &operator=(const ParamList &src);
			
			template<class T> T *getParamT(size_t i)
			{ return static_cast<T*>(mRuleParam[i]); }
			template<class T> const T *getParamT(size_t i) const
			{ return static_cast<T*>(mRuleParam[i]); }
			
			RuleParameter *getParam(size_t i) { return mRuleParam[i]; }
			const RuleParameter *getParam(size_t i) const
			{ return mRuleParam[i]; }
			
			const RuleParamVec &getParamVec() const { return mRuleParam; }
			RuleParamVec &getParamVec() { return mRuleParam; }
			
			Sim::IdType getTypeRuleId() const { return mRefTypeRuleId; }
			size_t size() const { return mRuleParam.size(); }
			
			bool isConstrained(ExtSim &ref) const;
			void traverseCallback();
			void clearListeners() const;
			
			void save(Sim::Save::BasePtr &fp) const;
			void load(Sim::Save::BasePtr &fp);
			
		private:
			void clearRuleParam();
			
			void addParam(RuleParameter *param)
			{ mRuleParam.push_back(param); }
			
			void setRefTypeRuleId(Sim::IdType id)
			{ mRefTypeRuleId = id; }
			
			RuleParamVec mRuleParam;
			Sim::IdType mRefTypeRuleId;
			
			friend class TypeRule;
	};
}

#endif
