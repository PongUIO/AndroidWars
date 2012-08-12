#ifndef SIMMGR_H
#define SIMMGR_H

#include "../extsim/ExtSim.h"

class SimMgr
{
	public:
		SimMgr();
		~SimMgr();
		
		ExtSim getExtSim();

	private:
		ExtSim &mExtSim;

	
};
#endif
