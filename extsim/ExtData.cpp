#include "ExtData.h"

#include "nepeta.h"

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
		mWeapon(esim),
		mGame(esim),
		mMap(esim),
		
		mCurrentContext(LcNone),
		mExtSim(&esim)
	{
		registerListener("ARMOR", Listener(&mArmor, LcDataLoading) );
		registerListener("DAMAGE", Listener(&mDamage, LcDataLoading) );
		registerListener("BOT", Listener(&mBot, LcDataLoading) );
		registerListener("PROGRAM", Listener(&mProgram, LcDataLoading) );
		registerListener("WEAPON", Listener(&mWeapon, LcDataLoading) );
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
		Nepeta daScr;
		daScr.compile(data);
		
		if(daScr.getError().size()>0)
			return;
		
		for(size_t i=0, nc=daScr.getRoot().getNodeCount(); i<nc; i++) {
			Nepeta::Node &node = daScr.getRoot().getNode(i);
			
			ListenerMapPair lmp = mListeners.equal_range(node.getId());
			for(ListenerMap::iterator i=lmp.first; i!=lmp.second; ++i) {
				i->second.mData->loadNode(node);
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
	{	mListeners.insert( std::make_pair(blockTag,listener) ); }
}
