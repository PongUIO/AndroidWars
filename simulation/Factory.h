#ifndef SIM_FACTORY_H
#define SIM_FACTORY_H

#include <stack>
#include <vector>
#include <algorithm>
#include <stdint.h>

#include <stdio.h>

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
	template<class Host, class T>
	class Factory {
		protected:
			typedef std::stack<uint32_t> IdStack;
			
		public:
			typedef std::vector<T*> ObjVec;
			typedef void(Host::*CallFunc)(T *v);
			
			uint32_t NoId() const { return -1; }
			
			Factory(Host *host) :
				mIdCounter(0),
				mHost(host)
				{}
			~Factory() {}
			
			virtual void deleteInstance(T *obj)=0;
			virtual T *newCopyInstance(T *obj)=0;
			
			void addObj(T *obj) {
				mData[obj->getId()] = obj;
			}
			
			void factoryCall(CallFunc f)
			{
				/// @note If new objects are created during this
				/// it is unpredictable whether they are called or not.
				for(uint32_t i = 0; i<mData.size(); i++) {
					T *obj = mData[i];
					
					if(obj) {
						// If the object is not dead, then process it
						if(!obj->isDead())
							(mHost->*(f))(obj);
						
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
			
			void copyFactory(const Factory<Host,T> &other) {
				// Make sure both vectors are of equal size
				if(mData.size() < other.mData.size())
					mData.resize(other.mData.size());
				
				// Purge extra elements
				while(mData.size() > other.mData.size()) {
					T *obj = mData.back();
					mData.pop_back();
					if(obj)
						deleteInstance(obj);
				}
				
				// Copy the data objects in the vector
				for(uint32_t i = 0; i<other.mData.size(); i++) {
					T *otherObj = other.getObject(i);
					T *selfObj = getObject(i);
					
					if(otherObj) {
						if(selfObj)
							*selfObj = *otherObj;
						else
							setObject( newCopyInstance(otherObj), i );
					} else {
						if(selfObj) {
							deleteInstance(selfObj);
							setObject(NULL, i);
						} else
							; // Nothing to do
					}
				}
				
				// Copy other state variables
				mFreeId = IdStack(other.mFreeId);
				mIdCounter = other.mIdCounter;
			}
			
		protected:
			void setObject(T *obj, uint32_t id) {
				if(id >= mData.size())
					mData.resize(id+1);
				
				mData[id] = obj;
			}
			
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
			
			Host *mHost;
	};
	
}

#endif
