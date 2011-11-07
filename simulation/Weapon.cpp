#include "Simulation.h"
#include "Weapon.h"

#include "data/WeaponD.h"

namespace Sim {
	WeaponFactory::WeaponFactory(Simulation *sim) :
		DefaultFactory<Weapon>(sim)
	{}
	
	WeaponFactory::~WeaponFactory()
	{}
	
	// Weapon class
	//
	//
	Weapon::Weapon(Simulation* sim, uint32_t id, const Sim::Weapon::Config& cfg) :
		mId(id),
		mType(cfg.type),
		mSim(sim),
		
		mReloadTimer(0.0),
		mIsDead(false)
	{
		mTypePtr = mSim->getData().getWeaponDb().getType(mType);
		
		mStateRef = StateSys::Reference(mTypePtr->getStateSys());
	}
	
	Weapon::~Weapon()
	{}
	
	void Weapon::shoot(ShootArg arg, uint32_t style)
	{
		arg.ref = this;
		mStateRef.startThread(style, arg);
	}
	
	void Weapon::step(double stepTime)
	{
		mStateRef.exec(stepTime);
		if(mReloadTimer > 0.0)
			mReloadTimer -= stepTime;
	}
	
	void Weapon::save(Save::BasePtr& fp)
	{
	}
	
	void Weapon::load(Save::BasePtr& fp)
	{
	}
	
	// State objects
	//
	//
	namespace WeaponState {
		void Shoot::exec(StateSys::Reference::Thread& t) const
		{
			const Weapon::ShootArg &arg =
				boost::any_cast<const Weapon::ShootArg>(t.mArg);
			
			Simulation *sim = arg.ref->getSim();
			Bullet::Config cfg;
			cfg.pos = arg.pos;
			cfg.vel = arg.dir;
			cfg.type = mType;
			sim->getState().getBulletFactory().create(cfg);
			
			t.mActive = nextState();
		}
	}
}
