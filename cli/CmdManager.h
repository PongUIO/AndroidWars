#ifndef CMDMANGAGER_H
#define CMDMANGAGER_H

#include <string>
#include <boost/unordered_map.hpp>

#include "Command.h"
#include "SimMgr.h"

class CmdManager {
	public:
		CmdManager();
		~CmdManager();
		
		void run(int argc, char **argv);
		void execute(const std::string &str);
		void execfile(const std::string &str);
		
		SimMgr &getSimMgr() { return msim; }
		
	private:
		typedef boost::unordered_map<std::string, Command*> CommandMap;
		CommandMap mCommands;
		
		void registerCmd(const std::string &str, Command *cmd)
		{ mCommands[str] = cmd;}
		
		void interact();
		SimMgr msim;
		
		GetLine *mgl;
};

#endif
