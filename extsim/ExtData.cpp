#include "ExtData.h"

#include "../dascript/script.h"

namespace ExtS {
#define _EXTS_X(type, name) \
	template<> type &ExtData::getComponent() \
	{ return get##name##Db(); }
	_EXTS_X_EXTDATA_COMPONENTS
#undef _EXTS_X
	
	ExtData::ExtData(ExtSim &esim) :
		mArmor(esim),
		mDamage(esim),
		mBot(esim),
		mProgram(esim),
		mGame(esim),
		mMap(esim),
		
		mCurrentContext(LcNone),
		mExtSim(esim)
	{
		registerListener("ARMOR", Listener(&mArmor, LcDataLoading) );
		registerListener("DAMAGE", Listener(&mDamage, LcDataLoading) );
		registerListener("BOT", Listener(&mBot, LcDataLoading) );
		registerListener("PROGRAM", Listener(&mProgram, LcDataLoading) );
		registerListener("GAME", Listener(&mGame, LcDataLoading) );
		
		registerListener("MAP", Listener(&mMap, LcContentLoading) );
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
				ExtBaseData *listener = getListener(block->getId());
				
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
			if(i->second.mContext & mCurrentContext)
				i->second.mData->postProcess();
		}
	}
	
	void ExtData::registerListener(const std::string& blockTag,
		const Listener &listener)
	{	mListeners[blockTag] = listener; }
	
	ExtBaseData* ExtData::getListener(const std::string& tag)
	{
		ListenerMap::iterator i=mListeners.find(tag);
		return (i==mListeners.end() || !(i->second.mContext&mCurrentContext)) ?
			0 : i->second.mData;
	}
	
	
	
	template<>
	bool ExtData::badCastStrategy(const std::string &str, bool def)
	{
		if(str=="false")
			return false;
		else if(str=="true")
			return true;
		else
			return def;
	}
}
