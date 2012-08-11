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
			
			virtual void makeInput(const ParamList *param) const=0;
			virtual void buildTimelineData(const ParamList *param) const=0;
			virtual Sim::IdType registerSimData(const std::string &name) const=0;
			
			bool checkConstrained(const ParamList *param) const;
			ParamList *makeParam() const;
			
			const ParamList *getRefParam() const { return &mParam; }
			const Sim::IdType getId() const { return mId; }
			
			void setObjectId(Sim::IdType id) { mObjectId = id; }
			
		protected:
			void registerParam(RuleParameter *param);
			void setId(Sim::IdType id);
			
			ExtSim &mExtSim;
			
			ParamList mParam;
			Sim::IdType mId;
			Sim::IdType mObjectId;
			
			friend class TypeRuleMgr;
	};
}

#endif
