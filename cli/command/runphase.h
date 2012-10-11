#ifndef RUNPHASE_H
#define RUNPHASE_H

#include "../Command.h"

class SimMgr;

class runphase : public Command 
{
	public:
		runphase(SimMgr &tmgr);
		~runphase();
		
		void run(const Nepeta::Node& node);
		
	private:
		SimMgr &mgr;
};

#endif
