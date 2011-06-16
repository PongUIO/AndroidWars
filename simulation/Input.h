#ifndef SIM_INPUT_H
#define SIM_INPUT_H

#include <stdint.h>

#include <queue>

namespace Sim {
	struct BotInput {
		BotInput(uint32_t id=0, uint32_t msg=0) :
			botId(id), dbgMessage(msg)
			{}
		
		uint32_t botId;
		uint32_t dbgMessage;
	};
	
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
				mInputQueue.pop();
				return tmp;
			}
			
			void addInput(const T &i) {
				mInputQueue.push(i);
			}
			
		private:
			typedef std::queue<T> InputQueue;
			InputQueue mInputQueue;
	};
}

#endif
