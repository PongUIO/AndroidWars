#ifndef SIM_COLMAP_H
#define SIM_COLMAP_H

#include <stdint.h>
#include <vector>
#include <math.h>

#include <boost/unordered_map.hpp>

namespace Sim {
	template<class T>
	class ColMap {
		private:
			/// @name Substructure definitions
			//@{
				typedef std::vector<T*> ColEntityVec;
				
				typedef int32_t IndexType;
				struct RectSlot {
					IndexType x,y;
				};
				
				struct ColRect {
					ColEntityVec mEntVec;
				};
				
				typedef boost::unordered_map<RectSlot, ColRect*> RectMap;
				typedef std::vector<ColRect*> RectVec;
			//@}
		
		public:
			
			
			
		private:
			ColRect &getRect(const RectSlot &index);
			ColRect &addRect(const RectSlot &index);
			
			IndexType getIndex(double val)
			{ return IndexType(floor(val*mInvRectSize)); }
			
			double mRectSize;
			double mInvRectSize;
			
			RectMap mRectMap;
			RectVec mRecycleRect;
			
	};
}

#endif
