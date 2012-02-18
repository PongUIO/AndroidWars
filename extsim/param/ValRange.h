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
			
			virtual void readBlock(Script::Block *block) {
				Script::Block *paramBlock, *constraintBlock;
				
				paramBlock = block->getBlock("PARAM");
				constraintBlock = block->getBlock("CONSTRAINT");
				
				// Read default value
				if(paramBlock) {
					Script::Data &paramData = getBlockData(paramBlock);
					
					mVal = ExtData::readValue<T>(paramData.getArg(0), T());
				}
				
				// Read constraint
				Script::Data &constrData = getBlockData(constraintBlock);
				if(constraintBlock && constrData.isDefined()) {
					setDefinedConstraint();
					
					typedef std::vector<std::string> StringVec;
					StringVec rangeVec;
					
					for(size_t i=0; i<constrData.argCount(); ++i) {
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
			
			virtual bool isValid(RuleParameter* param, ExtSim& extsim) const {
				ValRange<T> *valSrc = static_cast<ValRange<T>*>(param);
				
				T val = valSrc->getVal();
				for(typename ValPairVec::const_iterator i=mValPairs.begin();
					i!=mValPairs.end(); ++i) {
					if(val >= (*i).first && val <= (*i).second)
						return true;
				}
				return false;
			}
			
			
			T getVal() const { return mVal; }
			const ValPairVec &getValPairs() const { return mValPairs; }
			
		private:
			// Value
			T mVal;
			
			// Constraint
			ValPairVec mValPairs;
	};
}

#endif
