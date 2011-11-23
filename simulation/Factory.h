#ifndef SIM_FACTORY_H
#define SIM_FACTORY_H

#include <stack>
#include <vector>
#include <algorithm>
#include <stdint.h>

#include <stdio.h>

#include <boost/bind.hpp>

#include "utility/CallGroup.h"
#include "StateObj.h"
#include "Save.h"

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
	 * 
	 * The object this template uses is required to have the following
	 * implemented:
	 * - isDead() : Should return true when the object is to be purged.
	 * - getId() : Returns the ID of this object
	 */
	template<class T>
	class Factory {
		protected:
			typedef std::vector<uint32_t> IdStack;
			
		public:
			typedef std::vector<T*> ObjVec;
			
			uint32_t NoId() const { return -1; }
			
			Factory() :
				mIdCounter(0)
				{}
			virtual ~Factory() {}
			
			virtual void deleteInstance(T *obj)=0;
			
			void addObj(T *obj) {
				mData[obj->getId()] = obj;
			}
			
			void removeObj(uint32_t id) {
				T *obj = getObject(id);
				if(obj) {
					deleteInstance(mData[id]);
					freeId(id);
					mData[id] = 0;
				}
			}
			
			template<typename Func>
			void factoryCall(Func func)
			{
				/// @note If new objects are created during this
				/// it is unpredictable whether they are called or not.
				for(uint32_t i = 0; i<mData.size(); i++) {
					T *obj = mData[i];
					
					if(obj) {
						// If the object is not dead, then process it
						if(!obj->isDead())
							func(obj);
						
						// Free and disable this ID
						// if the object is dead
						if(obj->isDead()) {
							removeObj(i);
						}
					}
				}
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
					uint32_t id = mFreeId.back();
					mFreeId.pop_back();
					return id;
				}
			}
			
			void freeId(uint32_t id) {
				mFreeId.push_back(id);
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
	};
	
	/**
	 * This template provides a default implementation
	 * of the factory tailored to the simulation.
	 */
	template<typename T>
	class DefaultFactory : public Factory<T>, public StateObj {
		public:
			/// @name Initialization
			//@{
				DefaultFactory(Simulation *sim) : mSim(sim) {}
				virtual ~DefaultFactory() {}
				
				void startup() {}
				void shutdown()
					{ Factory<T>::killAll(); }
			//@}
			
			/// @name Interaction
			//@{
				void step(double stepTime)
				{ Factory<T>::factoryCall(boost::bind(&T::step, _1, stepTime)); }
				
				uint32_t create(const typename T::Config &cfg)
				{
					uint32_t id = Factory<T>::newId();
					Factory<T>::addObj(new T(mSim, id, cfg));
					return id;
				}
				
				const T *getObj(uint32_t id) const
					{ return Factory<T>::getObject(id); }
				T *getObj(uint32_t id)
					{ return Factory<T>::getObject(id); }
				
				const typename Factory<T>::ObjVec &getObjVector() const
					{ return Factory<T>::mData; }
			//@}
			
			/**
			 * Saves the factory into a savestate.
			 */
			void save(Save::BasePtr &fp)
			{
				// Save the data list
				fp.writeInt<uint32_t>(Factory<T>::mIdCounter);
				for(typename Factory<T>::ObjVec::iterator i
						=Factory<T>::mData.begin();
					i!=Factory<T>::mData.end(); ++i) {
					
					fp.writeInt<uint8_t>( (*i) != NULL );
					if( *i )
						(*i)->save(fp);
				}
				
				// Save the stack
				fp.writeInt<uint32_t>(Factory<T>::mFreeId.size());
				for(uint32_t i=0; i<Factory<T>::mFreeId.size(); ++i) {
					fp.writeInt<uint32_t>(Factory<T>::mFreeId[i]);
				}
			}
			
			/**
			 * Loads the factory from a savestate.
			 */
			void load(Save::BasePtr &fp)
			{
				// Destroy all objects in the factory
				Factory<T>::killAll();
				
				// Load all objects
				uint32_t facSize = fp.readInt<uint32_t>();
				for(uint32_t id=0; id<facSize; ++id) {
					Factory<T>::mData.push_back(0);
					uint8_t hasObj = fp.readInt<uint8_t>();
					if(hasObj) {
						T * obj = new T(mSim, id);
						Factory<T>::mData[id] = obj;
						obj->load(fp);
					}
				}
				
				// Set the id counter to the factory size.
				// This is because the id counter in effect points to
				// the maximum unallocated id (neither on stack nor active)
				Factory<T>::mIdCounter = facSize;
				
				// Load the id stack
				uint32_t stkSize = fp.readInt<uint32_t>();
				Factory<T>::mFreeId.resize(stkSize);
				
				for(uint32_t i=0; i<Factory<T>::mFreeId.size(); ++i) {
					Factory<T>::mFreeId[i] = fp.readInt<uint32_t>();
				}
			}
			
		protected:
			virtual void deleteInstance(T *obj) { delete obj; }
			
			Simulation *mSim;
	};
}

#endif
