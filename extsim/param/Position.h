#ifndef EXTSIM_POSITION_H
#define EXTSIM_POSITION_H

#include "../../simulation/Vector.h"

#include "../TypeRule.h"
#include "../ExtData.h"

namespace ExtS {
	class Position : public ParamList::Param {
		public:
			Position();
			virtual ~Position();
			
			void loadBlock(Script::Data& data) {
				mPos.x = ExtData::readValue<double>(data.getArg(0));
				mPos.y = ExtData::readValue<double>(data.getArg(1));
			}
			void postProcess() {}
			
			void save(Sim::Save::BasePtr& fp) const
			{ fp << mPos; }
			
			void fill();
			
		private:
			Sim::Vector mPos;
	};
}

#endif
