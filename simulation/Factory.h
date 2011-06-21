#ifndef SIM_FACTORY_H
#define SIM_FACTORY_H

#include <stack>
#include <vector>
#include <algorithm>
#include <stdint.h>

namespace Sim {
	// Forward declarations
	class Simulation;
	
	/**
	 * This template provides an id-referenced grouping of
	 * objects.
	 * 
	 * When calling specific functions (like step()),
	 * every single object is applied the same function ordered
	 * by their ID in ascending order (eg. the object with id 1
	 * is always called before id 2).
	 * 
	 * Any class inheriting this template is required to have the
	 * following implemented:
	 * - deleteInstance(T *obj) : Called when an object is dead.
	 * - newCopyInstance(T *obj) : Called when an object is copied
	 * 
	 * The object this template uses is required to have the following
	 * implemented:
	 * - isDead() : Should return true when the object is to be purged.
	 * - step(double stepTime)
	 * - getId() : Returns the ID of this object
	 */
	template<class T>
	class Factory {
		protected:
			typedef std::stack<uint32_t> IdStack;
			typedef std::vector<T*> ObjVec;
		
		public:
			Factory(Simulation *sim) :
				mIdCounter(0),
				mSim(sim)
				{}
			~Factory() {}
			
			virtual void deleteInstance(T *obj)=0;
			virtual T *newCopyInstance(T *obj)=0;
			
			void addObj(T *obj) {
				mData[obj->getId()] = obj;
			}
			
			void step(double stepTime) {
				for(uint32_t i = 0; i<mData.size(); i++) {
					T *obj = mData[i];
					
					if(obj) {
						// If the object is not dead, then process it
						if(!obj->isDead())
							obj->step(stepTime);
						
						// Free and disable this ID
						// if the object is dead
						if(obj->isDead()) {
							deleteInstance(obj);
							freeId(i);
							mData[i] = NULL;
						}
					}
				}
			}
			
			void copyFactory(const Factory<T> &other) {
				for(uint32_t i = 0; i<mData.size(); i++) {
					T *otherObj = other.mData[i];
					T *selfObj = mData[i];
					
					if(otherObj) {
						if(selfObj)
							*selfObj = *otherObj;
						else
							mData[i] = newCopyInstance(otherObj);
					} else {
						if(selfObj)
							deleteInstance(selfObj);
						else
							; // Nothing to do
					}
				}
			}
			
		protected:
			T *getObject(uint32_t id) const {
				if(id >= mData.size())
					return NULL;
				else
					return mData[id];
			}
			
			uint32_t newId() {
				if(mFreeId.empty()) {
					// Creates a new slot in the vector
					mData.push_back(NULL);
					return mIdCounter++;
				} else {
					uint32_t id = mFreeId.top();
					mFreeId.pop();
					return id;
				}
			}
			
			void freeId(uint32_t id) {
				mFreeId.push(id);
			}
			
			void killAll() {
				for(uint32_t i = 0; i<mData.size(); i++) {
					deleteInstance(mData[i]);
				}
				mData.clear();
				mFreeId = IdStack(); // Clears the free ID list
				mIdCounter = 0;
			}
			
			ObjVec mData;
			IdStack mFreeId;
			uint32_t mIdCounter;
			
			Simulation *mSim;
	};
	
}

#endif
