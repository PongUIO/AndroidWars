#ifndef SIM_BASEDATA_H
#define SIM_BASEDATA_H

#include <boost/unordered_map.hpp>
#include <vector>
#include <string>
#include <stdint.h>

#include "../Common.h"

namespace Sim {
	class Simulation;
	
	class BaseData {
		public:
			BaseData() {}
			virtual ~BaseData() {}
			
			virtual void startup(Simulation *sim)=0;
			virtual void shutdown()=0;
		
	};
	
	template<typename T>
	class DataCtr {
		public:
			DataCtr() {}
			~DataCtr() { clearData(); }
			
			T *createType() {
				T *data = new T();
				IdType id = addType(data);
				data->mId = id;
				return data;
			}
			
			const T *getDataById(IdType id) const
			{ return (id>=mData.size()) ? 0 : mData[id]; }
			
			size_t size() const { return mData.size(); }
			
		protected:
			IdType addType(T *type)
			{ mData.push_back(type); return IdType(mData.size()-1); }
			
			T *rawGet(IdType type)
			{ return mData.at(type); }
			
			void clearData()
			{
				for(typename DataVec::iterator i=mData.begin();
					i!=mData.end(); ++i)
					delete *i;
				
				mData.clear();
			}
			
			typedef std::vector<T*> DataVec;
			DataVec mData;
	};
	
	/**
	 * Implements a basic database system.
	 * Registers new types by an ID.
	 */
	template<typename T>
	class DataT : public DataCtr<T>, public BaseData {
		public:
			DataT() : mSim(0) {}
			virtual ~DataT() {}
			
			virtual void startup(Simulation *sim)
			{ mSim = sim; }
			
			virtual void shutdown()
			{ DataCtr<T>::clearData(); }
			
		protected:
			Simulation *mSim;
	};
	
	/**
	 * Simple utility class to associate string names with integer ids.
	 */
	class NameIdMgr {
		public:
			NameIdMgr() {}
			~NameIdMgr() {}
			
			void connect(IdType id, const std::string &name)
			{
				mNameMap[name] = id;
				mIdMap[id] = name;
			}
			
			IdType getIdOf(const std::string &name) const
			{
				NameMap::const_iterator i=mNameMap.find(name);
				return (i==mNameMap.end()) ? NoId : i->second;
			}
			
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
	 * This class implements the function that is used to create
	 * the behaviour class.
	 */
	template<class T>
	class BehaviourT {
		public:
			virtual T *createObj(Simulation *sim, IdType id) const=0;
			IdType mId;
	};
	
	/**
	 * Implements a database for behaviour implementation classes.
	 * 
	 * @param T Base class for behaviour.
	 * @param Impl Class that implements behaviour, needs getTypeName().
	 */
	template<class T, class Behaviour=BehaviourT<T> >
	class DataBehaviourT : public BaseData {
		public:
			DataBehaviourT() : mInternal(), mNameIdMgr() {}
			virtual ~DataBehaviourT() {}
			
			void startup(Simulation* sim) { mInternal.startup(sim); }
			void shutdown() { mInternal.shutdown(); }
			
			const Behaviour *getType(IdType type) const
			{	return mInternal.getDataById(type); }
			
			const Behaviour *getType(const std::string &name) const
			{	return mInternal.getDataById(getIdOf(name)); }
			
			
			/**
			 * Registers an implementation type.
			 * 
			 * This automatically implements a \c Behaviour class for
			 * the type. Suitable for implementations with only one
			 * interpretation.
			 */
			template<class Impl>
			IdType registerType() {
				return registerCustom(new BehaviourImpl<Impl>(),
					Impl::getTypeName());
			}
			
			/**
			 * Registers a implemented behaviour class.
			 * 
			 * @note Use this if the base class has several interpretations
			 * (such as for scripted behaviour using only one implemented class)
			 */
			IdType registerCustom(Behaviour *type, const std::string &name) {
				IdType id = mInternal.addTypeExt(type);
				type->mId = id;
				
				mNameIdMgr.connect(id, name);
				
				return id;
			}
			
			IdType getIdOf(const std::string &name) const
			{ return mNameIdMgr.getIdOf(name); }
			
			const std::string getNameOf(IdType id) const
			{ return mNameIdMgr.getNameOf(id); }
			
			IdType size() const
			{ return mInternal.size(); }
			
		private:
			/**
			 * Automatically provides an implementation for object creation.
			 */
			template<class Impl>
			class BehaviourImpl : public Behaviour {
				public:
					T *createObj(Simulation *sim, IdType id) const
					{ return new Impl(sim, id, Behaviour::mId,
						typename Impl::Config()); }
			};
			
			class InternalDatabase : public DataT<Behaviour> {
				public:
					IdType addTypeExt(Behaviour *b)
					{ return addType(b); }
					
			};
			
			InternalDatabase mInternal;
			NameIdMgr mNameIdMgr;
	};
}

#endif
