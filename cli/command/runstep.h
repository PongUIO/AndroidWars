#ifndef RUNSTEP_H
#define RUNSTEP_H

#include "../Command.h"

class SimMgr;

class runstep : public Command 
{
	public:
		runstep(SimMgr &tmgr);
		~runstep();
		
		void run(const Nepeta::Node& node);

	private:
		SimMgr &mgr;
};

#endif
