#ifndef SIM_INPUT_H
#define SIM_INPUT_H

#include <stdint.h>
#include <boost/bind.hpp>

#include <deque>
#include "Save.h"
#include "Common.h"
#include "StateObj.h"

namespace Sim {
	/**
	 * This class handles a buffer of input commands.
	 */
	template<class T>
	class InputBuffer : private Save::OperatorImpl< InputBuffer<T> > {
		public:
			InputBuffer() {}
			~InputBuffer() {}
			
			bool hasInput() {
				return !mInputQueue.empty();
			}
			
			T nextInput() {
				T tmp = mInputQueue.front();
				mInputQueue.pop_front();
				return tmp;
			}
			
			void addInput(const T &i) {
				mInputQueue.push_back(i);
			}
			
			void addInput(const InputBuffer<T> &buf) {
				for(typename InputQueue::const_iterator i=
					buf.mInputQueue.begin(); i!=buf.mInputQueue.end(); i++) {
					mInputQueue.push_back(*i);
				}
			}
			
			void save(Save::BasePtr &fp) const {
				fp.writeCtr(mInputQueue);
			}
			void load(Save::BasePtr &fp) {
				fp.readCtr(mInputQueue);
			}
			
		private:
			typedef std::deque<T> InputQueue;
			InputQueue mInputQueue;
	};
	
	/**
	 * This class handles basic bot input.
	 */
	struct BotInput : public Save::OperatorImpl<BotInput> {
		BotInput(IdType botId=0, IdType progId=0, uint32_t delay=0) :
			mTargetBot(botId), mProgramId(progId), mDelay(delay)
			{}
		
		IdType mTargetBot;
		IdType mProgramId;
		uint32_t mDelay;
		
		void save(Save::BasePtr &fp) const
		{ fp << mTargetBot << mProgramId << mDelay; }
		void load(Save::BasePtr &fp)
		{ fp >> mTargetBot >> mProgramId >> mDelay; }
	};
	
	class InputManager : public StateObj {
		public:
			InputManager(Simulation *sim);
			~InputManager();
			
			/// @name StateObj calls
			//@{
				void startup();
				void shutdown();
				
				void startPhase();
				void step(double stepTime);
				void endPhase();
				
				void save(Save::BasePtr &fp);
				void load(Save::BasePtr &fp);
			//@}
			
			void registerInput(const BotInput &in)
			{ mBotInput.addInput(in); }
			
			void registerInput(IdType botId, IdType progId, uint32_t delay)
			{ registerInput(BotInput(botId, progId, delay)); }
			
			void registerInput(const InputBuffer<BotInput> &in)
			{ mBotInput.addInput(in); }
			
		private:
			Simulation *mSim;
			
			InputBuffer<BotInput> mBotInput;
	};
}

#endif
