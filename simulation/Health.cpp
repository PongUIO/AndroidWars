#include <boost/bind.hpp>

#include "Health.h"
#include "Simulation.h"
#include "data/DamageD.h"
#include "data/ArmorD.h"

namespace Sim {
	// Health
	//
	//
	
	/**
	 * Adds a health attachment to this hull.
	 * 
	 * If we already have an attachment of that armor type, merge the data.
	 * 
	 * If the modified/added attachment is dead at the end of this call,
	 * then it is removed.
	 */
	void Health::addAttachment(const Hull& data)
	{
		AttachmentVec::iterator pos = findType(data.getType());
		
		if(pos == mAttachments.end()) {
			mAttachments.push_back(data);
			pos = mAttachments.begin()+mAttachments.size()-1;
		} else {
			Hull &internal = *pos;
			
			internal.addHealth(data.getHealth());
			internal.addMaxHealth(data.getMaxHealth());
		}
		
		if(pos != mAttachments.end()) {
			if((*pos).isDead())
				mAttachments.erase(pos);
		}
	}
	
	Health::AttachmentVec::iterator Health::findType(uint32_t type)
	{
		for(AttachmentVec::iterator i=mAttachments.begin();
			i!=mAttachments.end(); i++) {
			if( (*i).getType() == type )
				return i;
		}
		
		return mAttachments.end();
	}
	
	/**
	 * Causes damage based on a given damage type.
	 * 
	 * Damage will be caused until any of the following happens:
	 * - The total damage caused is equal to the input damage.
	 * - No damage was caused (this is either due to immunity or death)
	 * 
	 * Damage is caused in the following order:
	 * - Attachments are always damaged first, and the attachment taking the
	 * 		least damage is always damaged first, even if immune.
	 * - The core is damaged last, if any attachment is immune to the damage
	 * 		type, then the core is never harmed.
	 * 
	 * If the flag \c ignoreAttachments is set, then damage "phases through"
	 * all attachments.
	 * 
	 * @param damage How much health loss to cause.
	 */
	void Health::causeDamage(Simulation* sim, int32_t damage, uint32_t dmgType,
		bool ignoreAttachments)
	{
		ArmorDatabase &armorDatabase = sim->getData().getArmorDb();
		int32_t damageLeft = abs(damage);
		int damageSign = (damage<0) ? -1 : 1;
		
		while(damageLeft > 0) {
			Hull &curHull = ignoreAttachments ? mCore :
				selectBestHull(sim, dmgType);
			
			const ArmorD *armorData = armorDatabase.getArmor(curHull.getType());
			const ArmorD::DamageRule &rule = armorData->getDamageRule(dmgType);
			
			// If the current hull ignores the damage, it must mean
			// that we're targetting the core.
			//
			// Damage immunity also causes the loop to end, as
			// there is no possiblity of ever breaking the hull.
			if(rule.mIsIgnoring || rule.mMultiplier==0.0)
				break;
			
			// Calculate the real damage caused, then
			// adjust the result to find how much damage is left to cause
			// 
			// The real damage is always rounded down
			int32_t realDamage = damageSign*damageLeft*rule.mMultiplier;
			int32_t causedDamage = curHull.addHealth(-realDamage);
			
			// Prevents infinite loops
			if(causedDamage == 0)
				break;
			
			// The damage caused is based on the rule multiplier.
			// This value is scaled back by the multiplier, and ceiled to
			// always force damageLeft down.
			// 
			// This works both ways, if the attack healed the target, it
			// still forces damageLeft to decrease.
			damageLeft -= fabs(ceil(double(causedDamage)/rule.mMultiplier));
		}
		
		// Clean out all destroyed attachments
		clearDeadAtm();
	}
	
	/**
	 * Removes all dead attachments
	 */
	void Health::clearDeadAtm()
	{
		for(AttachmentVec::iterator i=mAttachments.begin();
			i!=mAttachments.end();) {
			if( (*i).isDead() )
				i = mAttachments.erase(i);
			else
				i++;
		}
	}

	
	/**
	 * Selects the best hull to cause the given damage type to.
	 * 
	 * If no attachment is affected by the damage type, then
	 * the core is returned.
	 */
	Health::Hull& Health::selectBestHull(Simulation *sim, uint32_t dmgType)
	{
		ArmorDatabase &armorDatabase = sim->getData().getArmorDb();
		
		AttachmentVec::iterator bestAtm = mAttachments.end();
		double bestMultiplier = 0.0;
		for(AttachmentVec::iterator i=mAttachments.begin();
			i!=mAttachments.end(); i++) {
			
			if( (*i).isDead() )
				continue;
			
			const ArmorD *armorData =
				armorDatabase.getArmor( (*i).getType() );
			const ArmorD::DamageRule &dmgRule =
				armorData->getDamageRule(dmgType);
			
			if( !armorData->getDamageRule(dmgType).mIsIgnoring ) {
				
				if(	bestAtm == mAttachments.end() ||
					dmgRule.mMultiplier < bestMultiplier) {
					bestAtm = i;
					bestMultiplier = dmgRule.mMultiplier;
				}
			}
		}
		
		if(bestAtm == mAttachments.end())
			return mCore;
		else
			return (*bestAtm);
	}
	
	void Health::save(Save::BasePtr& fp)
	{
		fp.writeCtr(mAttachments, boost::bind(&Hull::save, _1, _2) );
		mCore.save(fp);
	}

	void Health::load(Save::BasePtr& fp)
	{
		fp.readCtr(mAttachments, boost::bind(&Hull::load, _1, _2) );
		mCore.load(fp);
	}
	
	
	// Health::Hull
	//
	//
	/**
	 * Adds or removes health from this hull.
	 * 
	 * Takes care of capping the health to avoid going over the maximum or
	 * below zero.
	 * 
	 * @return The real amount of health added.
	 */
	int32_t Health::Hull::addHealth(int32_t amount)
	{
		uint32_t oldHealth = getHealth();
		int32_t newHealth = getHealth()+amount;
		
		if(newHealth < 0)
			newHealth = 0;
		else if(newHealth > int32_t(getMaxHealth()))
			newHealth = getMaxHealth();
		
		mHealth = newHealth;
		return int32_t(getHealth()) - int32_t(oldHealth);
	}
	
	/**
	 * Adds or removes maximum health.
	 * 
	 * Takes care of lowering ordinary health if max health falls below
	 * the current health.
	 * 
	 * @param adjustHealth If true will adjust ordinary health by the same
	 * amount.
	 */
	int32_t Health::Hull::addMaxHealth(int32_t amount, bool adjustHealth)
	{
		uint32_t oldMaxHealth = int32_t(mMaxHealth);
		int32_t newMaxHealth = int32_t(mMaxHealth)+amount;
		
		if(newMaxHealth < 0)
			newMaxHealth = 0;
		
		mMaxHealth = newMaxHealth;
		
		int32_t realAmount = int32_t(getMaxHealth()) - oldMaxHealth;
		if(adjustHealth)
			addHealth(realAmount);
		
		return realAmount;
	}
}
