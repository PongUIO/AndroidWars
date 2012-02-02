#ifndef EXTSIM_IDLIST_H
#define EXTSIM_IDLIST_H

#include <boost/unordered_set.hpp>

#include "../TypeRule.h"
#include "../../simulation/Common.h"

namespace Sim {
	class ArmorD;
}

namespace ExtS {
	template<class T>
	Sim::IdType translateNameToId(ExtSim &extsim, const std::string &str);
	
	template<class T>
	class IdListP : public Param, public ListenerSlot<IdListP<T> > {
		public:
			IdListP(MetaParam* parent) : Param(parent), mId(0) {}
			
			void readParam(Script::Data& data)
			{ mName = data.getArg(0); }
			void postProcess(ExtSim& extsim)
			{ mId = translateNameToId<T>(extsim, mName); }
			
			void callback()
			{ ListenerSlot<IdListP<T> >::raiseListener(this); }
			
			Sim::IdType getId() const { return mId; }
			
		private:
			std::string mName;
			Sim::IdType mId;
	};
	
	template<class T>
	class IdListC : public Constraint, public ListenerSlot<IdListC<T> > {
		public:
			typedef std::vector<std::string> StringVec;
			typedef boost::unordered_set<Sim::IdType> IdSet;
			
			IdListC(MetaParam* parent) : Constraint(parent),
				mIsAlwaysValid(false) {}
			
			void readConstraint(Script::Data& data) {
				for(size_t i=0; i<data.argCount(); i++)
					mNameVec.push_back(data.getArg(i));
			}
			void postProcess(ExtSim& extsim)
			{
				for(StringVec::iterator i=mNameVec.begin(); i!=mNameVec.end();
					++i) {
					// Special case: Matches any identifier
					if(*i == "*")
						mIsAlwaysValid=true;
					
					Sim::IdType id = translateNameToId<T>(extsim, *i);
					mIdSet.insert( id );
				}
			}
			
			bool isValid(Param* param, ExtSim& extsim) const {
				IdListP<T> *pIdList = static_cast<IdListP<T>*>(param);
				
				return mIsAlwaysValid ||
					mIdSet.find(pIdList->getId())!=mIdSet.end();
			}
			
			void callback()
			{ ListenerSlot<IdListC<T> >::raiseListener(this); }
			
		private:
			StringVec mNameVec;
			IdSet mIdSet;
			bool mIsAlwaysValid;
	};
	
	template<class T>
	class MetaIdList : public MetaParam {
		public:
			MetaIdList(const std::string& dataName) : MetaParam(dataName),
				mDefault(this), mConstraint(this) {}
			virtual ~MetaIdList() {}
			
			virtual Param* constructParam()
			{ return new IdListP<T>(mDefault); }
			
			MetaParam* clone()
			{ return new MetaIdList(*this); }
			
			virtual void readParam(Script::Data& data)
			{ mDefault.readParam(data); }
			virtual void readConstraint(Script::Data& data)
			{ mConstraint.readConstraint(data); }
			
			virtual void postProcess(ExtSim& extsim) {
				mDefault.postProcess(extsim);
				mConstraint.postProcess(extsim);
			}
			
			Param* getDefaultParam() { return &mDefault; }
			Constraint* getDefaultConstraint() { return &mConstraint; }
			
		private:
			IdListP<T> mDefault;
			IdListC<T> mConstraint;
	};
}

#endif
