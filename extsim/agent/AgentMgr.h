#ifndef EXTSIM_AGENTMGR_H
#define EXTSIM_AGENTMGR_H

#include "Agent.h"
#include "../../simulation/Factory.h"

#include "../ExtModule.h"

namespace exts {
	/**
	 * @brief Manages all ExtSim \c Agents
	 * 
	 */
	class AgentMgr : public ExtModule {
		public:
			AgentMgr();
			~AgentMgr();
			
			void startup() {}
			void shutdown() { clearAgents(); }
			
			/// @name Interface
			//@{
				void setupAgents(Sim::IdType count);
				
				Agent *getAgent(Sim::IdType id);
			//@}
			
		private:
			void clearAgents();
			
			typedef std::vector<Agent*> AgentVec;
			AgentVec mAgents;
	};
}

#endif
