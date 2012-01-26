#ifndef SIM_HEALTH_H
#define SIM_HEALTH_H

#include <vector>
#include <stdint.h>

#include "Save.h"
#include "Common.h"

namespace Sim {
	class Simulation;
	class ArmorD;
	
	class Health : private Save::OperatorImpl<Health> {
		public:
			explicit Health(IdType coreType=0, uint32_t coreHealth=0) :
				mCore(coreType,coreHealth) {}
			~Health() {}
			
			class Hull : private Save::OperatorImpl<Hull> {
				public:
					Hull(IdType type=0, uint32_t maxHealth=0) :
						mArmorType(type), mHealth(maxHealth),
						mMaxHealth(maxHealth)
						{}
					
					void operator+=(const Hull &b)
					{	addHealth(b.getHealth());
						addMaxHealth(b.getMaxHealth()); }
					void operator-=(const Hull &b)
					{	addHealth(-b.getHealth());
						addMaxHealth(-b.getMaxHealth()); }
					
					int32_t addHealth(int32_t amount);
					int32_t addMaxHealth(int32_t amount,
						bool adjustHealth=true);
					void setType(IdType type) { mArmorType = type; }
					
					IdType getType() const { return mArmorType; }
					uint32_t getHealth() const { return mHealth; }
					uint32_t getMaxHealth() const { return mMaxHealth; }
					
					bool isDead() const { return getHealth()<=0; }
					bool isRemovable() const { return getMaxHealth()<=0; }
					
					void save(Save::BasePtr &fp) const
						{ fp << mArmorType << mHealth << mMaxHealth; }
					void load(Save::BasePtr &fp)
						{ fp >> mArmorType >> mHealth >> mMaxHealth; }
					
				private:
					IdType mArmorType;
					uint32_t mHealth;
					uint32_t mMaxHealth;
			};
			typedef std::vector<Hull> AttachmentVec;
			
			Hull &getCore() { return mCore; }
			const Hull &getCore() const { return mCore; }
			const AttachmentVec &getAttachments() const { return mAttachments; }
			bool isDead() const { return mCore.isDead(); }
			
			void addAttachment(const Hull &data);
			void removeAttachment(const Hull &data);
			
			void causeDamage(Simulation *sim, int32_t damage, IdType dmgType,
				bool ignoreAttachments=false);
			
			void save(Save::BasePtr &fp) const;
			void load(Save::BasePtr &fp);
			
		private:
			AttachmentVec::iterator findType(IdType type);
			AttachmentVec::iterator findOrCreateType(IdType type);
			Hull &selectBestHull(Simulation *sim, IdType dmgType);
			void clearDeadAtm();
			
			AttachmentVec mAttachments;
			Hull mCore;
	};
}

#endif
