#ifndef SIM_BULLET_H
#define SIM_BULLET_H

#include "Factory.h"
#include "Body.h"
#include "Save.h"

#include "StateObj.h"
#include "statesys/StateSys.h"

namespace Sim {
	class Simulation;
	class BulletD;
	
	class Bullet {
		public:
			struct Config {
				uint32_t type;
				
				Vector pos;
			};
			
			enum EntryType {
				EGroundCol = 0,
				EUser
			};
			
			uint32_t getId() const { return mId; }
			
		private:
			Bullet(Simulation *sim, uint32_t id, const Config &cfg);
			~Bullet();
			
			/// @name Interaction
			//@{
				bool isDead() { return false; }
				void step(double stepTime);
				
				void save(Save::BasePtr &fp);
			//@}
			
			/// @name Identification
			//@{
				uint32_t mId;
				uint32_t mType;
				
			//@}
			
			/// @name Physical
			//@{
				Body mBody;
				
				const BulletD *mTypePtr;
				Simulation *mSim;
			//@}
			
			
			/// @name State system
			//@{
				StateSys::Reference mStateRef;
			//@}
			
			friend class BulletFactory;
			friend class Factory<Bullet>;
			friend class DefaultFactory<Bullet>;
	};
	
	namespace BulletState {
	}
	
	class BulletFactory : public DefaultFactory<Bullet> {
		public:
			/// @name Initialization
			//@{
				BulletFactory(Simulation *sim);
				~BulletFactory();
			//@}
	};
}

#endif
