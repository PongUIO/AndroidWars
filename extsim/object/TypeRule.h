#ifndef EXTSIM_TYPERULE_H
#define EXTSIM_TYPERULE_H

#include <string>

#include "nepeta.h"
#include "../../simulation/Common.h"
#include "ParamList.h"

namespace exts {
	class ExtSim;
	class RuleParameter;
	
	class TypeRule {
		public:
			TypeRule(ExtSim &esim) : mExtSim(esim),
				mId(Sim::NoId),
				mObjectId(Sim::NoId)
			{}
			
			virtual TypeRule *clone()=0;
			void load(const Nepeta::Node &node);
			
			virtual void registerInput(ParamList *param) const {}
			virtual void makeInput(const ParamList *param) const=0;
			virtual void buildTimelineData(const ParamList *param) const=0;
			virtual Sim::IdType registerSimData(const std::string &name) const=0;
			
			bool checkConstrained(const ParamList *param) const;
			ParamList *makeParam(Sim::IdType agentId=Sim::NoId) const;
			
			const ParamList *getRefParam() const { return &mParam; }
			const Sim::IdType getId() const { return mId; }
			
			void setObjectId(Sim::IdType id) { mObjectId = id; }
			
		protected:
			void allocateParam(size_t count);
			void setParam(size_t index, RuleParameter *param);
			
			void registerParam(RuleParameter *param);
			void allocateId(ParamList *param, size_t count) const;
			void setId(Sim::IdType id);
			
			ExtSim &mExtSim;
			
			ParamList mParam;
			Sim::IdType mId;
			Sim::IdType mObjectId;
			
			friend class TypeRuleMgr;
	};
	
/// Simple shortcut to define local variables for parameters for \c TypeRule objects
#define _EXTS_PARAM(type, name, index) \
	const type *name = param->getParamT< type >(index);
}

#endif
