#ifndef SIM_CALLGROUP_H
#define SIM_CALLGROUP_H

#include <vector>

namespace Sim {	
	/**
	 * Keeps an ordered group of a specific type.
	 * May apply a specific operation of every
	 * containing object using variants of \c call().
	 */
	template<typename CallObj>
	class CallGroup {
		private:
			typedef std::vector<CallObj*> DataVec;
			
		public:
			// Functions
			CallGroup() {}
			~CallGroup() {}
			
			/// Calls the group in normal order with a single argument
			template<typename Func>
			void call( Func func )
			{
				for(typename DataVec::iterator i=mData.begin(); i!=mData.end(); ++i) {
					func(*i);
				}
			}
			
			/// Calls the group in reverse order with a single argument
			template<typename Func>
			void rcall( Func func )
			{
				for(typename DataVec::reverse_iterator i=mData.rbegin(); i!=mData.rend(); ++i) {
					func(*i);
				}
			}
			/// Registers a call object
			void registerCallObj(CallObj *v)
			{ mData.push_back(v); }
			
		protected:
			void clear() { mData.clear(); }
			
		private:
			DataVec mData;
	};
}

#endif
