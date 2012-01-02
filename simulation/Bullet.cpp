#include <stdio.h>
#include "Simulation.h"
#include "Bullet.h"

#include "data/BulletD.h"

namespace Sim {
	BulletFactory::BulletFactory(Simulation* sim) :
		DefaultUidFactory<Bullet>(sim)
	{}
	
	BulletFactory::~BulletFactory()
	{}
	
	const DataBehaviourT< Bullet >::Behaviour* BulletFactory::
		getBehaviourFromId(uint32_t id) const
	{	return mSim->getData().getBulletDb().getType(id); }

	const DataBehaviourT< Bullet >::Behaviour* BulletFactory::
		getBehaviourFromName(const std::string& name) const
	{	return mSim->getData().getBulletDb().getType(name); }

	
	// Bullet class
	//
	//
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
