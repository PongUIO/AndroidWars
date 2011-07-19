#include "../BulletSys.h"

namespace Sim {
	BulletSys::Reference::Reference(Simulation* sim, BulletSys* sys) :
		mSim(sim), mSystem(sys), mActive(NULL)
	{
		mActive = sys->getRoot();
	}
	
	BulletSys::Reference::~Reference()
	{
	}
	
	
	// Thread functions
	//
	//
	
	BulletSys::Thread::Thread(Reference *ref) :
		mRef(ref)
	{
		mA
	}
	
	bool BulletSys::Reference::Thread::exec()
	{
		if(mActive != NULL)
			mActive->exec(*this);
		
		return mActive!=NULL;
	}
}
