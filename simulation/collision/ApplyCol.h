#ifndef SIM_APPLYCOL_H
#define SIM_APPLYCOL_H

#include <vector>
#include <set>

#include "Collision.h"
#include "ColMap.h"
#include "Vector.h"

namespace Sim {
	class ColInfo {
		public:
			ColInfo() : mSrcArg(0), mDstArg(0), mTargetPos(0,0)
				{}
			
			void setArgSrc(void *ptr) { mSrcArg = ptr; }
			void setArgDst(void *ptr) { mDstArg = ptr; }
			
			template<class T>
			T *getArgSrc() { return static_cast<T*>(mSrcArg); }
			template<class T>
			T *getArgDst() { return static_cast<T*>(mDstArg); }
			
		private:
			void *mSrcArg, *mDstArg;
			Vector mTargetPos;
	};
	
	class ColEntity {
		public:
			ColEntity() : mSelfCol(0)
				{}
			virtual ~ColEntity()
				{}
			
			virtual void receive(ColInfo &info)=0;
			virtual bool prepare(ColInfo &info)=0;
			
		private:
			Collision *mSelfCol;
	};
	
	class CollisionFunction {
		public:
			virtual bool collide(ColInfo &info, Collision *col)=0;
	};
	
	class ColFuncRay : public CollisionFunction {
		public:
			void setup(const Vector &start, const Vector &end);
			bool collide(ColInfo &info, Collision *col);
			
			Vector getIntersectPoint() { return mIntersectPoint; }
			double getDistance() { return mMinDist; }
			bool hasCol() { return mMinDist<mMaxDist; }
			
		private:
			double mMinDist;
			double mMaxDist;
			Vector mIntersectPoint;
			
			Vector mStart, mEnd;
	};
	
	template<class T>
	class ColFuncMember : public CollisionFunction {
		public:
			typedef bool (T::*ColFuncType)(ColInfo &info, Collision *col);
			
			ColFuncMember(T *ref, ColFuncType func) :
				ref(ref), func(func)
				{}
			
			bool collide(ColInfo &info, Collision *col) {
				return ((ref)->*(func))(info,col);
			}
			
		private:
			T *ref;
			ColFuncType func;
	};
	
	class ApplyCol {
		public:
			ApplyCol() : mColMap(0)
				{}
			~ApplyCol()
				{}
			
			
		private:
			ColMap<>
	};
	
}

#endif
