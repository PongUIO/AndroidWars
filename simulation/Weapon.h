#ifndef SIM_WEAPON_H
#define SIM_WEAPON_H

#include <vector>

#include "StateObj.h"
#include "statesys/StateSys.h"
#include "Factory.h"

namespace Sim {
	class Simulation;
	class WeaponD;
	
	class Weapon {
		public:
			struct Config {
				uint32_t type;
			};
			
			struct ShootArg {
				Vector pos;
				Vector dir;
				
				Weapon *ref;
			};
			
			uint32_t getId() const { return mId; }
			Simulation *getSim() const { return mSim; }
			
		private:
			Weapon(Simulation *sim, uint32_t id) : mId(id), mSim(sim) {}
			Weapon(Simulation *sim, uint32_t id, const Config &cfg);
			~Weapon();
			
			/// @name Interaction
			//@{
				bool isDead() { return mIsDead; }
				void step(double stepTime);
				
				void shoot(ShootArg arg, uint32_t style);
				
				void save(Save::BasePtr &fp);
				void load(Save::BasePtr &fp);
			//@}
			
			/// @name Identification
			//@{
				uint32_t mId;
				uint32_t mType;
				
				const WeaponD *mTypePtr;
				Simulation *mSim;
			//@}
			
			/// @name Behaviour
			//@{
				double mReloadTimer;
				
				bool mIsDead;
			//@}
			
			/// @name State system
			//@{
				StateSys::Reference mStateRef;
			//@}
			
			// Bots have access to internal calls
			friend class Bot;
			friend class Factory<Weapon>;
			friend class DefaultFactory<Weapon>;
	};
	
	/**
	 * Contains a vector of weapons.
	 */
	class WeaponBox {
		public:
			typedef std::vector<uint32_t> WeaponVec;
			
			uint32_t add(uint32_t wid) {
				mData.push_back(wid);
				return mData.size()-1;
			}
			uint32_t get(uint32_t index)
				{ return mData.at(index); }
			
		private:
			WeaponVec mData;
	};
	
	namespace WeaponState {
		class Shoot : public StateSys::State {
			public:
				Shoot(uint32_t bulletType) : mType(bulletType) {}
				
				void exec(StateSys::Reference::Thread &t) const;
				
			private:
				uint32_t mType;
		};
	}
	
	class WeaponFactory : public DefaultFactory<Weapon> {
		public:
			WeaponFactory(Simulation *sim);
			~WeaponFactory();
			
			void save(Save::BasePtr &fp) {}
			void load(Save::BasePtr &fp) {}
	};
}

#endif
