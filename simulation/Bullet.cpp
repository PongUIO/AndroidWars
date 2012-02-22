#include "Simulation.h"
#include "Bullet.h"

#include "data/BulletD.h"

namespace Sim {
	BulletFactory::BulletFactory(Simulation* sim) :
		DefaultUidFactory<Bullet>(sim)
	{}
	
	BulletFactory::~BulletFactory()
	{}
	
	// Bullet class
	//
	//
	void Bullet::step(double stepTime)
	{
		mBody.step(stepTime);
	}
	
	void Bullet::save(Save::BasePtr& fp) const
	{	fp << mBody; }
	
	void Bullet::load(Save::BasePtr& fp)
	{	fp >> mBody; }
}
