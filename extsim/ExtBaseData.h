#ifndef EXTSIM_EXTBASEDATA_H
#define EXTSIM_EXTBASEDATA_H

#include <vector>
#include <boost/lexical_cast.hpp>

#include "../dascript/dascript.h"
#include "../simulation/Common.h"
#include "../simulation/data/BaseData.h"
#include "TypeRule.h"

namespace ExtS {
	class ExtSim;
	
	struct NodeListener {
		virtual void loadNode(DaScript::Node &node)=0;
		virtual void postProcess()=0;
	};
	
	/**
	 * Base virtual class for processing and storing data.
	 * 
	 */
	class ExtBaseData : public NodeListener {
		public:
			ExtBaseData(ExtSim &sim) : mExtSim(&sim) {}
			virtual ~ExtBaseData() {}
			
			virtual void startup()=0;
			virtual void shutdown()=0;
			
			virtual void loadNode(DaScript::Node &node)=0;
			virtual void postProcess()=0;
			
		protected:
			ExtSim *mExtSim;
	};
	
	/**
	 * Default implementation for types who provide extended type-specific
	 * behaviour for simulation data.
	 */
	template<class T>
	class DefaultExtData : public Sim::DefaultDatabase<T>, public ExtBaseData,
	public TypeRuleMgr {
		public:
			typedef T DataType;
			
			DefaultExtData(ExtSim& extsim) : ExtBaseData(extsim)
			{}
			
			virtual ~DefaultExtData()
			{}
			
			virtual void setupObject(DaScript::Node &node,
				TypeRule *rule, Sim::IdType typeId, T *obj) {
				obj->loadNode(node, typeId, rule);
			}
			
			virtual DaScript::Node &getTypeRuleNode(DaScript::Node &node)
			{ return node; }
			
			virtual void loadNode(DaScript::Node &node) {
				const std::string &name = node.getNodeFirst("Name");
				
				// Ignore data if it doesn't have a name
				if(name.empty())
					return;
				
				// Check if an object is already defined with this name
				T *extObj = 0;
				TypeRule *rule = 0;
				Sim::IdType id = Sim::DefaultDatabase<T>::getIdOf(name);
				
				// If no object has been allocated with this name:
				// - Find a rule (if rules are used)
				// - Create an object
				if(id == Sim::NoId) {
					// Ignore data if the block does not define any rule
					rule = loadRuleNode(getTypeRuleNode(node));
					if(!rule)
						return;
					
					id = Sim::DefaultDatabase<T>::registerObj(0,name);
					extObj = new T(mExtSim);
					Sim::DefaultDatabase<T>::reseatObj(id,extObj);
					
					Sim::IdType coreId = rule->registerSimData(*mExtSim,
						name);
					
					assert(id == coreId &&
						"Simulation data identifiers (coreId) must"
						"always be equivalent to ExtSim identifiers (id)");
				}
				// If an object already has been allocated with this name:
				// - Get the previously allocated object
				// (The new data is appended to this object)
				else {
					extObj = Sim::DefaultDatabase<T>::getType(id);
				}
				
				// Load data into this object
				setupObject(node, rule, id, extObj);
			}
			
			virtual void postProcess() {
				for(Sim::IdType i=0; i<Sim::DefaultDatabase<T>::size(); ++i) {
					Sim::DefaultDatabase<T>::getType(i)->postProcess(*mExtSim);
				}
			}
			
		private:
	};
	
	/**
	 * Includes common data and behaviour for an extended data type.
	 */
	class ExtBaseDataObj {
		public:
			ExtBaseDataObj(ExtSim *esim) : mExtSim(esim), mId(Sim::NoId),
				mRule(0), mName(), mDescription()
				{}
			~ExtBaseDataObj() {
				if(mRule)
					delete mRule;
			}
			
			virtual void loadNode(DaScript::Node &node,
				Sim::IdType simTypeId, TypeRule *rule);
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
	};
}

#endif
