#ifndef SIM_INPUT_H
#define SIM_INPUT_H

#include <stdint.h>

#include <deque>
#include "Save.h"
#include "StateObj.h"

namespace Sim {
	/**
	 * This class handles a buffer of input commands.
	 */
	template<class T>
	class InputBuffer {
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
			
			void save(Save::BasePtr &fp) {
				fp.writeInt<uint32_t>(mInputQueue.size());
				for(typename InputQueue::iterator i=mInputQueue.begin();
					i!=mInputQueue.end(); ++i) {
					(*i).save(fp);
				}
			}
			void load(Save::BasePtr &fp) {
				uint32_t numElm = fp.readInt<uint32_t>();
				while( (numElm--) >0 ) {
					T v = T();
					v.load(fp);
					addInput(v);
				}
			}
			
		private:
			typedef std::deque<T> InputQueue;
			InputQueue mInputQueue;
	};
	
	/**
	 * This class handles basic bot input.
	 */
	struct BotInput {
		BotInput(uint32_t botId=0, uint32_t progId=0, uint32_t delay=0) :
			mTargetBot(botId), mProgramId(progId), mDelay(delay)
			{}
		
		uint32_t mTargetBot;
		uint32_t mProgramId;
		uint32_t mDelay;
		
		void save(Save::BasePtr &fp);
		void load(Save::BasePtr &fp);
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
			
			void registerInput(uint32_t botId, uint32_t progId, uint32_t delay)
			{ registerInput(BotInput(botId, progId, delay)); }
			
			void registerInput(const InputBuffer<BotInput> &in)
			{ mBotInput.addInput(in); }
			
		private:
			Simulation *mSim;
			
			InputBuffer<BotInput> mBotInput;
	};
}

#endif
