#ifndef SIM_FACTORY_H
#define SIM_FACTORY_H

#include <vector>
#include <stdint.h>

namespace Sim {
	
	template<class T>
	class Factory {
		public:
			Factory() : mCurId(1) {}
			~Factory() {}
			
			void addObj(T *obj);
			
		protected:
			typedef std::vector<T*> ObjVec;
			
			uint32_t newId() { return mCurId++; }
			
			ObjVec mData;
			uint32_t mCurId;
	};
	
}

#endif
