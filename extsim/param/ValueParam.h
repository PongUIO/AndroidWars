#ifndef EXTSIM_VALRANGE_H
#define EXTSIM_VALRANGE_H

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include "../object/ParamList.h"
#include "../util/TypeConv.h"

namespace exts {
	/**
	 * Allows an arbitrary value of a generic type, constrained
	 * to one or more value ranges.
	 */
	template<class T>
	class ValueParam : public RuleParameter,
	public ListenerSlot< ValueParam<T> > {
		public:
			typedef std::pair<T,T> ValPair;
			typedef std::vector<ValPair> ValPairVec;
			
			struct Value {
				Value(const T &val=T()) : mVal(val) {}
				
				void readNode(const Nepeta::Node &node) {
					mVal = convValue<T>(node.getArg(0), T());
				}
				
				void save(Sim::Save::BasePtr &fp) const	{ fp << mVal; }
				void load(Sim::Save::BasePtr &fp)		{ fp >> mVal; }
				
				T mVal;
			};
			
			struct Constraint {
				Constraint(bool whitelist) : mWhitelist(whitelist) {}
				
				void readNode(const Nepeta::Node &node) {
					typedef std::vector<std::string> StringVec;
					StringVec rangeVec;
					
					for(size_t i=0; i<node.getArgCount(); ++i) {
						StringVec rangeParam;
						boost::split(rangeParam, node.getArg(i),
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
				
				bool isConstrained(const Value &valp, ExtSim &esim) const {
					const T &val = valp.mVal;
					for(typename ValPairVec::const_iterator i
					=mValPairs.begin(); i!=mValPairs.end(); ++i) {
						if(val >= (*i).first && val <= (*i).second)
							return mWhitelist;
					}
					return !mWhitelist;
				}
				
				static void saveValPair(Sim::Save::BasePtr &fp, const ValPair &val)
				{	fp << val.first << val.second; }
				
				static void loadValPair(Sim::Save::BasePtr &fp, ValPair &val)
				{	fp >> val.first >> val.second; }
				
				void save(Sim::Save::BasePtr &fp) const {
					fp << mWhitelist;
					fp.writeCtrF(mValPairs, saveValPair);
				}
				
				void load(Sim::Save::BasePtr &fp) {
					fp >> mWhitelist;
					fp.readCtrF(mValPairs, loadValPair);
				}
				
				bool mWhitelist;
				ValPairVec mValPairs;
			};
			
			ValueParam(const std::string& dataName, bool whitelist=true) :
				RuleParameter(dataName), mValue(), mConstraint(whitelist) {}
			virtual ~ValueParam() {}
			
			virtual RuleParameter *clone() { return new ValueParam<T>(*this); }
			
			virtual void readNode(
				const Nepeta::Node &paramNode,
				const Nepeta::Node &constraintNode
 			) {
				// Read default value
				if(paramNode.isValid()) {
					const Nepeta::Node &paramData = getNodeData(paramNode);
					mValue.readNode(paramData);
				}
				
				// Read constraint
				const Nepeta::Node &constrData = getNodeData(constraintNode);
				if(constraintNode.isValid() && constrData.isValid()) {
					setConstraintDefined();
					mConstraint.readNode(constrData);
				}
			}
			
			virtual void callback()
			{ ListenerSlot< ValueParam<T> >::raiseListener(this); }
			virtual void clearListener()
			{ ListenerSlot< ValueParam<T> >::clearListener(); }
			
			virtual bool isConstrained(const RuleParameter* param, ExtSim& esim) const {
				const ValueParam<T> *valSrc = static_cast<const ValueParam<T>*>(param);
				return mConstraint.isConstrained(valSrc->mValue, esim);
			}
			
			virtual void save(Sim::Save::BasePtr& fp) const {
				mValue.save(fp);
				mConstraint.save(fp);
			}
			virtual void load(Sim::Save::BasePtr& fp) {
				mValue.load(fp);
				mConstraint.load(fp);
			}
			
			T getVal() const { return mValue.mVal; }
			void setVal(const T &val) { mValue.mVal=val; }
			
			const ValPairVec &getValPairs() const { return mConstraint.mValPairs; }
			ValPairVec &getValPairs() { return mConstraint.mValPairs; }
			
		private:
			Value mValue;
			Constraint mConstraint;
	};
}

#endif
