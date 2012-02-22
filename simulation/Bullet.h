#ifndef SIM_BULLET_H
#define SIM_BULLET_H

#include "Factory.h"
#include "Body.h"
#include "Save.h"

#include "StateObj.h"

namespace Sim {
	class Simulation;
	class BulletDatabase;
	
#define SIM_BULLET_HEADER(name) \
	static const std::string &getTypeName() \
	{ static std::string typeName = name; return typeName; }
	
	class Bullet {
		public:
			typedef BulletDatabase TypeDatabase;
			
			IdType getId() const { return mId; }
			IdType getTypeId() const { return mTypeId; }
			const Body &getBody() const { return mBody; }
			
			bool isDead() { return false; }
			
			Bullet(Simulation *sim, IdType id, IdType typeId) :
				mId(id), mTypeId(typeId), mSim(sim) {}
			virtual ~Bullet() {}
			
		protected:
			/// @name Interaction
			//@{
				virtual void save(Save::BasePtr &fp) const;
				virtual void load(Save::BasePtr &fp);
				
				virtual void step(double stepTime);
			//@}
			
			/// @name Identification
			//@{
				IdType mId;
				IdType mTypeId;
				Simulation *mSim;
			//@}
			
			/// @name Physical
			//@{
				Body mBody;
			//@}
			
			friend class BulletFactory;
			friend class DefaultUidFactory<Bullet>;
	};
	
	class BulletFactory : public DefaultUidFactory<Bullet> {
		public:
			BulletFactory(Simulation *sim);
			~BulletFactory();
			
			void destroyBullet(IdType id) { removeObj(id); }
			Bullet *getBullet(IdType id) { return getObject(id); }
			
		private:
			/// @name Factory-required functions
			//@{
				void deleteInstance(Bullet* obj) { delete obj; }
			//@}
	};
}

#endif
