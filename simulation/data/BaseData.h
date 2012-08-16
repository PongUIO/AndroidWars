#ifndef SIM_BASEDATA_H
#define SIM_BASEDATA_H

#include <boost/unordered_map.hpp>
#include <vector>
#include <string>
#include <stdint.h>

#include "../Common.h"

namespace Sim {
	class Simulation;
	
	/**
	 * Base interface that needs to be implemented for all database
	 * objects in \c Data.
	 */
	class BaseDatabase {
		public:
			BaseDatabase() {}
			virtual ~BaseDatabase() {}
			
			virtual void startup(Simulation *sim)=0;
			virtual void shutdown()=0;
		
	};
	
	/**
	 * @brief Light wrapper around \c std::vector specialized for external
	 * read-only access.
	 * 
	 * Contains a vector of ID-referenced objects. All objects stored in
	 * the container is assumed to belong to the container. This means that
	 * the container is responsible for destroying the objects.
	 */
	template<typename T>
	class DataCtr {
		public:
			DataCtr() {}
			~DataCtr() { clear(); }
			
			/// Returns the object corresponding to the given \a id, or
			/// null if no object with that id exists.
			const T *get(IdType id) const
			{ return (id>=mData.size()) ? 0 : mData[id]; }
			
			/// @copydoc getObj
			T *get(IdType id)
			{ return (id>=mData.size()) ? 0 : mData[id]; }
			
			/**
			 * Inserts a new object into the container. This new object
			 * is given an ID which is equal to the container's previous size.
			 * 
			 * @return The inserted object's ID.
			 */
			IdType add(T *obj)
			{ mData.push_back(obj); return IdType(mData.size()-1); }
			
			/**
			 * @brief Replaces the object with the given ID with another.
			 * 
			 * The old object is not destroyed by this call.
			 */
			void set(IdType id, T *obj)
			{ mData[id] = obj; }
			
			/**
			 * Removes and deletes all objects in the container.
			 */
			void clear()
			{
				for(typename DataVec::iterator i=mData.begin();
				i!=mData.end(); ++i) {
					if(*i)
						delete *i;
				}
				
				mData.clear();
			}
			
			/**
			 * Returns the size of the container. This should in
			 * all cases be equivalent to the number of objects in the
			 * container, but this is not guaranteed.
			 */
			SizeType size() const { return mData.size(); }
			
		private:
			typedef std::vector<T*> DataVec;
			DataVec mData;
	};
	
	/**
	 * Simple utility class to associate string names with integer
	 * identifiers.
	 */
	class NameIdMgr {
		public:
			NameIdMgr() {}
			~NameIdMgr() {}
			
			/// Associates the given \a id with the given \a name.
			void connect(IdType id, const std::string &name)
			{
				mNameMap[name] = id;
				mIdMap[id] = name;
			}
			
			/// Returns the id corresponding to the given name
			IdType getIdOf(const std::string &name) const
			{
				NameMap::const_iterator i=mNameMap.find(name);
				return (i==mNameMap.end()) ? NoId : i->second;
			}
			
			/// Returns the name corresponding to the given id
			std::string getNameOf(IdType id) const
			{
				IdMap::const_iterator i=mIdMap.find(id);
				return (i==mIdMap.end()) ? "" : i->second;
			}
			
		private:
			typedef boost::unordered_map<std::string,IdType> NameMap;
			typedef boost::unordered_map<IdType, std::string> IdMap;
			NameMap mNameMap;
			IdMap mIdMap;
	};
	
	/**
	 * @brief Extended implementation of DataCtr
	 */
	template<class T>
	class DataCtrAdv : public DataCtr<T> {
		public:
			typedef T Type;
			
			/// Retrieves a database object by id
			T *getType(IdType id) { return DataCtr<T>::get(id); }
			/// @copydoc getType()
			const T *getType(IdType id) const { return DataCtr<T>::get(id); }
			
			/// Retrieves a database object by name
			T *getType(const std::string &name)
			{ return getType(mNameIdMgr.getIdOf(name)); }
			/// @copydoc getType(const std::string&)
			const T *getType(const std::string &name) const
			{ return getType(mNameIdMgr.getIdOf(name)); }
			
			/// @see DataCtr::size
			SizeType size() const { return DataCtr<T>::size(); }
			
			/// Returns the identifier associated with the given name
			IdType getIdOf(const std::string &name) const
			{ return mNameIdMgr.getIdOf(name); }
			
			/// Returns the name associated with the given identifier
			const std::string getNameOf(IdType id) const
			{ return mNameIdMgr.getNameOf(id); }
			
			/**
			 * Registers a new object to the database.
			 * 
			 * @param obj Object to insert, may be null.
			 * @param name Optional string to associate the object with a name.
			 * 
			 * @return The identifier given to the object
			 */
			IdType registerObj(T *obj, const std::string &name="") {
				IdType id = DataCtr<T>::add(obj);
				
				if(!name.empty())
					mNameIdMgr.connect(id,name);
				
				return id;
			}
			
			/**
			 * @brief Reseats a database object.
			 * 
			 * The previous database object is not destroyed. No error
			 * checking is performed, as the ID is assumed valid.
			 * 
			 * @return A pointer to the old object.
			 */
			T *reseatObj(IdType id, T *newObj) {
				T *oldObj = getType(id);
				
				DataCtr<T>::set(id,newObj);
				
				return oldObj;
			}
			
		protected:
			NameIdMgr mNameIdMgr;
	};
	
	/**
	 * Default implementation for databases tailored to the simulation.
	 * 
	 * In essence, this class defines an extended version of \c DataCtr.
	 * 
	 * A \c NameIdMgr is also included, providing the ability for data to
	 * associate a name with each identifier.
	 * 
	 * @param T Database object type.
	 */
	template<class T>
	class DefaultDatabase : public BaseDatabase, public DataCtrAdv<T> {
		public:
			DefaultDatabase() : mSim(0) {}
			virtual ~DefaultDatabase() {}
			
			virtual void startup(Simulation* sim)
			{ mSim = sim; }
			
			virtual void shutdown()
			{ DataCtr<T>::clear(); }
			
		protected:
			Simulation *mSim;
	};
	
	/// Empty struct for types without any shared metadata
	/// @internal
	struct SharedVoid {};
	
	/**
	 * @brief Describes a behaviour for a data type.
	 * 
	 * A behaviour is a collection of two, possibly three (depending on
	 * inheritance), components:
	 * - \c createObj() : Creates an object of the type that the database's
	 * 		associated simulation type requires (\c T). The object created is
	 * 		usually an inherited and specialized version of \c T.
	 * - \c SharedMeta : Metadata for \c T that all database objects of this
	 * 		type use.
	 * - Any class inheriting from \c Behaviour may add additional metadata
	 * 		specific to that type.
	 * 
	 * These components make up the static data for a simulation type.
	 * 
	 * @param T The type of the object that the simulation (within \c State)
	 * processes.
	 * @param SharedMeta A class representing additional shared metadata
	 * for this behaviour.
	 */
	template<class T, class SharedMeta=SharedVoid>
	class Behaviour : public SharedMeta {
		public:
			Behaviour(IdType id, const SharedMeta &meta) :
				SharedMeta(meta), mId(id) {}
			virtual ~Behaviour() {}
			
			/**
			 * Creates an object of the associated simulation type \c T.
			 * 
			 * The created type is usually a derivate of \c T.
			 * 
			 * @param sim Parent simulation of the created object.
			 * @param id Unique identifier for the created object.
			 */
			virtual T *createObj(Simulation *sim, IdType id) const=0;
			
			IdType getId() const { return mId; }
			void setId(IdType id) { mId=id; }
			
		private:
			IdType mId;
	};
	
	/**
	 * Extends \c DefaultDatabase with functionality tailored to
	 * \c Behaviour style data types.
	 * 
	 * @param T Base simulated class for behaviour (ex. \c Weapon).
	 * @param SharedMeta Shared metadata for behaviours
	 */
	template<class T, class SharedMeta=SharedVoid>
	class DefaultBehaviourDatabase :
	public DefaultDatabase< Behaviour<T,SharedMeta> > {
		public:
			DefaultBehaviourDatabase() {}
			virtual ~DefaultBehaviourDatabase() {}
			
			/**
			 * Registers an implementation type.
			 * 
			 * This automatically implements a \c Behaviour class for
			 * the type.
			 */
			template<class Impl>
			IdType registerImpl(const std::string &name="",
			const SharedMeta &meta=SharedMeta()) {
				// Allocate a new ID for the object
				IdType id = DefaultDatabase< Behaviour<T,SharedMeta> >::
					registerObj(0,name);
				
				// Create and insert the new object
				DefaultDatabase< Behaviour<T,SharedMeta> >::reseatObj(
					id, new BehaviourImpl<Impl>(id,meta));
				
				return id;
			}
			
		private:
			/**
			 * Automatically provides an implementation for object creation.
			 */
			template<class Impl>
			class BehaviourImpl : public Behaviour<T,SharedMeta> {
				public:
					BehaviourImpl(IdType id, const SharedMeta &meta) :
						Behaviour<T,SharedMeta>(id,meta) {}
					virtual ~BehaviourImpl() {}
					
					T *createObj(Simulation *sim, IdType id) const
					{ return new Impl(sim, id, Behaviour<T,SharedMeta>::getId(),
						typename Impl::Config()); }
			};
	};
}

#endif
