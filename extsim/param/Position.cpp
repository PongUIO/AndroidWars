#include "Position.h"
#include "ValRange.h"

namespace ExtS {
	// Parameter
	//
	//
	void PositionP::readParam(Script::Data& data)
	{
		mPos = ExtData::readVector(data.getArg(0));
	}
	
	// Constraint
	//
	//
	void PositionC::readConstraint(Script::Data& data)
	{
		mMin = ExtData::readVector(data.getArg(0), -Sim::Vector::infinity());
		mMax = ExtData::readVector(data.getArg(1), Sim::Vector::infinity());
	}
	
	bool PositionC::isValid(Param* param, ExtSim& extsim) const
	{
		PositionP *paramPos = static_cast<PositionP*>(param);
		
		Sim::Vector pos = paramPos->getPos();
		return pos >= mMin && pos <= mMax;
	}

}
