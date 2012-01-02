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
#include "data/BaseData.h"
#include "StateObj.h"
#include "Save.h"

namespace Sim {
	// Forward declarations
	class Simulation;
	
	static const uint32_t FactoryNoId = -1;
	
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
			
			const DataList &getData() const { return mData; }
			
		private:
			uint32_t mCurrentUniqueId;
			
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
				virtual const typename DataBehaviourT<T>::Behaviour *
					getBehaviourFromName(const std::string &name) const
				{ throw std::string("UidFactory::getBehaviourFromName() unimplemented"); }
				virtual const typename DataBehaviourT<T>::Behaviour *
					getBehaviourFromId(uint32_t id) const
				{ throw std::string("UidFactory::getBehaviourFromId() unimplemented"); }
				
				/**
				 * Creates a type based on an implementation, and
				 * inserts it into the factory. May create any valid inheritor
				 * of the type this factory uses.
				 * 
				 * This function uses Impl::getTypeName() to find the
				 * typeid of the new object.
				 * 
				 * @return A pointer to the new object if successful, or NULL if
				 * the type is not registered to the simulation.
				 */
				template<typename Impl>
				Impl *createType(const typename Impl::Config &cfg) {
					const typename DataBehaviourT<T>::Behaviour *b =
						getBehaviourFromName(Impl::getTypeName());
					if(!b)
						return 0;
					
					typename UidFactory<T>::InsertData insData =
						UidFactory<T>::insertObject();
					Impl *tmp = new Impl(mSim, insData.first, b->mId, cfg);
					*insData.second = tmp;
					
					return tmp;
				}
				
				/**
				 * Creates an abstract type based on a type ID.
				 * 
				 * As the ID of the created type is know, it should be
				 * trivial to cast the abstract pointer to the proper type.
				 */
				T *createObjFromTypeName(const std::string &name) {
					typename DataBehaviourT<T>::Behaviour *b =
						getBehaviourFromName(name);
					if(!b)
						return 0;
					
					typename UidFactory<T>::InsertData insData =
						UidFactory<T>::insertObject();
					T *obj = b->createObj(mSim, insData.first);
					*insData.second = obj;
					
					return obj;
				}
			//@}
			
		protected:
			/// @name Factory-required default functions
			//@{
				virtual void saveObj(T *obj, Save::BasePtr &fp) {
					fp.writeInt<uint32_t>(obj->getTypeId());
					obj->save(fp);
				}
				
				virtual T *loadObj(uint32_t internalId, Save::BasePtr &fp) {
					uint32_t type = fp.readInt<uint32_t>();
					
					const typename DataBehaviourT<T>::Behaviour *b =
						getBehaviourFromId(type);
					
					T *obj = b->createObj(mSim, internalId);
					obj->load(fp);
					
					return obj;
				}
			//@}
			
			Simulation *mSim;
	};
}

#endif
