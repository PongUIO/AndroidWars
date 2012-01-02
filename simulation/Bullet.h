#ifndef SIM_BULLET_H
#define SIM_BULLET_H

#include "Factory.h"
#include "Body.h"
#include "Save.h"

#include "StateObj.h"

namespace Sim {
	class Simulation;
	
#define SIM_BULLET_HEADER(name) \
	static const std::string &getTypeName() \
	{ static std::string typeName = name; return typeName; }
	
	class Bullet {
		public:
			uint32_t getId() const { return mId; }
			uint32_t getTypeId() const { return mTypeId; }
			const Body &getBody() const { return mBody; }
			
			bool isDead() { return false; }
			
		protected:
			Bullet(Simulation *sim, uint32_t id, uint32_t typeId) :
				mId(id), mTypeId(typeId), mSim(sim) {}
			virtual ~Bullet() {}
			
			/// @name Interaction
			//@{
				virtual void save(Save::BasePtr &fp);
				virtual void load(Save::BasePtr &fp);
				
				virtual void step(double stepTime);
			//@}
			
			/// @name Identification
			//@{
				uint32_t mId;
				uint32_t mTypeId;
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
			
			void destroyBullet(uint32_t id) { removeObj(id); }
			Bullet *getBullet(uint32_t id) { return getObject(id); }
			
		private:
			/// @name Factory-required functions
			//@{
				void deleteInstance(Bullet* obj) { delete obj; }
				
				const DataBehaviourT<Bullet>::Behaviour* getBehaviourFromId(uint32_t id) const;
				const DataBehaviourT<Bullet>::Behaviour* getBehaviourFromName(const std::string& name) const;
			//@}
	};
}

#endif
