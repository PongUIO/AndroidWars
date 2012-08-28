#ifndef EXTSIM_AGENT_H
#define EXTSIM_AGENT_H

#include "../../simulation/Common.h"

namespace exts {
	class Agent {
		public:
			Agent(Sim::IdType id,
			Sim::IdType startId=0, Sim::IdType endId=Sim::NoId) :
				mId(id),
				mBaseId(startId), mPeekId(startId), mMaxId(endId),
				mRefPlayer(Sim::NoId) {}
			~Agent() {}
			
			/// @name Interface
			//@{
				Sim::IdType getId() const { return mId; }
				
				void setupPlayer(Sim::IdType playerId)
				{ mRefPlayer = playerId; }
				
				Sim::IdType allocateId()
				{
					if(mPeekId < mMaxId)
						return mPeekId++;
					else
						return Sim::NoId;
				}
				
				void updateAlloc()
				{	mBaseId=mPeekId; }
				void resetAlloc()
				{	mPeekId=mBaseId; }
			//@}
			
		private:
			Sim::IdType mId;
			
			/// @name ID allocation
			//@{
				Sim::IdType mBaseId;
				Sim::IdType mPeekId;
				
				Sim::IdType mMaxId;
			//@}
			
			/// @name Simulation metadata
			//@{
				Sim::IdType mRefPlayer;
			//@}
	};
}

#endif
