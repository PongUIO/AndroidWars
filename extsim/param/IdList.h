#ifndef EXTSIM_IDLIST_H
#define EXTSIM_IDLIST_H

#include <boost/unordered_set.hpp>

#include "../ExtSim.h"
#include "../object/ParamList.h"
#include "../../simulation/Common.h"

namespace exts {
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
			
			IdList(const std::string& dataName, bool whiteList=true) :
				RuleParameter(dataName), mWhitelist(whiteList) {}
			virtual ~IdList() {}
			
			RuleParameter *clone() { return new IdList<T>(*this); }
			
			void readNode(const Nepeta::Node &paramNode,
			const Nepeta::Node &constraintNode) {
				// Read default value
				if(paramNode.isValid()) {
					const Nepeta::Node &paramData = getNodeData(paramNode);
					mIdName = paramData.getArg(0);
				}
				
				// Read constraints
				const Nepeta::Node &constraintData = getNodeData(constraintNode);
				if(constraintData.isValid()) {
					setConstraintDefined();
					
					for(size_t i=0, nc=constraintData.getNodeCount(); i<nc;++i)
						mIdNameVec.push_back(constraintData.getArg(i));
				}
			}
			
			void postProcess(ExtSim& extsim) {
				typename T::TypeDatabase &db =
					Sim::getDataComponent<typename T::TypeDatabase>(
					extsim.getSim());
				
				// Translate default value
				mId = db.getIdOf(mIdName);
				
				// Translate constraints
				bool doAdd = true; // True to add to the set, false to remove
				for(StringVec::iterator i=mIdNameVec.begin();
				    i!=mIdNameVec.end(); ++i) {
					size_t len = i->length();
					
					if (i->at(0) == '+') {
						doAdd = true;
					} else if (i->at(0) == '-') {
						doAdd = false;
					} else if (i->at(len-1) == '*') {
						std::string candidate;
						
						// wildcard at the end of string,
						// we iterate over every name
						// and accept every completion
						for (Sim::IdType j = 0; j < db.size(); j++) {
							candidate = db.getNameOf(j);
							size_t cand_len = candidate.size();
							
							// candidate shorter than
							// possible completion or
							// doesn't fit description
							if (cand_len < len || candidate.substr(0, len-1) != *i)
								continue;
							
							if(doAdd)
								mIdSet.insert(j);
							else
								mIdSet.erase(j);
						}
					} else {
						Sim::IdType id = db.getIdOf(*i);
						
						if(doAdd)
							mIdSet.insert(id);
						else
							mIdSet.erase(id);
					}
				}
				mIdNameVec.clear();
			}
		
			bool isConstrained(const RuleParameter* param,
			ExtSim& extsim) const {
				const IdList<T> *srcList = static_cast<const IdList<T>*>(param);
				
				// If this is a blacklist, the parameter must not be in the set.
				// If this is a whitelist, the parameter must be in the set
				return (mIdSet.find(srcList->getId()) != mIdSet.end())
					== mWhitelist;
			}
			
			void callback()
			{ ListenerSlot<IdList<T> >::raiseListener(this); }
			void clearListener()
			{ ListenerSlot<IdList<T> >::clearListener(); }
			
			void save(Sim::Save::BasePtr& fp) const {
				fp << mId;
				fp << mWhitelist;
				fp.writeUnorderedSet(mIdSet);
			}
			void load(Sim::Save::BasePtr& fp) {
				fp >> mId;
				fp >> mWhitelist;
				fp.readUnorderedSet(mIdSet);
			}
			
			void setId(Sim::IdType id) { mId=id; }
			Sim::IdType getId() const { return mId; }
			
			const IdSet &getIdSet() const { return mIdSet; }
			IdSet &getIdSet() { return mIdSet; }
			
			bool isWhitelist() const { return mWhitelist; }
			void setWhitelist(bool isWhite) { mWhitelist=isWhite; }
			
		private:
			// Value
			std::string mIdName;
			Sim::IdType mId;
			
			// Constraint
			bool mWhitelist;
			StringVec mIdNameVec;
			IdSet mIdSet;
	};
}

#endif
