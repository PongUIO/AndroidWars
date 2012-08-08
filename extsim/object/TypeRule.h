#ifndef EXTSIM_TYPERULE_H
#define EXTSIM_TYPERULE_H

#include <string>

#include "nepeta.h"
#include "../../simulation/Common.h"

namespace exts {
	class ExtSim;
	class ParamList;
	
	class TypeRule {
		public:
			TypeRule(ExtSim &esim) : mExtSim(esim), mId(Sim::NoId) {}
			
			virtual TypeRule *clone()=0;
			virtual void load(Nepeta::Node &node)=0;
			
			virtual void makeInput(const ParamList *param) const=0;
			virtual void buildTimelineData(const ParamList *param) const=0;
			virtual Sim::IdType registerSimData(const std::string &name) const=0;
			
			bool checkConstrained(const ParamList *param) const;
			ParamList *makeParam() const;
			
			const ParamList *getRefParam() const { return mRefParam; }
			const Sim::IdType getId() const { return mId; }
			
		protected:
			ExtSim &mExtSim;
			
			ParamList *mRefParam;
			Sim::IdType mId;
			
			friend class TypeRuleMgr;
	};
}

#endif
