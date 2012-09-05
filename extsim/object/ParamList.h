#ifndef EXTSIM_PARAMLIST_H
#define EXTSIM_PARAMLIST_H

#include "nepeta.h"
#include "../../simulation/Save.h"
#include "../../simulation/Common.h"

namespace exts {
	class ExtSim;
	class ParamVisitor;
	
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
			virtual void accept(ParamVisitor &visitor)=0;
			
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
			typedef std::vector<Sim::IdType> IdVec;
			
			ParamList(Sim::IdType refTypeRule=Sim::NoId)
				: mRefTypeRuleId(refTypeRule), mRefAgent(Sim::NoId) {}
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
			
			Sim::IdType getAgent() const { return mRefAgent; }
			void setAgent(Sim::IdType id) { mRefAgent = id; }
			
			Sim::IdType getAllocId(size_t i) const
			{	return i<mAllocId.size() ? mAllocId[i] : Sim::NoId; }
			
			bool isConstrained(ExtSim &ref) const;
			void traverseVisitor(ParamVisitor &visitor);
			
			void save(Sim::Save::BasePtr &fp) const;
			void load(Sim::Save::BasePtr &fp);
			
		private:
			void clearRuleParam();
			
			void addParam(RuleParameter *param)
			{ mRuleParam.push_back(param); }
			
			void allocateId(Sim::IdType id)
			{ mAllocId.push_back(id);}
			
			void setRefTypeRuleId(Sim::IdType id)
			{ mRefTypeRuleId = id; }
			
			RuleParamVec mRuleParam;
			IdVec mAllocId;
			Sim::IdType mRefTypeRuleId;
			Sim::IdType mRefAgent;
			
			friend class TypeRule;
	};
}

#endif
