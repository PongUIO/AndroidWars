#ifndef EXTSIM_EXTBASEDATA_H
#define EXTSIM_EXTBASEDATA_H

#include <vector>
#include <boost/lexical_cast.hpp>

#include "../dascript/script.h"
#include "../simulation/Common.h"
#include "../simulation/data/BaseData.h"
#include "TypeRule.h"

namespace ExtS {
	class ExtSim;
	
	/**
	 * Base virtual class for processing and storing data.
	 * 
	 */
	class ExtBaseData {
		public:
			ExtBaseData(ExtSim &sim) : mExtSim(&sim) {}
			virtual ~ExtBaseData() {}
			
			virtual void startup()=0;
			virtual void shutdown()=0;
			
			virtual void loadBlock(Script::Block &block)=0;
			
			virtual void postProcess()=0;
			
		protected:
			ExtSim *mExtSim;
	};
	
	/**
	 * Default implementation for types who provide extended type-specific
	 * behaviour for simulation data.
	 */
	template<class T>
	class DefaultExtData : public Sim::DataCtr<T>, public ExtBaseData,
	public TypeRuleMgr {
		public:
			typedef T DataType;
			
			DefaultExtData(ExtSim& extsim) : ExtBaseData(extsim),
				mIgnoreRule(false)
			{}
			
			virtual ~DefaultExtData()
			{}
			
			virtual void setupObject(Script::Block &block,
				TypeRule *rule, T *obj) {
				obj->loadBlock(block, rule);
			}
			
			virtual Script::Block &getTypeRuleBlock(Script::Block &block)
			{ return block; }
			
			virtual void loadBlock(Script::Block& block) {
				const std::string &name = block.getDataFirst("Name");
				
				// Ignore data if it doesn't have a name
				if(name.empty())
					return;
				
				// Check if an object is already defined with this name
				T *extObj = 0;
				TypeRule *rule = 0;
				Sim::IdType id = getIdOf(name);
				
				// If no object has been allocated with this name:
				// - Find a rule (if rules are used)
				// - Create an object
				if(id == Sim::NoId) {
					// Ignore data if the block does not define any rule
					rule = loadRuleBlock(getTypeRuleBlock(block));
					if(!rule && !mIgnoreRule)
						return;
					
					extObj = Sim::DataCtr<T>::createType();
					mNameIdMgr.connect(extObj->getId(), name);
				}
				// If an object already has been allocated with this name:
				// - Get the previously allocated object
				// (The new data is appended to this object)
				else {
					extObj = Sim::DataCtr<T>::rawGet(id);
				}
				
				// Load data into this object
				extObj->mExtSim = mExtSim;
				setupObject(block, rule, extObj);
			}
			
			virtual void postProcess() {
				for(typename Sim::DataCtr<T>::DataVec::iterator i=
					Sim::DataCtr<T>::mData.begin();
					i!=Sim::DataCtr<T>::mData.end(); ++i) {
					(*i)->postProcess(*mExtSim);
				}
			}
			
			Sim::IdType getIdOf(const std::string &name) const
			{ return mNameIdMgr.getIdOf(name); }
			std::string getNameOf(Sim::IdType id) const
			{ return mNameIdMgr.getNameOf(id); }
			
		protected:
			bool mIgnoreRule; ///< Minor work-around for \c ExtBotData
			
		private:
			Sim::NameIdMgr mNameIdMgr;
	};
	
	/**
	 * Includes common data and behaviour for an extended data type.
	 */
	class ExtBaseDataObj {
		public:
			ExtBaseDataObj() : mExtSim(0), mId(Sim::NoId), mRule(0),
				mName(), mDescription()
				{}
			~ExtBaseDataObj() {
				if(mRule)
					delete mRule;
			}
			
			virtual void loadBlock(Script::Block &block, TypeRule *rule);
			virtual void postProcess(ExtSim &extsim);
			
			Sim::IdType getId() const { return mId; }
			const TypeRule *getRule() const { return mRule; }
			
			const std::string &getName() const { return mName; }
			const std::string &getDescription() const { return mDescription; }
			
		protected:
			ExtSim *mExtSim;
			
			Sim::IdType mId;
			TypeRule *mRule;
			
			std::string mName;
			std::string mDescription;
			
			template<class T>
			friend class Sim::DataCtr;
			template<class T>
			friend class DefaultExtData;
	};
}

#endif
