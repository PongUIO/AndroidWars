#ifndef SIM_WEAPON_H
#define SIM_WEAPON_H

#include <vector>

#include "StateObj.h"
#include "Factory.h"

namespace Sim {
	class Simulation;
	class Bot;
	
#define SIM_WEAPON_HEADER(name) \
	static const std::string &getTypeName() \
	{ static std::string typeName = name; return typeName; }
	
	/**
	 * Base class for a weapon.
	 */
	class Weapon {
		public:
			IdType getId() const { return mId; }
			IdType getTypeId() const { return mTypeId; }
			Simulation *getSim() const { return mSim; }
			
			bool isDead() { return mIsDead; }
			
			uint32_t getReloadTime() const { return mReloadTimer; }
			
		protected:
			Weapon(Simulation *sim, IdType id, IdType typeId) :
				mId(id), mTypeId(typeId), mSim(sim), mIsDead(false) {}
			virtual ~Weapon() {}
			
			/// @name Interaction
			//@{
				virtual void save(Save::BasePtr &fp)=0;
				virtual void load(Save::BasePtr &fp)=0;
				
				virtual void step(double stepTime)=0;
				
				virtual void shoot(Bot *bot, Save &weapArg)=0;
			//@}
			
			/// @name Identification
			//@{
				IdType mId;
				IdType mTypeId;
				Simulation *mSim;
			//@}
			
			/// @name Behaviour
			//@{
				uint32_t mReloadTimer;
				
				void saveSys(Save::BasePtr &fp) const
				{ fp << mReloadTimer; }
				void loadSys(Save::BasePtr &fp)
				{ fp >> mReloadTimer; }
				
				bool mIsDead;
			//@}
			
			// Bots have access to internal calls
			friend class Bot;
			friend class BotWeapon;
			friend class DefaultUidFactory<Weapon>;
			friend class WeaponFactory;
	};
	
	/**
	 * Contains a vector of weapons.
	 */
	class WeaponBox : private Save::OperatorImpl<WeaponBox> {
		public:
			typedef std::vector<IdType> WeaponVec;
			
			WeaponBox() {}
			
			IdType add(IdType wid) {
				mData.push_back(wid);
				return mData.size()-1;
			}
			IdType get(IdType i)
				{ return (i<mData.size()) ? mData[i] : NoId; }
			
			IdType size()
				{ return mData.size(); }
			
			void save(Save::BasePtr &fp) const;
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
			
			void destroyWeapon(IdType id) { removeObj(id); }
			Weapon *getWeapon(IdType id) { return getObject(id); }
			
		private:
			/// @name Factory-required functions
			//@{
				void saveObj(Weapon* obj, Save::BasePtr& fp);
				Weapon* loadObj(IdType internalId, Save::BasePtr& fp);
				
				void deleteInstance(Weapon* obj) { delete obj; }
				
				const DataBehaviourT<Weapon>::Behaviour* getBehaviourFromId(IdType id) const;
				const DataBehaviourT<Weapon>::Behaviour* getBehaviourFromName(const std::string& name) const;
			//@}
	};
}

#endif
