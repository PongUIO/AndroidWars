#ifndef EXTSIM_BOT_DEFAULTBOT_H
#define EXTSIM_BOT_DEFAULTBOT_H

#include "../../TypeRule.h"

namespace ExtS { namespace Bot {
	class DefaultBot : public TypeRule {
		public:
			DefaultBot() {}
			virtual ~DefaultBot() {}
			
			virtual TypeRule* clone()
			{ return new DefaultBot(*this); }
			
			virtual Sim::IdType registerSimData(ExtSim& esim,
				const std::string& name);
			
			virtual void makeInput(ExtSim& extsim, Sim::IdType typeId,
				const ExtS::ParamList* param) const {}
	};
} }

#endif
