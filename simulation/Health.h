#ifndef SIM_HEALTH_H
#define SIM_HEALTH_H

#include <vector>
#include <stdint.h>

#include "Save.h"

namespace Sim {
	class Simulation;
	class ArmorD;
	
	class Health {
		public:
			explicit Health(uint32_t coreType=0, uint32_t coreHealth=0) :
				mCore(coreType,coreHealth) {}
			~Health() {}
			
			class Hull {
				public:
					Hull(uint32_t type=0, uint32_t health=0) :
						mArmorType(type), mHealth(health), mMaxHealth(health)
						{}
					
					int32_t addHealth(int32_t amount);
					int32_t addMaxHealth(int32_t amount,
						bool adjustHealth=true);
					
					uint32_t getType() const { return mArmorType; }
					uint32_t getHealth() const { return mHealth; }
					uint32_t getMaxHealth() const { return mMaxHealth; }
					
					bool isDead() const { return getHealth()<=0; }
					
					void save(Save::BasePtr &fp) const {
						fp.writeInt<uint32_t>(mArmorType);
						fp.writeInt<uint32_t>(mHealth);
						fp.writeInt<uint32_t>(mMaxHealth);
					}
					void load(Save::BasePtr &fp) {
						mArmorType = fp.readInt<uint32_t>();
						mHealth = fp.readInt<uint32_t>();
						mMaxHealth = fp.readInt<uint32_t>();
					}
					
				private:
					uint32_t mArmorType;
					uint32_t mHealth;
					uint32_t mMaxHealth;
			};
			typedef std::vector<Hull> AttachmentVec;
			
			Hull &getCore() { return mCore; }
			const AttachmentVec &getAttachments() const { return mAttachments; }
			bool isDead() const { return mCore.isDead(); }
			
			void addAttachment(const Hull &data=Hull());
			
			void causeDamage(Simulation *sim, int32_t damage, uint32_t dmgType,
				bool ignoreAttachments=false);
			
			void save(Save::BasePtr &fp);
			void load(Save::BasePtr &fp);
			
		private:
			AttachmentVec::iterator findType(uint32_t type);
			Hull &selectBestHull(Simulation *sim, uint32_t dmgType);
			void clearDeadAtm();
			
			AttachmentVec mAttachments;
			Hull mCore;
	};
}

#endif
