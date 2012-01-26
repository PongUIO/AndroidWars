#ifndef EXTSIM_BASEDATA_H
#define EXTSIM_BASEDATA_H

#include <vector>
#include <boost/lexical_cast.hpp>

#include "../dascript/script.h"
#include "../simulation/Common.h"
#include "../simulation/data/BaseData.h"

namespace Sim {
	class Simulation;
}

namespace ExtS {
	/**
	 * Base virtual class for processing and storing data.
	 * 
	 */
	class BaseData {
		public:
			BaseData(Sim::Simulation &sim) : mSim(sim) {}
			virtual ~BaseData();
			
			virtual void startup()=0;
			virtual void shutdown()=0;
			
			virtual void loadBlock(Script::Block &block)=0;
			
			virtual void postProcess()=0;
			
		protected:
			template<class T>
			T badCastStrategy(const std::string &str, T def) { return def; }
			
			template<class T>
			T readValue(const std::string &str, T def=T())
			{
				T val = def;
				if(str.empty())
					return def;
				
				try { val = boost::lexical_cast<T>(str); }
				catch(boost::bad_lexical_cast &)
				{ val=badCastStrategy<T>(str,def); }
				
				return val;
			}
			
			Sim::Simulation &mSim;
	};
	
	/**
	 * Default implementation for types who mirror simulation data.
	 */
	template<class T>
	class DefaultData : public Sim::DataCtr<T>, public BaseData {
		public:
			DefaultData(Sim::Simulation& sim) : BaseData(sim)
			{}
			
			virtual ~DefaultData()
			{}
	};
}

#endif
