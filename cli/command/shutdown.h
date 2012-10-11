#ifndef SHUTDOWN_H
#define SHUTDOWN_H

#include "../Command.h"

class SimMgr;

class shutdown : public Command
{
	public:
		shutdown(SimMgr &tmgr);
		~shutdown();
		
		void run(const Nepeta::Node& node);
		
	private:
		SimMgr &mgr;
};

#endif
