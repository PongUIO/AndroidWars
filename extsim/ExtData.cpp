#include "ExtData.h"

#include "../dascript/script.h"

namespace ExtS {
	ExtData::ExtData(Sim::Simulation& sim) :
		mArmor(sim),
		mDamage(sim),
		mBot(sim),
		
		mSim(sim)
	{
		registerListener("ARMOR", &mArmor);
		registerListener("DAMAGE", &mDamage);
		registerListener("BOT", &mBot);
	}
	
	ExtData::~ExtData()
	{}
	
	/**
	 * @brief Loads a script string.
	 * 
	 * Compiles a script from a data string. Each \c Script::Block in the
	 * root scope is iterated through in sequence, and control is passed
	 * on to any \c BaseData listener who matches the block's identifier.
	 */
	void ExtData::loadScript(const std::string& data)
	{
		Script daScr;
		daScr.compile(data);
		
		if(daScr.getError().size()>0)
			return;
		
		for(size_t i=0; i<daScr.getRoot().getSize(); i++) {
			Script::Block *block = daScr.getRoot().getBlockIndex(i);
			if(block) {
				BaseData *listener = getListener(block->getId());
				
				if(listener)
					listener->loadBlock(*block);
			}
		}
	}
	
	/**
	 * Calls \c BaseData::postProcess for all listeners.
	 */
	void ExtData::postProcess()
	{
		for(ListenerMap::iterator i=mListeners.begin();
			i!=mListeners.end(); i++) {
			i->second->postProcess();
		}
	}
	
	void ExtData::registerListener(const std::string& blockTag, BaseData* db)
	{	mListeners[blockTag] = db; }
	
	BaseData* ExtData::getListener(const std::string& tag)
	{
		ListenerMap::iterator i=mListeners.find(tag);
		return (i==mListeners.end()) ? 0 : i->second;
	}
}
