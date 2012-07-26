#ifndef EXTSIM_TYPERULE_H
#define EXTSIM_TYPERULE_H

#include <string>

#include "nepeta.h"
#include "../../simulation/Common.h"

namespace ExtS {
	class ExtSim;
	class ParamList;
	
	class TypeRule {
		public:
			TypeRule(ExtSim &esim) : mExtSim(esim) {}
			
			virtual TypeRule *clone()=0;
			virtual void load(Nepeta::Node &node);
			
			virtual void makeInput(Sim::IdType typeId, const ParamList *param)=0;
			virtual void buildTimelineData(const ParamList *param)=0;
			virtual Sim::IdType registerSimData(const std::string &name)=0;
			
			virtual bool checkConstrained(const ParamList *param) const;
			ParamList *makeParam() const;
			
			const ParamList *getRefParam() const { return mRefParam; }
			
		protected:
			ExtSim &mExtSim;
			
			ParamList *mRefParam;
	};
}

#endif
