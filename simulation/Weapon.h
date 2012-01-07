#ifndef SIM_WEAPON_H
#define SIM_WEAPON_H

#include <vector>

#include "StateObj.h"
#include "Factory.h"

namespace Sim {
	class Simulation;
	
#define SIM_WEAPON_HEADER(name) \
	static const std::string &getTypeName() \
	{ static std::string typeName = name; return typeName; }
	
	class Weapon {
		public:
			uint32_t getId() const { return mId; }
			uint32_t getTypeId() const { return mTypeId; }
			Simulation *getSim() const { return mSim; }
			
			bool isDead() { return mIsDead; }
			
			/// @name Type information
			//@{
			//@}
			
		protected:
			Weapon(Simulation *sim, uint32_t id, uint32_t typeId) :
				mId(id), mTypeId(typeId), mSim(sim) {}
			virtual ~Weapon() {}
			
			/// @name Interaction
			//@{
				virtual void save(Save::BasePtr &fp)=0;
				virtual void load(Save::BasePtr &fp)=0;
				
				virtual void step(double stepTime)=0;
				virtual void shoot()=0;
			//@}
			
			/// @name Identification
			//@{
				uint32_t mId;
				uint32_t mTypeId;
				Simulation *mSim;
			//@}
			
			/// @name Behaviour
			//@{
				double mReloadTimer;
				
				bool mIsDead;
			//@}
			
			// Bots have access to internal calls
			friend class Bot;
			friend class DefaultUidFactory<Weapon>;
			friend class WeaponFactory;
	};
	
	/**
	 * Contains a vector of weapons.
	 */
	class WeaponBox {
		public:
			typedef std::vector<uint32_t> WeaponVec;
			
			WeaponBox() {}
			
			uint32_t add(uint32_t wid) {
				mData.push_back(wid);
				return mData.size()-1;
			}
			uint32_t get(uint32_t index)
				{ return mData.at(index); }
			
			void save(Save::BasePtr &fp);
			void load(Save::BasePtr &fp);
			
		private:
			WeaponVec mData;
	};
	
	class WeaponFactory : public DefaultUidFactory<Weapon> {
		public:
			WeaponFactory(Simulation *sim);
			~WeaponFactory();
			
			/// @name StateObj functions
			//@{
				// All functions use the standard implementation
			//@}
			
			void destroyWeapon(uint32_t id) { removeObj(id); }
			Weapon *getWeapon(uint32_t id) { return getObject(id); }
			
		private:
			/// @name Factory-required functions
			//@{
				void deleteInstance(Weapon* obj) { delete obj; }
				
				const DataBehaviourT<Weapon>::Behaviour* getBehaviourFromId(uint32_t id) const;
				const DataBehaviourT<Weapon>::Behaviour* getBehaviourFromName(const std::string& name) const;
			//@}
	};
}

#endif
