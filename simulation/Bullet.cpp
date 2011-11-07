#include <stdio.h>
#include "Simulation.h"
#include "Bullet.h"

#include "data/BulletD.h"

namespace Sim {
	BulletFactory::BulletFactory(Simulation* sim) :
		DefaultFactory<Bullet>(sim)
	{}
	
	BulletFactory::~BulletFactory()
	{}
	
	// Bullet class
	//
	//
	Bullet::Bullet(Simulation* sim, uint32_t id, const Bullet::Config& cfg) :
		mId(id),
		mType(cfg.type),
		mSim(sim)
	{
		mBody.mPos = cfg.pos;
		mBody.mVel = cfg.vel;
		
		mTypePtr = mSim->getData().getBulletDb().getType(mType);
		
		mStateRef = StateSys::Reference(mTypePtr->getStateSys());
	}

	Bullet::~Bullet()
	{}
	
	void Bullet::step(double stepTime)
	{
		mBody.step(stepTime);
	}
	
	void Bullet::save(Save::BasePtr& fp)
	{
		mBody.save(fp);
	}
	
	void Bullet::load(Save::BasePtr& fp)
	{
		mBody.load(fp);
	}
}
