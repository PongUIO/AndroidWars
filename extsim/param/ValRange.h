#ifndef EXTSIM_VALRANGE_H
#define EXTSIM_VALRANGE_H

#include "../TypeRule.h"
#include "../ExtData.h"

namespace ExtS {
	template<class T>
	class ValueRangeP : public Param, public ListenerSlot<ValueRangeP<T> > {
		public:
			ValueRangeP(MetaParam* parent) : Param(parent), mVal(0) {}
			
			void readParam(Script::Data& data)
			{ mVal = ExtData::readValue<T>(data.getArg(0), 0); }
			
			void postProcess(ExtSim&) {}
			void callback()
			{ ListenerSlot<ValueRangeP<T> >::raiseListener(this); }
			
			T getVal() { return mVal; }
			
		private:
			T mVal;
	};
	
	template<class T>
	class ValueRangeC : public Constraint,
	public ListenerSlot<ValueRangeC<T> > {
		public:
			ValueRangeC(MetaParam* parent) : Constraint(parent) {}
			
			void readConstraint(Script::Data& data) {
				typedef std::vector<std::string> StringVec;
				StringVec rangeVec;
				
				boost::split(rangeVec, data.firstArg(), boost::is_any_of(",;"),
					boost::algorithm::token_compress_on);
				
				for(StringVec::iterator i=rangeVec.begin(); i!=rangeVec.end(); ++i) {
					StringVec rangeParam;
					boost::split(rangeParam, *i, boost::is_any_of(">"),
						boost::algorithm::token_compress_on);
					
					ValPair valPair = std::make_pair<T,T>(T(),T());
					if(rangeParam.size()==1) {
						valPair.first = valPair.second =
						ExtData::readValue<T>(rangeParam.front(),0);
					} else if(rangeParam.size()>=2) {
						valPair.first = ExtData::readValue<T>(rangeParam.front(),0);
						valPair.second = ExtData::readValue<T>(rangeParam.back(),0);
					}
					
					mValPairs.push_back(valPair);
				}
			}
			
			void postProcess(ExtSim&) {}
			
			bool isValid(Param* param, ExtSim& extsim) const {
				ValueRangeP<T> *pValRange =
					static_cast<ValueRangeP<T>*>(param);
				
				T val = pValRange->getVal();
				for(typename ValPairVec::const_iterator i=mValPairs.begin();
					i!=mValPairs.end(); ++i) {
					if(val >= (*i).first && val <= (*i).second)
						return true;
				}
				
				return false;
			}
			
			void callback()
			{ ListenerSlot<ValueRangeC<T> >::raiseListener(this); }
			
		private:
			typedef std::pair<T,T> ValPair;
			typedef std::vector<ValPair> ValPairVec;
			
			ValPairVec mValPairs;
	};
	
	template<class T>
	class MetaValRange : public MetaParam {
		public:
			MetaValRange(const std::string& dataName) : MetaParam(dataName),
				mDefault(this), mConstraint(this) {}
			virtual ~MetaValRange() {}
			
			Param* constructParam() { return new ValueRangeP<T>(mDefault); }
			MetaParam* clone() { return new MetaValRange<T>(*this); }

			void readParam(Script::Data& data) { mDefault.readParam(data); }
			void readConstraint(Script::Data& data)
			{ mConstraint.readConstraint(data); }
			
			Param* getDefaultParam() { return &mDefault; }
			Constraint* getDefaultConstraint() { return &mConstraint; }
			
		private:
			ValueRangeP<T> mDefault;
			ValueRangeC<T> mConstraint;
	};
}

#endif
