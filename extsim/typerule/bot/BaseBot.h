#ifndef EXTSIM_BASEBOT_H
#define EXTSIM_BASEBOT_H

#include "../../object/TypeRule.h"

namespace exts { namespace bot {
	class BaseBot : public TypeRule {
		public:
			enum ParamIndex {
				PiPosition = 0,
				
				PiMax
			};
			
			BaseBot(ExtSim& esim);
			virtual ~BaseBot();
			
			virtual TypeRule* clone() { return new BaseBot(*this); }
			
			virtual void registerInput(ParamList* param) const;
			virtual void makeInput(const ParamList* param) const;
			virtual void buildTimelineData(const ParamList* param) const {}
			virtual Sim::IdType registerSimData(const std::string& name) const;
			
		private:
	};
} }

#endif
