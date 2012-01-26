#include "BotWeapon.h"
#include "../Bot.h"
#include "../Simulation.h"

namespace Sim {
	BotWeapon::BotWeapon() : mHost(0)
	{}
	
	BotWeapon::~BotWeapon()
	{}
	
	void BotWeapon::initialize(Bot* host)
	{
		mHost = host;
	}
	
	void BotWeapon::addOrder(uint32_t weapIndex, Save& arg)
	{
		mOrder.push_back( Order(weapIndex, Save(arg)) );
	}
	
	/**
	 * Utility function to simplify weapon retrieval.
	 * 
	 * @param index The \c WeaponBox index of the weapon to get.
	 * 
	 * @warning Do not use if this object is not attached to a bot.
	 */
	Weapon* BotWeapon::getWeapon(uint32_t index)
	{	return mHost->mSim->getState().getWeaponFactory().
			getWeapon(mWeaponBox.get(index)); }

	
	void BotWeapon::step(double delta)
	{
		// Process all weapons
		for(uint32_t i=0; i<mWeaponBox.size(); i++) {
			Weapon *w = getWeapon(i);
			if(w)
				w->step(delta);
		}
		
		// Execute all firing orders
		for(OrderVec::iterator i=mOrder.begin(); i!=mOrder.end(); ++i) {
			Order &order = *i;
			Weapon *w = getWeapon(order.mIndex);
			if(w)
				w->shoot(mHost, order.mArg);
		}
		
		// Clear the order queue as no order is ever repeated
		mOrder.clear();
	}
	
	void BotWeapon::save(Save::BasePtr& fp) const
	{
		fp.writeCtr(mOrder);
		fp << mWeaponBox;
	}
	
	void BotWeapon::load(Save::BasePtr& fp)
	{
		fp.readCtr(mOrder);
		fp >> mWeaponBox;
	}
}
