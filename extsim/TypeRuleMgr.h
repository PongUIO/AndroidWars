#ifndef EXTSIM_TYPERULEMGR_H
#define EXTSIM_TYPERULEMGR_H

#include "../simulation/data/BaseData.h"
#include "../simulation/Save.h"

#include "ExtModule.h"

namespace exts {
	class ExtSim;
	class TypeRule;
	class ParamList;
	
	/**
	 * @brief Stores and manages every \c TypeRule object in existence and their
	 * ID.
	 * 
	 * This manager handles all \c TypeRule objects created by other data
	 * components. While the data components store a pointer to the rule,
	 * all rules are most often referenced by their ID.
	 * 
	 * The reason for this manager is so that all \c TypeRule objects are
	 * associated with an unique ID, because this ID is equivalent for all
	 * \c ExtSim objects that generated from the same data set. If pointers
	 * were used, their unique value would be different for different \c ExtSim
	 * objects.
	 * 
	 * Destruction of \c TypeRule objects are done by this manager.
	 */
	class TypeRuleMgr : public ExtModule, private Sim::DataCtr<TypeRule> {
		public:
			/// @name Initialization
			//@{
				TypeRuleMgr(ExtSim &esim);
				~TypeRuleMgr();
				
				void startup();
				void shutdown();
			//@}
			
			/// @name Interface
			//@{
				const TypeRule *getRule(Sim::IdType id) const;
				Sim::IdType registerRule(TypeRule *rule);
				
				ParamList *loadParamList(Sim::Save::BasePtr &fp) const;
				void saveParamList(Sim::Save::BasePtr &fp,
					const ParamList *ref) const;
			//@}
			
		private:
			ExtSim &mExtSim;
	};
}

#endif
