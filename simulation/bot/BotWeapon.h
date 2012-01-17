#ifndef SIM_BOTWEAPON_H
#define SIM_BOTWEAPON_H

#include <vector>
#include "../Save.h"
#include "../Weapon.h"

namespace Sim {
	class Bot;
	class Weapon;
	
	/**
	 * Manages all weapons for a bot.
	 */
	class BotWeapon : private Save::OperatorImpl<BotWeapon> {
		public:
			BotWeapon();
			~BotWeapon();
			
			void addOrder(uint32_t weapIndex, Save &arg);
			void addWeapon(IdType id) { mWeaponBox.add(id); }
			
			WeaponBox &getWeaponBox() { return mWeaponBox; }
			Weapon *getWeapon(uint32_t index);
			
			void save(Save::BasePtr &fp) const;
			void load(Save::BasePtr &fp);
			
		private:
			struct Order : private Save::OperatorImpl<Order> {
				Order(uint32_t index=0, const Save &arg=Save()) :
					mIndex(index), mArg(arg) {}
				
				uint32_t mIndex;
				Save mArg;
				
				void save(Save::BasePtr &fp) const
				{	fp << mIndex << mArg; }
				void load(Save::BasePtr &fp)
				{	fp >> mIndex >> mArg; }
			};
			typedef std::vector<Order> OrderVec;
			
			void initialize(Bot *host);
			
			void step(double delta);
			
			/// @name Constant/temporary data
			//@{
				Bot *mHost;
			//@}
			
			/// @name Dynamic data
			//@{
				OrderVec mOrder;
				WeaponBox mWeaponBox;
			//@}
			
			friend class Bot;
	};
}

#endif
