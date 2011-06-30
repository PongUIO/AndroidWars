#ifndef SIM_APPLYCOL_H
#define SIM_APPLYCOL_H

#include <vector>
#include <set>

namespace Sim {
	class ColInfo {
		
	};
	
	class ColEntity {
	};
	
	class CollisionFunction {
	};
	
	class ColFuncRay : public CollisionFunction {
	};
	
	template<class T>
	class ColFuncMember : public CollisionFunction {
	};
	
	class ApplyCol {
		public:
			ApplyCol()  {}
			~ApplyCol() {}
			
			
		private:
			
	};
	
}

#endif
