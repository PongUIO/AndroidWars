#ifndef SIM_BASEDATA_H
#define SIM_BASEDATA_H

#include <boost/unordered_map.hpp>
#include <vector>
#include <string>
#include <stdint.h>

namespace Sim {
	class Simulation;
	
	class BaseData {
		public:
			BaseData() {}
			virtual ~BaseData() {}
			
			virtual void startup(Simulation *sim)=0;
			virtual void shutdown()=0;
		
	};
	
	/**
	 * Implements a basic database system.
	 * Registers new types by an ID.
	 */
	template<typename T, typename IdType=uint32_t>
	class DataT : public BaseData {
		public:
			DataT() {}
			virtual ~DataT() { clearData(); }
			
			virtual void startup(Simulation *sim)
			{ mSim = sim; }
			
			virtual void shutdown()
			{ clearData(); }
			
			const T *getType(IdType type) const
			{ return mData.at(type); }
			
			T *createType() {
				T *data = new T();
				IdType id = addType(data);
				data->mId = id;
				return data;
			}
			
			size_t debugSize() { return mData.size(); }
			
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
			Simulation *mSim;
	};
	
	/**
	 * Implements a database for behaviour implementation classes.
	 * 
	 * @param T Base class for behaviour.
	 * @param Impl Class that implements behaviour, needs getTypeName().
	 */
	template<class T>
	class DataBehaviourT : public BaseData {
		public:
			/**
			 * This class implements the function that is used to create
			 * the behaviour class.
			 */
			class Behaviour {
				public:
					virtual T *createObj(Simulation *sim, uint32_t id) const=0;
					uint32_t mId;
			};
			
			DataBehaviourT() : mInternal(), mTypeMap() {}
			virtual ~DataBehaviourT() {}
			
			void startup(Simulation* sim) { mInternal.startup(sim); }
			void shutdown() { mInternal.shutdown(); }
			
			const Behaviour *getType(uint32_t type) const
			{	return mInternal.getType(type); }
			
			const Behaviour *getType(const std::string &name) const
			{	return mInternal.getType(getTypeIdOf(name)); }
			
			
			/**
			 * Registers an implementation type.
			 * 
			 * This automatically implements a \c Behaviour class for
			 * the type. Suitable for implementations with only one
			 * interpretation.
			 */
			template<class Impl>
			uint32_t registerType() {
				return registerCustom(new BehaviourImpl<Impl>(),
					Impl::getTypeName());
			}
			
			/**
			 * Registers a implemented behaviour class.
			 * 
			 * @note Use this if the base class has several interpretations
			 * (such as for scripted behaviour using only one implemented class)
			 * 
			 * @warning At present this should not be used, as types using this
			 * rely on Impl::getTypeName(). This will be fixed soon.
			 */
			uint32_t registerCustom(Behaviour *type, const std::string &name) {
				uint32_t id = mInternal.addTypeExt(type);
				type->mId = id;
				
				mTypeMap[name] = id;
				mIdMap[id] = name;
				
				return id;
			}
			
			static uint32_t NoId() { return -1; }
			uint32_t getTypeIdOf(const std::string &type) const {
				TypeMap::const_iterator i=mTypeMap.find(type);
				return (i==mTypeMap.end()) ? NoId() : i->second;
			}
			
			const std::string &getTypeOf(uint32_t id) const {
				IdMap::const_iterator i=mIdMap.find(id);
				return (i==mIdMap.end()) ? "" : i->second;
			}
			
		private:
			/**
			 * Automatically provides an implementation for object creation.
			 */
			template<class Impl>
			class BehaviourImpl : public Behaviour {
				public:
					T *createObj(Simulation *sim, uint32_t id) const
					{ return new Impl(sim, id, Behaviour::mId,
						typename Impl::Config()); }
			};
			
			class InternalDatabase : public DataT<Behaviour> {
				public:
					uint32_t addTypeExt(Behaviour *b) { addType(b); }
					
			};
			
			InternalDatabase mInternal;
			
			typedef boost::unordered_map<std::string,uint32_t> TypeMap;
			typedef boost::unordered_map<uint32_t, std::string> IdMap;
			TypeMap mTypeMap;
			IdMap mIdMap;
	};
}

#endif
