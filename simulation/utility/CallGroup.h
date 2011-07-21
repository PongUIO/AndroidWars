#ifndef SIM_CALLGROUP_H
#define SIM_CALLGROUP_H

#include <vector>

namespace Sim {
	#define CallGroup_Call(v,f) ((v)->*(f))
	#define CallGroup_Iter(x) \
		for(typename DataVec::iterator i=mData.begin(); i!=mData.end(); ++i) { \
			x; \
		}
	#define CallGroup_RIter(x) \
		for(typename DataVec::reverse_iterator i=mData.rbegin(); i!=mData.rend(); ++i) { \
			x; \
		}
	
	/**
	 * Keeps an ordered group of a specific type.
	 * May apply a specific operation of every
	 * containing object using variants of \c call().
	 */
	template<typename CallObj>
	class CallGroup {
		private:
			typedef std::vector<CallObj*> DataVec;
			typedef void(CallObj::*CallFunc)();
		
		public:
			// Functions
			CallGroup() {}
			~CallGroup() {}
			
			/// Calls the group with no parameters
			void call( CallFunc f )
			{	CallGroup_Iter( CallGroup_Call(*i, f)() ) }
			
			void rcall( CallFunc f)
			{	CallGroup_RIter( CallGroup_Call( (*i), f)() ) }
			
			/// Calls the group in normal order
			template<typename Arg>
			void callArg( void(CallObj::*f)(Arg a), Arg a )
			{	CallGroup_Iter( CallGroup_Call(*i, f)(a) ) }
			
			/// Calls the group in reverse order
			template<typename Arg>
			void rcallArg( void(CallObj::*f)(Arg a), Arg a )
			{	CallGroup_RIter( CallGroup_Call(*i, f)(a) ) }
			
			/// Registers a call object
			void registerCallObj(CallObj *v)
			{ mData.push_back(v); }
			
			
		private:
			DataVec mData;
	};
	
	#undef CallGroup_Call
}

#endif
