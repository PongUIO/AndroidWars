#ifndef EXTSIM_EXTDATAOBJ_H
#define EXTSIM_EXTDATAOBJ_H

#include <string>

#include "nepeta.h"
#include "../../simulation/Common.h"

namespace exts {
	class ExtSim;
	
	class ExtDataObjBase {
		public:
			ExtDataObjBase(ExtSim &esim, Sim::IdType id) :
				mExtSim(esim), mId(id) {}
			~ExtDataObjBase() {}
			
			virtual void loadNode(const Nepeta::Node &node);
			virtual void postProcess();
			
			Sim::IdType getId() const { return mId; }
			
			const std::string &getName() const { return mName; }
			const std::string &getDescription() const { return mDescription; }
			
		protected:
			ExtSim &mExtSim;
			
		private:
			Sim::IdType mId;
			
			std::string mName;
			std::string mDescription;
	};
}

#endif
