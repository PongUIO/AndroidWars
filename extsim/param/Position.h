#ifndef EXTSIM_POSITION_H
#define EXTSIM_POSITION_H

#include "../../simulation/Vector.h"

#include "../TypeRule.h"
#include "../ExtData.h"

namespace ExtS {
	class PositionC;
	
	class PositionP : public Param, public ListenerSlot<PositionP> {
		public:
			PositionP(MetaParam* parent) : Param(parent) {}
			
			void readParam(Script::Data& data);
			void callback()
			{ ListenerSlot<PositionP>::raiseListener(this); }
			
			Sim::Vector getPos() const { return mPos; }
			
		private:
			Sim::Vector mPos;
	};
	
	class PositionC : public Constraint, public ListenerSlot<PositionC> {
		public:
			PositionC(MetaParam* parent) : Constraint(parent),
				mMin(-Sim::Vector::infinity()),
				mMax(Sim::Vector::infinity()) {}
			
			void readConstraint(Script::Data& data);
			bool isValid(Param* param, ExtSim& extsim) const;
			
			void callback()
			{ ListenerSlot<PositionC>::raiseListener(this); }
			
		private:
			Sim::Vector mMin, mMax;
	};
	
	class MetaPosition : public MetaParam {
		public:
			MetaPosition(const std::string& dataName) : MetaParam(dataName),
				mDefault(this), mConstraint(this) {}
			virtual ~MetaPosition() {}
			
			Param* constructParam() { return new PositionP(mDefault); }
			MetaParam* clone() { return new MetaPosition(*this); }
			
			void readParam(Script::Data& data)
			{ mDefault.readParam(data); }
			void readConstraint(Script::Data& data)
			{ mConstraint.readConstraint(data); }
			
			Param* getDefaultParam() { return &mDefault; }
			Constraint* getDefaultConstraint() { return &mConstraint; }
			
		private:
			PositionP mDefault;
			PositionC mConstraint;
	};
}

#endif
