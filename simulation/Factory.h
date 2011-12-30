#ifndef SIM_FACTORY_H
#define SIM_FACTORY_H

#include <stack>
#include <vector>
#include <list>
#include <algorithm>
#include <stdint.h>

#include <stdio.h>

#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>

#include "utility/CallGroup.h"
#include "StateObj.h"
#include "Save.h"

namespace Sim {
	// Forward declarations
	class Simulation;
	
	static const uint32_t FactoryNoId = -1;
	
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
	 * - saveObj(T* obj, Save::BasePtr &fp) : Called when an object is saved
	 * - loadObj(uint32_t id, Save::BasePtr &fp) : Called when an object is
	 * 			loaded
	 * 
	 * The object this template uses is required to have the following
	 * implemented:
	 * - isDead() : Should return true when the object is to be purged.
	 */
	template<class T>
	class Factory {
		protected:
			typedef std::vector<uint32_t> IdStack;
			
		public:
			typedef std::vector<T*> ObjVec;
			
			Factory() : mIdCounter(0)
				{}
			virtual ~Factory() {}
			
			virtual void deleteInstance(T *obj)=0;
			virtual void saveObj(T*, Save::BasePtr &)=0;
			virtual T *loadObj(uint32_t id, Save::BasePtr&)=0;
			
			uint32_t addObj(T *obj, uint32_t idPos) {
				if(idPos >= mData.size())
					idPos = newId();
				mData[idPos] = obj;
				return idPos;
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
			
			/**
			 * Saves the factory into a savestate.
			 */
			virtual void save(Save::BasePtr &fp)
			{
				// Save the data list
				fp.writeInt<uint32_t>(mIdCounter);
				for(typename ObjVec::iterator i=mData.begin();
					i!=mData.end(); ++i) {
					
					fp.writeInt<uint8_t>( (*i) != NULL );
					if( *i )
						saveObj(*i, fp);
				}
				
				// Save the stack
				fp.writeInt<uint32_t>(mFreeId.size());
				for(uint32_t i=0; i<mFreeId.size(); ++i) {
					fp.writeInt<uint32_t>(mFreeId[i]);
				}
			}
			
			/**
			 * Loads the factory from a savestate.
			 */
			virtual void load(Save::BasePtr &fp)
			{
				// Destroy all objects in the factory
				killAll();
				
				// Load all objects
				uint32_t facSize = fp.readInt<uint32_t>();
				for(uint32_t id=0; id<facSize; ++id) {
					mData.push_back(0);
					uint8_t hasObj = fp.readInt<uint8_t>();
					if(hasObj)
						mData[id] = loadObj(id, fp);
				}
				
				// Set the id counter to the factory size.
				// This is because the id counter in effect points to
				// the maximum unallocated id (neither on stack nor active)
				mIdCounter = facSize;
				
				// Load the id stack
				uint32_t stkSize = fp.readInt<uint32_t>();
				mFreeId.resize(stkSize);
				
				for(uint32_t i=0; i<mFreeId.size(); ++i) {
					mFreeId[i] = fp.readInt<uint32_t>();
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
	 * This template provides a variant of factories that provide
	 * unique ids for an entity.
	 * 
	 * This uses an implementation which mix a linked list (providing strict
	 * ordering) and a hash table (providing fast random access and fast
	 * deletion). The hash table is essentially an index that maps numerical
	 * ids to iterators.
	 * 
	 * The interface is designed to be as similar to ordinary factories as
	 * possible.
	 * 
	 * This factory is generally superior to ordinary factories when it comes
	 * to deterministic behaviour. The downsides being that it is slower at
	 * inserting/removing elements, and has only a finite number of ids.
	 * 
	 * Objects of type \c T need the following implemented:
	 * - uint32_t getId()
	 * - bool isDead()
	 */
	template<class T>
	class UidFactory {
		public:
			UidFactory() : mCurrentUniqueId(0)
				{}
			~UidFactory() {}
			
			virtual void deleteInstance(T *obj)=0;
			virtual void saveObj(T*, Save::BasePtr &)=0;
			virtual T *loadObj(uint32_t id, Save::BasePtr &)=0;
			
			uint32_t addObj(T *obj) { return insertObject(obj)->first; }
			void removeObj(uint32_t id) { deleteObject(id); }
			
			template<typename Func>
			void factoryCall(Func func)
			{
				for(typename DataList::iterator i=mData.begin();
					i!=mData.end(); i++) {
					T *obj = *i;
					
					if(!obj->isDead())
						func(obj);
					
					if(obj->isDead())
						removeObj(obj->getId());
				}
			}
			
			virtual void save(Save::BasePtr &fp)
			{
				fp.writeInt<uint32_t>(mCurrentUniqueId);
				
				fp.writeInt<uint32_t>(mData.size());
				for(typename DataList::iterator i=mData.begin();
					i!=mData.end(); i++) {
					T *obj = *i;
					fp.writeInt<uint32_t>(obj->getId());
					saveObj(obj, fp);
				}
			}
			
			virtual void load(Save::BasePtr &fp)
			{
				killAll();
				
				mCurrentUniqueId = fp.readInt<uint32_t>();
				
				uint32_t objCount = fp.readInt<uint32_t>();
				for(uint32_t i=0; i<objCount; i++) {
					uint32_t uid = fp.readInt<uint32_t>();
					T *obj = loadObj(uid, fp);
					
					insertObject(obj, uid);
				}
			}
			
		protected:
			typedef std::list<T*> DataList;
			typedef boost::unordered_map<uint32_t, typename DataList::iterator>
				IdResolveMap;
			typedef std::pair<uint32_t,typename DataList::iterator>
				InsertData;
			
			T *getObject(uint32_t id) {
				typename IdResolveMap::iterator i = mIdResolve.find(id);
				return (i==mIdResolve.end()) ? 0 : *i->second;
			}
			
			void killAll() {
				for(typename DataList::iterator i=mData.begin();
					i!=mData.end(); i++) {
					deleteInstance(*i);
				}
				mData.clear();
				mIdResolve.clear();
				
				mCurrentUniqueId = 0;
			}
			
			InsertData insertObject(
				T *obj=0, uint32_t uid=FactoryNoId) {
				
				if(uid == FactoryNoId)
					uid = mCurrentUniqueId++;
				
				mData.push_back(obj);
				typename DataList::iterator listElm = --mData.end();
				InsertData insertPair = std::make_pair(uid, listElm);
				mIdResolve.insert( insertPair );
				
				return insertPair;
			}
			
			void deleteObject(uint32_t id) {
				typename IdResolveMap::iterator i= mIdResolve.find(id);
				if(i != mIdResolve.end()) {
					deleteInstance(*i->second);
					mData.erase(i->second);
					mIdResolve.erase(i);
				}
			}
			
			const DataList &getData() { return mData; }
			
		private:
			uint32_t mCurrentUniqueId;
			
			DataList mData;
			IdResolveMap mIdResolve;
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
				virtual void step(double stepTime)
				{ Factory<T>::factoryCall(boost::bind(&T::step, _1, stepTime)); }
				
				uint32_t create(const typename T::Config &cfg)
				{
					uint32_t id = Factory<T>::newId();
					return Factory<T>::addObj(new T(mSim, id, cfg), id);
				}
				
				const T *getObj(uint32_t id) const
					{ return Factory<T>::getObject(id); }
				T *getObj(uint32_t id)
					{ return Factory<T>::getObject(id); }
				
				const typename Factory<T>::ObjVec &getObjVector() const
					{ return Factory<T>::mData; }
				
				void save(Save::BasePtr &fp) { Factory<T>::save(fp); }
				void load(Save::BasePtr &fp) { Factory<T>::load(fp); }
			//@}
			
			
			
		protected:
			virtual void deleteInstance(T *obj) { delete obj; }
			
			virtual void saveObj(T *obj, Save::BasePtr &fp) {
				obj->save(fp);
			}
			virtual T* loadObj(uint32_t id, Save::BasePtr &fp) {
				T * obj = new T(mSim, id);
				obj->load(fp);
				return obj;
			}
			
			Simulation *mSim;
	};
}

#endif
