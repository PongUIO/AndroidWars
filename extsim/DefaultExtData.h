#ifndef EXTSIM_DEFAULTEXTDATA_H
#define EXTSIM_DEFAULTEXTDATA_H

#include "nepeta.h"

#include "../simulation/data/BaseData.h"

#include "ExtDataComponent.h"
#include "CommonLoad.h"

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
			
			virtual void startup() {}
			virtual void shutdown()
			{ Sim::DataCtrAdv<T>::clear(); }
			
			/**
			 * @brief Loads a object of type \c T from a \c Nepeta::Node.
			 * 
			 * The object is stored in the internal \c Sim::DataCtrAdv.
			 */
			virtual void loadNode(const Nepeta::Node& node)
			{
				// Ignore data without name
				const std::string &name = node.getNodeFirst(Load::Name);
				if(name.empty())
					return;
				
				// Check if the object already exists
				T *extObj = 0;
				Sim::IdType id = Sim::DataCtrAdv<T>::getIdOf(name);
				
				// If it doesn't exist, create a new object
				if(id== Sim::NoId) {
					id = Sim::DataCtrAdv<T>::registerObj(0,name);
					extObj = new T(mExtSim,id);
					Sim::DataCtrAdv<T>::reseatObj(id,extObj);
				}
				// If the object already exists, reuse it
				else {
					extObj = Sim::DataCtrAdv<T>::getType(id);
				}
				
				// Load the object
				extObj->loadNode(node);
			}
			
			virtual void postProcess()
			{
				for(Sim::IdType i=0; i<Sim::DataCtrAdv<T>::size(); ++i) {
					Sim::DataCtrAdv<T>::getType(i)->postProcess();
				}
			}
			
		protected:
			ExtSim &mExtSim;
	};
}

#endif
