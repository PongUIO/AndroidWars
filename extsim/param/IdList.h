#ifndef EXTSIM_IDLIST_H
#define EXTSIM_IDLIST_H

#include <boost/unordered_set.hpp>

#include "../ExtSim.h"
#include "../object/ParamList.h"
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
			
			IdList(const std::string& dataName) : RuleParameter(dataName) {}
			virtual ~IdList() {}
			
			RuleParameter *clone() { return new IdList<T>(*this); }
			
			void readNode(Nepeta::Node &node) {
				Nepeta::Node &paramNode = node.getNode("PARAM");
				Nepeta::Node &constraintNode = node.getNode("CONSTRAINT");
				
				// Read default value
				if(paramNode.isValid()) {
					Nepeta::Node &paramData = getNodeData(paramNode);
					mIdName = paramData.getArg(0);
				}
				
				// Read constraints
				Nepeta::Node &constraintData = getNodeData(constraintNode);
				if(constraintData.isValid()) {
					setConstraintDefined();
					
					for(size_t i=0, nc=constraintData.getNodeCount(); i<nc;++i)
						mIdNameVec.push_back(constraintData.getArg(i));
				}
			}
			
			void postProcess(ExtSim& extsim) {
				bool whiteList = true;
				IdSet deny;
				typename T::DatabaseType &db =
					Sim::getDataComponent<typename T::DatabaseType>(
					extsim.getSim());
				
				// Translate default value
				mId = db.getIdOf(mIdName);
				
				// Translate constraints
				for(StringVec::iterator i=mIdNameVec.begin();
				    i!=mIdNameVec.end(); ++i) {
					size_t len = i->length();
					
					if (i->at(0) == '+') {
						whiteList = true;
					} else if (i->at(0) == '-') {
						whiteList = false;
					} else if (i->at(len-1) == '*') {
						std::string candidate;
						
						// wildcard at the end of string,
						// we iterate over every name
						// and accept every completion
						for (Sim::IdType j = 0; j < db.size(); j++) {
							candidate = db.getNameOf(j);
							size_t cand_len = candidate.length();
							
							// candidate shorter than
							// possible completion or
							// doesn't fit description
							if (cand_len < len || candidate.substr(0, len-1) != *i)
								continue;
							
							whiteList ?
								mIdSet.insert(j) :
								deny.insert(j);
						}
					} else {
						Sim::IdType id = db.getIdOf(*i);
						
						whiteList ?
							mIdSet.insert(id) :
							deny.insert(id);
					}
				}
				mIdNameVec.clear();
				
				// filter through denied IdTypes
				for (IdSet::iterator i = deny.begin(); i != deny.end(); i++)
					mIdSet.erase(*i);
			}
		
			bool isConstrained(const RuleParameter* param, ExtSim& extsim) const {
				const IdList<T> *srcList = static_cast<const IdList<T>*>(param);
				
				return mIdSet.find(srcList->getId())!=mIdSet.end();
			}
			
			void callback()
			{ ListenerSlot<IdList<T> >::raiseListener(this); }
			
			void save(Sim::Save::BasePtr& fp) const {
				fp << mId;
				fp.writeUnorderedSet(mIdSet);
				//fp << mIsAlwaysValid;
			}
			void load(Sim::Save::BasePtr& fp) {
				fp >> mId;
				fp.readUnorderedSet(mIdSet);
				//fp >> mIsAlwaysValid;
			}
			
			Sim::IdType getId() const { return mId; }
			const IdSet &getIdSet() const { return mIdSet; }
			
		private:
			// Value
			std::string mIdName;
			Sim::IdType mId;
			
			// Constraint
			StringVec mIdNameVec;
			IdSet mIdSet;
	};
}

#endif
