#ifndef EXTSIM_VALRANGE_H
#define EXTSIM_VALRANGE_H

#include "../object/ParamList.h"
#include "../util/TypeConv.h"

namespace exts {
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
			
			ValRange(const std::string& dataName, bool whitelist=true) :
				RuleParameter(dataName), mWhitelist(whitelist) {}
			virtual ~ValRange() {}
			
			virtual RuleParameter *clone() { return new ValRange<T>(*this); }
			
			virtual void readNode(
				const Nepeta::Node &paramNode,
				const Nepeta::Node &constraintNode
 			) {
				// Read default value
				if(paramNode.isValid()) {
					const Nepeta::Node &paramData = getNodeData(paramNode);
					
					mVal = convValue<T>(paramData.getArg(0), T());
				}
				
				// Read constraint
				const Nepeta::Node &constrData = getNodeData(constraintNode);
				if(constrData.isValid()) {
					setConstraintDefined();
					
					typedef std::vector<std::string> StringVec;
					StringVec rangeVec;
					
					for(size_t i=0; i<constrData.getArgCount(); ++i) {
						StringVec rangeParam;
						boost::split(rangeParam, constrData.getArg(i),
							boost::is_any_of(">"),
							boost::algorithm::token_compress_on);
						
						ValPair valPair = std::make_pair<T,T>(T(),T());
						if(rangeParam.size()==1) {
							valPair.first = valPair.second =
							convValue<T>(rangeParam.front(),0);
						} else if(rangeParam.size()>=2) {
							valPair.first = convValue<T>(
								rangeParam.front(),0);
							valPair.second = convValue<T>(
								rangeParam.back(),0);
						}
						
						mValPairs.push_back(valPair);
					}
				}
			}
			
			virtual void callback()
			{ ListenerSlot< ValRange<T> >::raiseListener(this); }
			virtual void clearListener()
			{ ListenerSlot< ValRange<T> >::clearListener(); }
			
			virtual bool isConstrained(const RuleParameter* param, ExtSim& extsim) const {
				const ValRange<T> *valSrc = static_cast<const ValRange<T>*>(param);
				
				T val = valSrc->getVal();
				for(typename ValPairVec::const_iterator i=mValPairs.begin();
					i!=mValPairs.end(); ++i) {
					if(val >= (*i).first && val <= (*i).second)
						return mWhitelist;
				}
				return !mWhitelist;
			}
			
			virtual void save(Sim::Save::BasePtr& fp) const {
				fp << mVal;
				fp << mWhitelist;
				
				fp << uint32_t(mValPairs.size());
				for(typename ValPairVec::const_iterator i=mValPairs.begin();
					i!=mValPairs.end(); ++i) {
					fp << i->first;
					fp << i->second;
				}
			}
			virtual void load(Sim::Save::BasePtr& fp) {
				fp >> mVal;
				fp >> mWhitelist;
				
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
			ValPairVec &getValPairs() { return mValPairs; }
			
		private:
			// Value
			T mVal;
			
			// Constraint
			bool mWhitelist;
			ValPairVec mValPairs;
	};
}

#endif
