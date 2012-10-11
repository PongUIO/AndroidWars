#ifndef EXTSIM_IDLIST_H
#define EXTSIM_IDLIST_H

#include <boost/unordered_set.hpp>

#include "../ExtSim.h"
#include "../object/ParamList.h"
#include "../../simulation/Common.h"

#include "../object/ParamVisitor.h"

namespace exts {
	/**
	 * Allows an identifier of an arbitrary type, constrained to a
	 * list of identifiers.
	 */
	template<class T>
	class IdParam : public RuleParameter {
		public:
			typedef std::vector<std::string> StringVec;
			typedef boost::unordered_set<Sim::IdType> IdSet;
			
			struct Value {
				void readNode(const Nepeta::Node &node)
				{ mIdName = node.getArg(0); }
				
				void postProcess(ExtSim &esim) {
					mId = Sim::getDataComponent<typename T::TypeDatabase>(
					esim.getSim()).getIdOf(mIdName);
					
					mIdName.clear();
				}
				
				void save(Sim::Save::BasePtr &fp) const	{ fp << mId; }
				void load(Sim::Save::BasePtr &fp)		{ fp >> mId; }
				
				std::string mIdName;
				Sim::IdType mId;
			};
			
			struct Constraint {
				Constraint(bool whitelist) : mWhitelist(whitelist) {}
				
				void readNode(const Nepeta::Node &node) {
					for(size_t i=0, nc=node.getNodeCount(); i<nc;++i)
						mIdNameVec.push_back(node.getArg(i));
				}
				
				void postProcess(ExtSim &esim) {
					typename T::TypeDatabase &db =
					Sim::getDataComponent<typename T::TypeDatabase>(
					esim.getSim());
					
					// Translate constraints
					bool doAdd = true;
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
				
				bool isConstrained(const IdParam<T> *param, ExtSim &esim)
				const {
					// If this is a blacklist, the parameter must not be in the set.
					// If this is a whitelist, the parameter must be in the set
					return (mIdSet.find(param->getId()) != mIdSet.end())
					== mWhitelist;
				}
				
				void save(Sim::Save::BasePtr &fp) const {
					fp << mWhitelist;
					fp.writeUnorderedSet(mIdSet);
				}
				void load(Sim::Save::BasePtr &fp) {
					fp >> mWhitelist;
					fp.readUnorderedSet(mIdSet);
				}
				
				bool mWhitelist;
				StringVec mIdNameVec;
				IdSet mIdSet;
			};
			
			IdParam(const std::string& dataName, bool whiteList=true) :
				RuleParameter(dataName),
				mValue(), mConstraint(whiteList)
			{}
			virtual ~IdParam() {}
			
			RuleParameter *clone() { return new IdParam<T>(*this); }
			
			void readNode(const Nepeta::Node &paramNode,
			const Nepeta::Node &constraintNode) {
				// Read default value
				if(paramNode.isValid()) {
					const Nepeta::Node &paramData = getNodeData(paramNode);
					mValue.readNode(paramData);
				}
				
				// Read constraints
				const Nepeta::Node &constraintData = getNodeData(constraintNode);
				if(constraintData.isValid()) {
					setConstraintDefined();
					mConstraint.readNode(constraintData);
				}
			}
			
			void postProcess(ExtSim& extsim) {
				mValue.postProcess(extsim);
				mConstraint.postProcess(extsim);
			}
		
			bool isConstrained(const RuleParameter* param,
			ExtSim& extsim) const {
				return mConstraint.isConstrained(
				static_cast<const IdParam<T>*>(param), extsim);
			}
			
			virtual void accept(ParamVisitor& visitor)
			{	visitor.visit(*this); }
			
			void save(Sim::Save::BasePtr& fp) const {
				mValue.save(fp);
				mConstraint.save(fp);
			}
			void load(Sim::Save::BasePtr& fp) {
				mValue.load(fp);
				mConstraint.load(fp);
			}
			
			void setId(Sim::IdType id) { mValue.mId=id; }
			Sim::IdType getId() const { return mValue.mId; }
			
			const IdSet &getIdSet() const { return mConstraint.mIdSet; }
			IdSet &getIdSet() { return mConstraint.mIdSet; }
			
			bool isWhitelist() const { return mConstraint.mWhitelist; }
			void setWhitelist(bool isWhite) { mConstraint.mWhitelist=isWhite; }
			
		private:
			Value mValue;
			Constraint mConstraint;
	};
}

#endif
