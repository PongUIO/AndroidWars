#include "Simulation.h"
#include "Bullet.h"

#include "data/BulletD.h"

namespace Sim {
	BulletFactory::BulletFactory(Simulation* sim) :
		Factory<Bullet>()
	{}
	
	BulletFactory::~BulletFactory()
	{}
	
	void BulletFactory::startup()
	{
	}
	
	void BulletFactory::shutdown()
	{
		killAll();
	}
	
	uint32_t BulletFactory::createBullet(const Sim::Bullet::Config& cfg)
	{
		uint32_t id = newId();
		
		addObj(new Bullet(mSim, id, cfg));
		
		return id;
	}
	
	void BulletFactory::save(Save::BasePtr& fp)
	{
	}
	
	Bullet::Bullet(Simulation* sim, uint32_t id, const Bullet::Config& cfg) :
		mId(id),
		mType(cfg.type),
		mSim(sim)
	{
		mBody.mPos = cfg.pos;
		
		mTypePtr = mSim->getData().getBulletDb().getType(mType);
		
		mStateRef = StateSys::Reference(mTypePtr->getStateSys());
	}

	Bullet::~Bullet()
	{}

}
