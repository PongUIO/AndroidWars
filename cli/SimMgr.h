#ifndef SIMMGR_H
#define SIMMGR_H

#include "../extsim/ExtSim.h"

class CmdManager;

class SimMgr
{
	public:
		SimMgr(CmdManager &tmgr);
		~SimMgr();
		
		exts::ExtSim &getExtSim(){ return mExtSim; }

	private:
		exts::ExtSim mExtSim;
		CmdManager &mgr;


};
#endif
