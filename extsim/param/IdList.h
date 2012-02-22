#ifndef EXTSIM_IDLIST_H
#define EXTSIM_IDLIST_H

#include <boost/unordered_set.hpp>

#include "../ExtSim.h"
#include "../TypeRule.h"
#include "../../simulation/Common.h"

namespace ExtS {
	/**
	 * Allows an identifier of an arbitrary type, constrained to a
	 * list of identifiers.
	 */
	template<class T>
	class IdList : public RuleParameter,
	public ListenerSlot<IdList<T> > {
		public:
			typedef std::vector<std::string> StringVec;
			typedef boost::unordered_set<Sim::IdType> IdSet;
			
			IdList(const std::string& dataName) : RuleParameter(dataName),
				mIsAlwaysValid(false) {}
			virtual ~IdList() {}
			
			RuleParameter *clone() { return new IdList<T>(*this); }
			
			void readBlock(Script::Block* block) {
				Script::Block *paramBlock, *constraintBlock;
				
				paramBlock = block->getBlock("PARAM");
				constraintBlock = block->getBlock("CONSTRAINT");
				
				// Read default value
				if(paramBlock) {
					Script::Data &paramData = getBlockData(paramBlock);
					mIdName = paramData.getArg(0);
				}
				
				// Read constraints
				Script::Data &constraintData = getBlockData(constraintBlock);
				if(constraintBlock && constraintData.isDefined()) {
					setDefinedConstraint();
					
					for(size_t i=0; i<constraintData.argCount(); ++i)
						mIdNameVec.push_back(constraintData.getArg(i));
				}
			}
			
			void postProcess(ExtSim& extsim) {
				typename T::DatabaseType &db =
					Sim::getDataComponent<typename T::DatabaseType>(
					extsim.getSim());
				
				// Translate default value
				mId = db.getIdOf(mIdName);
				
				// Translate constraints
				for(StringVec::iterator i=mIdNameVec.begin();
					i!=mIdNameVec.end(); ++i) {
					if(*i == "*")
						mIsAlwaysValid=true;
					else {
						Sim::IdType id = db.getIdOf(*i);
						mIdSet.insert(id);
					}
				}
				mIdNameVec.clear();
			}
			
			bool isValid(RuleParameter* param, ExtSim& extsim) const {
				IdList<T> *srcList = static_cast<IdList<T>*>(param);
				
				return mIsAlwaysValid ||
					mIdSet.find(srcList->getId())!=mIdSet.end();
			}
			
			void callback()
			{ ListenerSlot<IdList<T> >::raiseListener(this); }
			
			Sim::IdType getId() const { return mId; }
			const IdSet &getIdSet() const { return mIdSet; }
			bool isAlwaysValid() const { return mIsAlwaysValid; }
			
		private:
			// Value
			std::string mIdName;
			Sim::IdType mId;
			
			// Constraint
			StringVec mIdNameVec;
			IdSet mIdSet;
			bool mIsAlwaysValid;
	};
}

#endif
