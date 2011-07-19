#ifndef SIM_CALLGROUP_H
#define SIM_CALLGROUP_H

#include <vector>

namespace Sim {
	#define CallGroup_Call(f) (mHost->*(f))
	
	/**
	 * Keeps an ordered group of a specific type.
	 * May apply a specific operation of every
	 * containing object using variants of \c call().
	 */
	template<typename Host, typename CallObj>
	class CallGroup {
		private:
			typedef std::vector<CallObj*> DataVec;
			typedef void(Host::*CallFunc)(CallObj *v);
		
		public:
			// Functions
			CallGroup(Host *host) : mHost(host) {}
			~CallGroup() {}
			
			/// Calls the group in normal order
			void call(CallFunc f)
			{
				for(typename DataVec::iterator i=mData.begin();
					i!=mData.end(); ++i) {
					CallGroup_Call(f)(*i);
				}
			}
			
			/// Calls the group in reverse order
			void rcall(CallFunc f)
			{
				for(typename DataVec::reverse_iterator i=mData.rbegin();
					i!=mData.rend(); ++i) {
					CallGroup_Call(f)(*i);
				}
			}
			
			/// Registers a call object
			void registerCallObj(CallObj *v)
			{ mData.push_back(v); }
			
			
		private:
			DataVec mData;
			Host *mHost;
	};
	
	#undef CallGroup_Call
}

#endif
