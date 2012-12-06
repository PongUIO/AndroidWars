#ifndef EXTSIM_TIMELINEMGR_H
#define EXTSIM_TIMELINEMGR_H

#include <vector>
#include <boost/unordered_map.hpp>

#include "ExtModule.h"
#include "object/ParamList.h"
#include "../simulation/Bot.h"

namespace exts {
	class ExtSim;
	
	class TlNode {
		public:
			typedef std::vector<ParamList*> OrderVec;
			
			TlNode(Sim::IdType botId, const Sim::Vector &botPos);
			~TlNode();
			
			void registerOrder(ParamList *param);
			
		private:
			Sim::IdType mBotId;
			uint32_t mPhase, mStep;
			Sim::Vector mBotPos;
			
			OrderVec mOrder;
	};
	
	class TlBot {
		public:
			
		private:
			Sim::IdType mBotId;
	};
	
	/**
	 * @brief Stores data about bot commands, and when and where they
	 * were called.
	 */
	class TimelineMgr : public ExtModule {
		public:
			TimelineMgr(ExtSim &esim) : ExtModule(esim) {}
			virtual ~TimelineMgr() {}
			
			void startup() {}
			void shutdown() {}
			
			void update();
			
			void registerOrder(Sim::IdType botId, ParamList *param);
			
		private:
			typedef boost::unordered_map<Sim::IdType, TlBot*> BotNodeMap;
			
			/// Bot-associated
			BotNodeMap mBotNode;
	};
}

#endif
