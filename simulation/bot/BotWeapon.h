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
	class BotWeapon {
		public:
			BotWeapon();
			~BotWeapon();
			
			void addOrder(uint32_t weapIndex, Save &arg);
			void addWeapon(uint32_t id) { mWeaponBox.add(id); }
			
			WeaponBox &getWeaponBox() { return mWeaponBox; }
			Weapon *getWeapon(uint32_t index);
			
		private:
			struct Order {
				Order(uint32_t index=0, const Save &arg=Save()) :
					mIndex(index), mArg(arg) {}
				
				uint32_t mIndex;
				Save mArg;
				
				void save(Save::BasePtr &fp) const {
					fp.writeInt<uint32_t>(mIndex);
					fp.writeSave(mArg);
				}
				void load(Save::BasePtr &fp) {
					mIndex = fp.readInt<uint32_t>();
					mArg = fp.readSave();
				}
			};
			typedef std::vector<Order> OrderVec;
			
			void initialize(Bot *host);
			
			void step(double delta);
			
			void save(Save::BasePtr &fp);
			void load(Save::BasePtr &fp);
			
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
