#ifndef SIM_INPUT_H
#define SIM_INPUT_H

#include <stdint.h>

#include <deque>
#include "Save.h"

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
}

#endif
