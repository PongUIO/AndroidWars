#ifndef EXTSIM_DEFAULTEXTDATA_H
#define EXTSIM_DEFAULTEXTDATA_H

#include "nepeta.h"

#include "../simulation/data/BaseData.h"

#include "ExtDataComponent.h"

namespace ExtS {
	class ExtSim;
	
	/**
	 * @brief Implements the common interface for \c ExtData databases
	 * 
	 * All extensions on this are either implemented as extended classes or
	 * modules added on top of this class.
	 */
	template<class T>
	class DefaultExtData : public ExtDataComponent, public Sim::DataCtrAdv<T> {
		public:
			DefaultExtData(ExtSim &extsim) : mExtSim(extsim) {}
			virtual ~DefaultExtData() {}
			
			virtual void startup();
			virtual void shutdown()
			{ Sim::DataCtrAdv<T>::clear(); }
			
			virtual void loadNode(const Nepeta::Node& node);
			virtual void postProcess();
			
		protected:
			ExtSim &mExtSim;
	};
}

#endif
