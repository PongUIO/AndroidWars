#ifndef INCLUDE_H
#define INCLUDE_H

#include <string>
#include <nepeta.h>

#include "../CmdManager.h"
#include "../Command.h"

class include : public Command
{
	public:
		include(CmdManager &tcmd); //tmp cmd
		~include();
		
		void run(const Nepeta::Node& node);
		

	private:
		CmdManager &mcmd; 

};

#endif
