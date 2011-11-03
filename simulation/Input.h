#ifndef SIM_INPUT_H
#define SIM_INPUT_H

#include <stdint.h>

#include <queue>

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
