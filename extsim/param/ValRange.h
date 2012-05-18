#ifndef EXTSIM_VALRANGE_H
#define EXTSIM_VALRANGE_H

#include "../TypeRule.h"
#include "../ExtData.h"

namespace ExtS {
	/**
	 * Allows an arbitrary value of a generic type, constrained
	 * to one or more value ranges.
	 */
	template<class T>
	class ValRange : public RuleParameter,
	public ListenerSlot< ValRange<T> > {
		public:
			typedef std::pair<T,T> ValPair;
			typedef std::vector<ValPair> ValPairVec;
			
			ValRange(const std::string& dataName) : RuleParameter(dataName) {}
			virtual ~ValRange() {}
			
			virtual RuleParameter *clone() { return new ValRange<T>(*this); }
			
			virtual void readNode(DaScript::Node &node) {
				DaScript::Node &paramNode = node.getNode("PARAM");
				DaScript::Node &constraintNode = node.getNode("CONSTRAINT");
				
				// Read default value
				if(paramNode.isValid()) {
					DaScript::Node &paramData = getNodeData(paramNode);
					
					mVal = ExtData::readValue<T>(paramData.getArg(0), T());
				}
				
				// Read constraint
				DaScript::Node &constrData = getNodeData(constraintNode);
				if(constrData.isValid()) {
					setDefinedConstraint();
					
					typedef std::vector<std::string> StringVec;
					StringVec rangeVec;
					
					for(size_t i=0, nc=constrData.getNodeCount(); i<nc; ++i) {
						StringVec rangeParam;
						boost::split(rangeParam, constrData.getArg(i),
							boost::is_any_of(">"),
							boost::algorithm::token_compress_on);
						
						ValPair valPair = std::make_pair<T,T>(T(),T());
						if(rangeParam.size()==1) {
							valPair.first = valPair.second =
							ExtData::readValue<T>(rangeParam.front(),0);
						} else if(rangeParam.size()>=2) {
							valPair.first = ExtData::readValue<T>(
								rangeParam.front(),0);
							valPair.second = ExtData::readValue<T>(
								rangeParam.back(),0);
						}
						
						mValPairs.push_back(valPair);
					}
				}
			}
			
			virtual void callback()
			{ ListenerSlot< ValRange<T> >::raiseListener(this); }
			
			virtual bool isValid(const RuleParameter* param, ExtSim& extsim) const {
				const ValRange<T> *valSrc = static_cast<const ValRange<T>*>(param);
				
				T val = valSrc->getVal();
				for(typename ValPairVec::const_iterator i=mValPairs.begin();
					i!=mValPairs.end(); ++i) {
					if(val >= (*i).first && val <= (*i).second)
						return true;
				}
				return false;
			}
			
			virtual void save(Sim::Save::BasePtr& fp) const {
				fp << mVal;
				
				fp << uint32_t(mValPairs.size());
				for(typename ValPairVec::const_iterator i=mValPairs.begin();
					i!=mValPairs.end(); ++i) {
					fp << i->first;
					fp << i->second;
				}
			}
			virtual void load(Sim::Save::BasePtr& fp) {
				fp >> mVal;
				
				mValPairs.clear();
				uint32_t count;
				fp >> count;
				
				mValPairs.resize(count);
				for(typename ValPairVec::iterator i=mValPairs.begin();
					i!=mValPairs.end(); ++i) {
					fp >> i->first;
					fp >> i->second;
				}
			}
			
			
			T getVal() const { return mVal; }
			void setVal(const T &val) { mVal=val; }
			const ValPairVec &getValPairs() const { return mValPairs; }
			
		private:
			// Value
			T mVal;
			
			// Constraint
			ValPairVec mValPairs;
	};
}

#endif
