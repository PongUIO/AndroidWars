#ifndef SIM_COLMAP_H
#define SIM_COLMAP_H
// Other includes
#include "../Vector.h"

namespace Sim {
	// Forward declaration
	class CollisionHandler;
	class Collision;
	class ColGroup;
	class CollisionInfo;

	/**
	* Contains an arbitrary-dimension collision map.
	* The map is designed to store entities sectioned in "cubes" based on their
	* bounding box. Each entity may inhabit more than one cube, but retrieving
	* all entities from a bounding box returns a sorted stack with only
	* one instance per stack.
	*/
	template<typename Vec, int DIM>
	class BaseMap {
		public:
			/// A slot in the map.
			struct Slot {
				ColMap::IndexType ind[DIM];
				
				inline bool operator==(Slot const &b)
				{
					for(unsigned int i=0; i<DIM; i++) {
						if(ind[i] != b.ind[i])
							return false;
					}
					
					return true;
				}
			};
	
			/// A section refered to by a slot
			class Section { public: ColMap::ColEntityVec data; };
		
		private:
			/// @name Loop operations
			//@{
				struct CheckOper {
					void operator()(BaseMap<Vec,DIM> &h,
									Slot &slot, ColMap::ColSet *arg)
					{
						Section *c = h.getSection(slot);
						if(c) {
							ColMap::ColEntityVec &vec = c->data;
							arg->insert(vec.begin(), vec.end());
						}
					}
				};
				
				struct SetOper {
					void operator()(BaseMap<Vec,DIM> &h,
									Slot &slot, CollisionEntity *data)
					{
						Section *sec = h.getSection(slot);
						if(!sec)
							sec = h.addSection(slot);
						sec->data.push_back(data);
					}
				};
				
				template<class Oper, class Arg, int CDIM>
				struct Loop {
					inline static void doloop(
						BaseMap<Vec,DIM> &base, Slot &slot,
						ColMap::IndexType miI[DIM], ColMap::IndexType maI[DIM],
						Arg data)
					{
						for(slot.ind[CDIM] = miI[CDIM];
							slot.ind[CDIM]<=maI[CDIM]; slot.ind[CDIM]++) {
							Loop<Oper,Arg,CDIM+1>::doloop(base,slot, miI, maI, data);
						}
					}
				};
				
				template<class Oper, class Arg>
				struct Loop<Oper,Arg,DIM> {
					inline static void doloop(
						BaseMap<Vec,DIM> &base, Slot &slot,
						ColMap::IndexType miI[DIM], ColMap::IndexType maI[DIM],
						Arg data)
					{
						Oper op;
						op(base,slot,data);
					}
				};
			//@}
		
		public:
			typedef boost::unordered_map<Slot, Section*> SlotEntityMap;
			typedef std::vector<Section*> SectionEntityVec;
			
			typedef std::vector< ColMap::ColSet* > ColSetVec;
			
			BaseMap(double secSize, size_t bucketAlloc) :
				mData(bucketAlloc), mCurSet(-1)
				{	resize(secSize); }
			~BaseMap() {
				clear();
				
				for(typename SectionEntityVec::iterator i=mRecycle.begin();
					i!=mRecycle.end(); ++i)
					delete *i;
				
				for(typename ColSetVec::iterator i=mColSetVec.begin();
					i!=mColSetVec.end(); ++i)
					delete *i;
			}
			
			/// @name Map interaction
			//@{
				void setbox(const Vec &min, const Vec &max, CollisionEntity *data) {
					ColMap::IndexType miI[DIM];
					ColMap::IndexType maI[DIM];
					Slot slot;
					
					for(size_t i=0; i<DIM; ++i) {
						miI[i] = getIndex(min[i]);
						maI[i] = getIndex(max[i]);
					}
					
					Loop<SetOper,CollisionEntity*, 0>::doloop(*this,slot,miI, maI, data);
				}
				
				void checkbox(const Vector3d &min, const Vector3d &max, CollisionHandler &apl) {
					ColMap::IndexType miI[DIM];
					ColMap::IndexType maI[DIM];
					Slot slot;
					
					for(size_t i=0; i<DIM; ++i) {
						miI[i] = getIndex(min[i]);
						maI[i] = getIndex(max[i]);
					}
					
					ColMap::ColSet *cset = pushSet();
					
					Loop<CheckOper, ColMap::ColSet*, 0>::doloop(*this,slot,miI,maI, cset);
					apl.iterateCollision(cset);
					
					popSet();
				}
				
				Section *checkpoint(const Vector3d &pt) {
					Slot slot;
					for(size_t i=0; i<DIM; i++)
						slot.ind[i] = getIndex(pt[i]);
					
					return getSection(slot);
				}
				
				void checkray(const Vector3d &start, const Vector3d &end, CollisionHandler &apl) {
					Vector3d minV = start, maxV = end;
					nVector3::sortMinMax(minV, maxV);
					checkbox(minV, maxV, apl);
				}
				
				// Helper function to clean out a collision map
				void clear() {
					for(typename SlotEntityMap::iterator i=mData.begin();
						i!=mData.end(); ++i) {
						if(i->second) {
							i->second->data.clear();
							mRecycle.push_back(i->second);
						}
					}
					mData.clear();
				}
				
				// Resizes the system
				void resize(double secSize) {
					mSectionSize = secSize;
					mInvSectionSize = 1.0/mSectionSize;
				}
			//@}
			
			friend class CollisionHandler;
		private:
			Section *getSection(const Slot &index) { return mData[index]; }
			Section *addSection(const Slot &index) {
				Section *sec;
				if(mRecycle.size()>0) {
					sec = mRecycle.back();
					mRecycle.pop_back();
				} else
					sec = new Section;
				
				mData[index] = sec;
				return sec;
			}
			
			ColMap::IndexType getIndex(double val)
			{ return ColMap::IndexType(floor(val*mInvSectionSize)); }
			
			/// @name Collision layer data
			//@{
				double mSectionSize;
				double mInvSectionSize;
				SlotEntityMap mData;
				SectionEntityVec mRecycle;
			//@}
			
			/// @name Collision iteration data
			//@{
				// A vector of unordered sets used for iterating through collision
				// entities.
				ColSetVec mColSetVec;
				size_t mCurSet;
				
				ColMap::ColSet *pushSet() {
					ColMap::ColSet *ret = NULL;
					mCurSet++;
					
					if(mCurSet >= mColSetVec.size()) {
						mColSetVec.resize(mCurSet+1);
						ret = new ColMap::ColSet(21);
						mColSetVec[mCurSet] = ret;
					} else {
						ret = mColSetVec[mCurSet];
						ret->clear();
					}
					
					return ret;
				}
				void popSet() { mCurSet--; }
			//@}
	};

	/// Wrapped default implementation for a 3D collision map
	class RectMap : public BaseMap<Vector, 2> {
		public:
			RectMap(double cubeSize, size_t bucketAlloc) :
				BaseMap< Vector, 2 >(cubeSize, bucketAlloc)
				{}
			~RectMap() {}
	};
}
#endif
