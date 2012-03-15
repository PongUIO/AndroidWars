#ifndef EXTSIM_EXTBASEDATA_H
#define EXTSIM_EXTBASEDATA_H

#include <vector>
#include <boost/lexical_cast.hpp>

#include "../dascript/script.h"
#include "../simulation/Common.h"
#include "../simulation/data/BaseData.h"

namespace ExtS {
	class ExtSim;
	
	/**
	 * Base virtual class for processing and storing data.
	 * 
	 */
	class ExtBaseData {
		public:
			ExtBaseData(ExtSim &sim) : mExtSim(sim) {}
			virtual ~ExtBaseData();
			
			virtual void startup()=0;
			virtual void shutdown()=0;
			
			virtual void loadBlock(Script::Block &block)=0;
			
			virtual void postProcess()=0;
			
		protected:
			ExtSim &mExtSim;
	};
	
	/**
	 * Default implementation for types who mirror simulation data.
	 */
	template<class T>
	class DefaultExtData : public Sim::DataCtr<T>, public ExtBaseData {
		public:
			typedef T DataType;
			
			DefaultExtData(ExtSim& extsim) : ExtBaseData(extsim)
			{}
			
			virtual ~DefaultExtData()
			{}
	};
}

#endif
