#ifndef SIM_FACTORY_H
#define SIM_FACTORY_H

#include <stack>
#include <vector>
#include <list>
#include <algorithm>
#include <stdint.h>

#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>

#include "CommonTemplate.h"

#include "utility/CallGroup.h"
#include "data/BaseData.h"
#include "StateObj.h"
#include "Save.h"

#include "Common.h"

namespace Sim {
	// Forward declarations
	class Simulation;
	
	/**
	 * @brief Provides a factory that stores objects in an ordered list with
	 * unique identifiers per entity.
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
	 * - IdType getId()
	 * - bool isDead()
	 */
	template<class T>
	class UidFactory {
		private:
			static void nullfunc() {}
		
		public:
			typedef T Type;
			typedef std::list<T*> DataList;
			typedef boost::unordered_map<IdType, typename DataList::iterator>
				IdResolveMap;
			typedef std::pair<IdType,typename DataList::iterator>
				InsertData;
			typedef std::pair<typename IdResolveMap::iterator,bool>
				InsertResult;
			
			UidFactory()
				{}
			~UidFactory() {}
			
			virtual void deleteInstance(T *obj)=0;
			virtual void saveObj(T*, Save::BasePtr &)=0;
			virtual T *loadObj(IdType id, Save::BasePtr &)=0;
			
			IdType addObj(T *obj) { return insertObject(obj)->first; }
			void removeObj(IdType id) { deleteObject(id); }
			
			template<typename Func>
			void factoryCall(Func func)
			{
				for(typename DataList::iterator i=mData.begin();
					i!=mData.end();) {
					T *obj = *i;
					
					if(!obj->isDead())
						func(obj);
					
					// Increment the iterator pre-emptively
					// this is to avoid iterator invalidation when
					// removeObj() is called
					i++;
					if(obj->isDead()) {
						removeObj(obj->getId());
					}
				}
			}
			
			void cleanDead()
			{
				factoryCall(boost::bind(&nullfunc));
			}
			
			virtual void save(Save::BasePtr &fp)
			{
				fp << IdType(mData.size());
				for(typename DataList::iterator i=mData.begin();
					i!=mData.end(); i++) {
					T *obj = *i;
					fp << obj->getId();
					saveObj(obj, fp);
				}
			}
			
			virtual void load(Save::BasePtr &fp)
			{
				killAll();
				
				IdType objCount;
				fp >> objCount;
				for(IdType i=0; i<objCount; i++) {
					IdType uid;
					fp >> uid;
					T *obj = loadObj(uid, fp);
					
					insertObject(obj, uid);
				}
			}
			
			const DataList &getData() const { return mData; }
			
		protected:
			T *getObject(IdType id) {
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
			}
			
			InsertData insertObject(
			T *obj, IdType uid) {
				// Handle the special illegal identifier
				if(uid == NoId)
					throw IllegalId();
				
				mData.push_back(obj);
				typename DataList::iterator listElm = --mData.end();
				InsertData insertPair = std::make_pair(uid, listElm);
				
				// Insert the data, and check if an element already exists
				// with the same ID
				if(!mIdResolve.insert( insertPair ).second)
					throw IllegalId();
				
				return insertPair;
			}
			
			bool deleteObject(IdType id) {
				typename IdResolveMap::iterator i= mIdResolve.find(id);
				if(i != mIdResolve.end()) {
					deleteInstance(*i->second);
					mData.erase(i->second);
					mIdResolve.erase(i);
					return true;
				}
				return false;
			}
			
		private:
			DataList mData;
			IdResolveMap mIdResolve;
	};
	
	/**
	 * Provides a default implementation of a UidFactory, tailored to the
	 * simulation.
	 */
	template<typename T>
	class DefaultUidFactory : public UidFactory<T>, public StateObj {
		public:
			DefaultUidFactory(Simulation *sim) : UidFactory<T>(), mSim(sim) {}
			virtual ~DefaultUidFactory() {}
			
			/// @name StateObj functions
			//@{
				virtual void startup() {}
				virtual void shutdown()
				{ UidFactory<T>::killAll(); }
				
				virtual void save(Save::BasePtr& fp)
				{ UidFactory<T>::save(fp); }
				virtual void load(Save::BasePtr& fp)
				{ UidFactory<T>::load(fp); }
				
				virtual void startPhase() {}
				virtual void endPhase() {}
				
				virtual void step(double stepTime) {}
			//@}
			
			/// @name Object creation
			//@{
				/**
				 * @brief Constructs an object implementation from parameters.
				 */
				template<class Impl>
				Impl *createImpl(const typename Impl::Config &cfg,
				IdType typeId, IdType id)
				{
					Impl *obj = new Impl(mSim,id,typeId,cfg);
					UidFactory<T>::insertObject(obj,id);
					return obj;
				}
				
				
				/**
				 * Loads and creates an object from a serialized stream \c fp.
				 * 
				 * @param fp Source stream to read the object from.
				 * @param doInsert If true, the object is added to the factory.
				 * 
				 * @return A pointer to the newly created object.
				 */
				T *createSerialized(Save::BasePtr &fp, bool doInsert=true) {
					typename UidFactory<T>::InsertData insData;
					IdType id = NoId;
					
					if(doInsert) {
						insData = UidFactory<T>::insertObject();
						id = insData.first;
					}
					
					T *obj = loadObj(id, fp);
					if(doInsert)
						*insData.second = obj;
					
					return obj;
				}
			//@}
			
			/// @name Factory-required default functions
			//@{
				virtual void saveObj(T *obj, Save::BasePtr &fp) {
					fp << obj->getTypeId();
					obj->save(fp);
				}
				
				virtual T *loadObj(IdType internalId, Save::BasePtr &fp) {
					IdType type;
					fp >> type;
					
					const typename T::TypeDatabase::Type *b =
						getDataObjFromType<T>(*mSim, type);
					
					T *obj = b->createObj(mSim, internalId);
					obj->load(fp);
					
					return obj;
				}
			//@}
			
		protected:
			Simulation *mSim;
	};
}

#endif
