#ifndef SIM_CALLGROUP_H
#define SIM_CALLGROUP_H

#include <vector>

namespace Sim {
	#define CallObj_Call(v,f) ((v)->*(f))
	
	template<typename Obj>
	class BaseObjCall {
		public:
			typedef void(Obj::*CallFunc)();
			
			template<typename Arg, typename T>
			static void doCall( Obj *obj, T func, Arg a)
				{ CallObj_Call(obj, func)(a); }
			
			template<typename Arg>
			static void doCall( Obj *obj, CallFunc func, Arg a)
				{ CallObj_Call(obj, func)(); }
	};
	
	#undef CallObj_Call
	
	/**
	 * Keeps an ordered group of a specific type.
	 * May apply a specific operation of every
	 * containing object using variants of \c call().
	 */
	template<typename CallObj>
	class CallGroup : private BaseObjCall<CallObj> {
		private:
			typedef std::vector<CallObj*> DataVec;
			
		public:
			// Functions
			CallGroup() {}
			~CallGroup() {}
			
			/// Calls the group in normal order with a single argument
			template<typename Arg, typename Func>
			void callArg( Func f, Arg a )
			{
				for(typename DataVec::iterator i=mData.begin(); i!=mData.end(); ++i) {
					doCall(*i, f, a);
				}
			}
			
			/// Calls the group in reverse order with a single argument
			template<typename Arg, typename Func>
			void rcallArg( Func f, Arg a )
			{
				for(typename DataVec::reverse_iterator i=mData.rbegin(); i!=mData.rend(); ++i) {
					doCall(*i, f, a);
				}
			}
			
			/// Calls the group in sequence with no parameters
			template<typename Func>
			void call( Func f )
			{	callArg(f, 0); }
			
			/// Equivalent to \c call, but in reverse order
			template<typename Func>
			void rcall( Func f )
			{	rcallArg(f, 0); }
			
			/// Registers a call object
			void registerCallObj(CallObj *v)
			{ mData.push_back(v); }
			
		private:
			DataVec mData;
	};
}

#endif
