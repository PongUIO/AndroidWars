#ifndef STARTUP_H
#define STARTUP_H

#include "../Command.h"

class SimMgr;

class startup : public Command 
{
	public:
		startup(SimMgr &tmgr);
		~startup();
	
		void run(const Nepeta::Node& node);

	private:
		SimMgr &mgr;
	
};

#endif
