#include <boost/foreach.hpp>
#include "ExtData.h"

#include "nepeta.h"

#define foreach BOOST_FOREACH

namespace exts {
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
		
		mExtSim(esim)
	{
		mActiveListener = mDefaultListener = new ListenerGroup();
		
		registerListener("ARMOR", &mArmor);
		registerListener("DAMAGE", &mDamage);
		registerListener("BOT", &mBot);
		registerListener("PROGRAM", &mProgram);
		registerListener("WEAPON", &mWeapon);
		registerListener("GAME", &mGame);
		registerListener("MAP", &mMap);
	}
	
	ExtData::~ExtData()
	{
		if(mActiveListener)
			delete mActiveListener;
	}
	
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
		
		for(Nepeta::Iterator i=daScr.getRoot().begin();
		daScr.getRoot().hasNext(i); daScr.getRoot().next(i))
			mActiveListener->loadNode(daScr.getRoot().getIterNode(i));
	}
	
	/**
	 * @brief Performs post processing for the active listener
	 */
	void ExtData::postProcess()
	{	mActiveListener->postProcess(); }
	
	void ExtData::registerListener(const std::string& blockTag, DataListener* entry)
	{	mActiveListener->registerListener(blockTag, entry); }

	void ExtData::switchContext(ListenerGroup* ctx)
	{
		mActiveListener = ctx;
		if(mActiveListener == 0)
			mActiveListener = mDefaultListener;
	}
	
	// ListenerEntry
	// 
	// 
	void ListenerEntry::loadNode(const Nepeta::Node& node)
	{
		foreach(DataListener *listener, mListeners)
			listener->loadNode(node);
	}
	
	void ListenerEntry::postProcess()
	{
		foreach(DataListener *listener, mListeners)
			listener->postProcess();
	}
	
	// ListenerGroup
	// |
	// 
	ListenerGroup::~ListenerGroup()
	{
		foreach(ListenerMap::value_type& entry, mListenerMap)
			delete entry.second;
	}
	
	void ListenerGroup::loadNode(const Nepeta::Node& node)
	{
		ListenerMap::iterator i=mListenerMap.find(node.getId());
		if(i!=mListenerMap.end())
			i->second->loadNode(node);
	}

	void ListenerGroup::postProcess()
	{
		foreach(ListenerMap::value_type& entry, mListenerMap)
			entry.second->postProcess();
	}
	
	void ListenerGroup::registerListener(const std::string& str,
		DataListener* L)
	{
		ListenerMap::iterator i=mListenerMap.insert(
		std::make_pair(str,static_cast<ListenerEntry*>(0)) ).first;
		
		if(i->second == 0)
			i->second = new ListenerEntry();
		
		i->second->registerListener(L);
	}
}
