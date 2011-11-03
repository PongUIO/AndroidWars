#ifndef SIM_BASEDATA_H
#define SIM_BASEDATA_H

#include <vector>
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
	
	template<typename T, typename IdType=uint32_t>
	class DataT : public BaseData {
		public:
			DataT() {}
			virtual ~DataT() {}
			
			virtual void startup(Simulation *sim)
			{ mSim = sim; }
			
			virtual void shutdown()
			{}
			
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
			}
			
			typedef std::vector<T*> DataVec;
			
			DataVec mData;
			Simulation *mSim;
	};
}

#endif
